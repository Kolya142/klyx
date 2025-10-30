//    Klyx - an operating system kernel.
//    Copyright (C) 2025 Nikolay Shevelko
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, see <https://www.gnu.org/licenses/>.



#include <klyx/i386_emulator.h>
#include <thirdparty/printf.h>
#include <klyx/console.h>
#include <klyx/kernel.h>
#include <klyx/shred.h>
#include <klyx/hw.h>

word_t interrupt_esp_stack[64] = {0};
word_t *interrupt_esp_stack_ptr = interrupt_esp_stack;

#define INT_START asm volatile ("cli\n"                                 \
                                "pushal\n"                              \
                                "push %%ds\n"                           \
                                "push %%ss\n"                           \
                                "push %%es\n"                           \
                                "push %%fs\n"                           \
                                "push %%gs\n"                           \
                                "movw $0x10, %%dx\n"                    \
                                "mov %%dx, %%ds\n"                      \
                                "mov %%dx, %%ss\n"                      \
                                "mov %%dx, %%es\n"                      \
                                ::: "edx");                             \
    asm volatile ("mov %%esp, %0" : "=r"(*interrupt_esp_stack_ptr));    \
    int_regs_t *regs = (int_regs_t *)(*interrupt_esp_stack_ptr);        \
    interrupt_esp_stack_ptr += 1;

#define INT_END {                                                       \
        interrupt_esp_stack_ptr -= 1;                                   \
        asm volatile ("mov %0, %%esp" :: "r"(*interrupt_esp_stack_ptr)); \
        asm volatile ("pop %gs\n"                                       \
                      "pop %fs\n"                                       \
                      "pop %es\n"                                       \
                      "pop %ss\n"                                       \
                      "pop %ds\n"                                       \
                      "mov $0x20, %al\n"                                \
                      "outb %al, $0xA0\n"                               \
                      "outb %al, $0x20\n"                               \
                      "popal\n"                                         \
                      "iretl");                                         \
    }

#define INT3 asm volatile ("int3")

int errno;

void a(void) {
    for (;;) {
        asm volatile ("mov $2, %%eax\n"
                      "mov $0, %%ebx\n"
                      "mov %0, %%ecx\n"
                      "mov $1, %%edx\n"
                      "int $0x80" :: "r"("A") : "eax", "ebx", "ecx", "edx");
    }
}

void b(void) {
    asm volatile ("mov $2, %%eax\n"
                  "mov $1, %%ebx\n"
                  "mov %0, %%ecx\n"
                  "mov $1, %%edx\n"
                  "int $0x80" :: "r"("B") : "eax", "ebx", "ecx", "edx");
    for (;;) {
        asm volatile ("mov $2, %%eax\n"
                      "mov $1, %%ebx\n"
                      "mov %0, %%ecx\n"
                      "mov $1, %%edx\n"
                      "int $0x80" :: "r"("B") : "eax", "ebx", "ecx", "edx");
        asm volatile ("mov $2, %%eax\n"
                      "mov $1, %%ebx\n"
                      "mov %0, %%ecx\n"
                      "mov $1, %%edx\n"
                      "int $0x80" :: "r"("B") : "eax", "ebx", "ecx", "edx");
        asm volatile ("mov $4, %%eax\n"
                      "int $0x80" ::: "eax");
    }
    asm volatile ("mov $1, %eax\nint $0x80\n");
}

volatile void panic(const char *msg) {
    printf("\n\nKernel panic: `%s`.\nTODO: be more verbose.", msg);
    for(;;);
}

void x86_error_trap(const int_regs_t *regs, const char *name) {
    printf("\n\n"
           "%s:\n"
           "\tEAX: %016X\tEBX: %016X\n\tECX: %016X\tEDX: %016X\n\tESI: %016X\tEDI: %016X\n"
           "\tEIP: %016X\tESP: %016X\n\tEBP: %016X\n"
           "\tCS: %016X\tSS: %016X\n\tES: %016X\tDS: %016X\n"
           "\tFS: %016X\tGS: %016X\n"
           , name,
           regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi,
           regs->eip, regs->esp, regs->ebp,
           regs->cs, regs->ss, regs->es, regs->ds,
           regs->fs, regs->gs);
    for(;;);
}

__attribute__((naked)) void x86_trap_do_divide_by_0() {
    INT_START;x86_error_trap(regs, "divide by 0");
}

__attribute__((naked)) void x86_trap_nmi() {
    INT_START;x86_error_trap(regs, "NMI. WTF?");
}

#define DEBUGGING_TYPE_STEPPING 0
#define DEBUGGING_TYPE_ASYNC 1
#define DEBUGGING_TYPE DEBUGGING_TYPE_STEPPING

void x86_trap_breakpoint_impl(int_regs_t *regs) {
    printf("\n\n"
           "DEBUG TRAP:\n"
           "\tEAX: %016X\tEBX: %016X\n\tECX: %016X\tEDX: %016X\n\tESI: %016X\tEDI: %016X\n"
           "\tEIP: %016X\tESP: %016X\n\tEBP: %016X\n"
           "\tCS: %016X\tSS: %016X\n\tES: %016X\tDS: %016X\n"
           "\tFS: %016X\tGS: %016X\n"
#if DEBUGGING_TYPE == DEBUGGING_TYPE_STEPPING
           "Press enter to continue...\n"
#endif
           ,
           regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi,
           regs->eip, regs->esp, regs->ebp,
           regs->cs, regs->ss, regs->es, regs->ds,
           regs->fs, regs->gs
           );
#if DEBUGGING_TYPE == DEBUGGING_TYPE_STEPPING
    while (con_handle_input() != '\n');
#endif
}

__attribute__((naked)) void x86_trap_breakpoint() {
    INT_START;
    x86_trap_breakpoint_impl(regs);
    INT_END;
}

__attribute__((naked)) void x86_trap_overflow() {
    INT_START;x86_error_trap(regs, "x86 overflow?");
}

__attribute__((naked)) void x86_trap_bound() {
    INT_START;x86_error_trap(regs, "x86 bound?");
}

__attribute__((naked)) void x86_trap_invalid_opcode() {
    INT_START;x86_error_trap(regs, "invalid opcode");
}

__attribute__((naked)) void x86_trap_device_no_avaliable() {
    INT_START;x86_error_trap(regs, "device no avaliable");
}

__attribute__((naked)) void x86_trap_double_fault() {
    INT_START;x86_error_trap(regs, "double fault");
}

__attribute__((naked)) void x86_trap_coprocessor_segment_overrun() {
    INT_START;x86_error_trap(regs, "coprocessor segment overrun");
}

__attribute__((naked)) void x86_trap_invalid_tss() {
    INT_START;x86_error_trap(regs, "invalid TSS");
}

__attribute__((naked)) void x86_trap_segment_not_present() {
    INT_START;x86_error_trap(regs, "segment XXXX not present");
}

__attribute__((naked)) void x86_trap_stack_segment_fault() {
    INT_START;x86_error_trap(regs, "stack segment fault");
}

__attribute__((naked)) void x86_trap_general_protection() {
    INT_START;x86_error_trap(regs, "general protection");
}

__attribute__((naked)) void x86_trap_page_fault() {
    INT_START;x86_error_trap(regs, "page fault");
}

__attribute__((naked)) void x86_trap_x87_error() {
    INT_START;x86_error_trap(regs, "FPU error");
}

__attribute__((naked)) void x86_trap_alignment() {
    INT_START;x86_error_trap(regs, "alignment");
}

__attribute__((naked)) void x86_trap_machine() {
    INT_START;x86_error_trap(regs, "x86 machine?");
}

__attribute__((naked)) void x86_trap_SIMD() {
    INT_START;x86_error_trap(regs, "SIMD");
}

volatile uint32_t ditch = 0;
volatile bool is_tasking_avaliable = false;

void int_timer_impl(int_regs_t *regs) {
    if (is_tasking_avaliable && !tasks[current_task].yield_only) {
        shred_next_task(regs);
    }
}

__attribute__((naked))
void int_timer() {
    INT_START;
    int_timer_impl(regs);
    INT_END;
}

void syscall_handler_impl(int_regs_t *regs) {
    // TODO
    switch (regs->eax) {
    case 0: {
        is_tasking_avaliable = true;
        shred_start_tasking(regs);
    } break;
    case 1: {
        if (current_task == 0) panic("Attempt to kill init.");
        tasks[current_task].status = TASK_DEAD;
        shred_next_task(regs);
    } break;
    case 2: {
        tty_write(regs->ebx, (void *)regs->ecx, regs->edx);
    } break;
    case 3: {
        tty_read(regs->ebx, (void *)regs->ecx, regs->edx);
    } break;
    case 4: {
        shred_next_task(regs);
    } break;
    default: {
        printf("Syscall: %08X\n", regs->eax);
        printf("\n\n"
               "\tEAX: %016X\tEBX: %016X\n\tECX: %016X\tEDX: %016X\n\tESI: %016X\tEDI: %016X\n"
               "\tEIP: %016X\tESP: %016X\n\tEBP: %016X\n"
               "\tCS: %016X\tSS: %016X\n\tES: %016X\tDS: %016X\n"
               "\tFS: %016X\tGS: %016X\n",
               regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi,
               regs->eip, regs->esp, regs->ebp,
               regs->cs, regs->ss, regs->es, regs->ds,
               regs->fs, regs->gs);
        panic("Unknown syscall");
    }
    }
}

__attribute__((naked))
void syscall_handler() {
    INT_START;
    syscall_handler_impl(regs);
    INT_END;
}

__attribute__((naked))
void int_keyboard() {
    extern uint16_t vga_copies[4][80*25];
    INT_START;
    tty_char ch = con_handle_input();
    switch (ch) {
    case TTY_CC_F1: {
        current_tty_displ = 0;
        memcpy((void *)0xB8000, vga_copies[current_tty_displ], 80*25*2);
    } break;
    case TTY_CC_F2: {
        current_tty_displ = 1;
        memcpy((void *)0xB8000, vga_copies[current_tty_displ], 80*25*2);
    } break;
    case TTY_CC_F3: {
        current_tty_displ = 2;
        memcpy((void *)0xB8000, vga_copies[current_tty_displ], 80*25*2);
    } break;
    case TTY_CC_F4: {
        current_tty_displ = 3;
        memcpy((void *)0xB8000, vga_copies[current_tty_displ], 80*25*2);
    } break;
    }
    INT_END;
}

void kernel_start() {
    for (pid_t pid = 0; pid < TASKS_CAP; ++pid) {
        tasks[pid].status = TASK_DEAD;
    }
    ttys[0].write = con_putstr;
    ttys[0].read = con_read;
    ttys[0].getcur = con_getcur;
    ttys[0].setcur = con_setcur;
    ttys[0].width = 80;
    ttys[0].height = 25;
    ttys[1].write = con_putstr;
    ttys[1].read = con_read;
    ttys[1].getcur = con_getcur;
    ttys[1].setcur = con_setcur;
    ttys[1].width = 80;
    ttys[1].height = 25;
    ttys[2].write = con_putstr;
    ttys[2].read = con_read;
    ttys[2].getcur = con_getcur;
    ttys[2].setcur = con_setcur;
    ttys[2].width = 80;
    ttys[2].height = 25;
    ttys[3].write = con_putstr;
    ttys[3].read = con_read;
    ttys[3].getcur = con_getcur;
    ttys[3].setcur = con_setcur;
    ttys[3].width = 80;
    ttys[3].height = 25;
    
    gdt_init();
    idt_init();

    asm volatile ("sti\n");

    // TODO: use separated files!
    
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);

    uint16_t pit_div = 1193182 / 60;
    idt_set(32, 0x08, int_timer, IDT_GATE_32BIT_INT, 0);
    idt_set(33, 0x08, int_keyboard, IDT_GATE_32BIT_INT, 0);
    idt_set(0x80, 0x08, syscall_handler, IDT_GATE_32BIT_INT, 3);
    
    outb(0x43, 0x36);
    outb(0x40, pit_div & 0xff);
    outb(0x40, (pit_div>>8) & 0xff);
    outb(0x21, inb(0x21) & ~1);

    idt_set(0x00, 0x08, x86_trap_do_divide_by_0             , IDT_GATE_32BIT_INT, 0);
    idt_set(0x02, 0x08, x86_trap_nmi                        , IDT_GATE_32BIT_INT, 0);
    idt_set(0x03, 0x08, x86_trap_breakpoint                 , IDT_GATE_32BIT_INT, 0);
    idt_set(0x04, 0x08, x86_trap_overflow                   , IDT_GATE_32BIT_INT, 0);
    idt_set(0x05, 0x08, x86_trap_bound                      , IDT_GATE_32BIT_INT, 0);
    idt_set(0x06, 0x08, x86_trap_invalid_opcode             , IDT_GATE_32BIT_INT, 0);
    idt_set(0x07, 0x08, x86_trap_device_no_avaliable        , IDT_GATE_32BIT_INT, 0);
    idt_set(0x08, 0x08, x86_trap_double_fault               , IDT_GATE_32BIT_INT, 0);
    idt_set(0x09, 0x08, x86_trap_coprocessor_segment_overrun, IDT_GATE_32BIT_INT, 0);
    idt_set(0x0A, 0x08, x86_trap_invalid_tss                , IDT_GATE_32BIT_INT, 0);
    idt_set(0x0B, 0x08, x86_trap_segment_not_present        , IDT_GATE_32BIT_INT, 0);
    idt_set(0x0C, 0x08, x86_trap_stack_segment_fault        , IDT_GATE_32BIT_INT, 0);
    idt_set(0x0D, 0x08, x86_trap_general_protection         , IDT_GATE_32BIT_INT, 0);
    idt_set(0x0E, 0x08, x86_trap_page_fault                 , IDT_GATE_32BIT_INT, 0);
    idt_set(0x10, 0x08, x86_trap_x87_error                  , IDT_GATE_32BIT_INT, 0);
    idt_set(0x11, 0x08, x86_trap_alignment                  , IDT_GATE_32BIT_INT, 0);
    idt_set(0x12, 0x08, x86_trap_machine                    , IDT_GATE_32BIT_INT, 0);
    idt_set(0x13, 0x08, x86_trap_SIMD                       , IDT_GATE_32BIT_INT, 0);
    
    shred_make_task((word_t)a, 0, 0, 0, 0x08, 0x10, false);
    shred_make_task((word_t)b, 1, 0, 0, 0x08, 0x10, true);
    
    asm (
         "xor %eax, %eax\n"
         "int $0x80\n"
         );
    
    for (;;);
}

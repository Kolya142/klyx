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



#define __KLYX_LIB__
#include <unistd.h>
#include <klyx/i386_emulator.h>
#include <thirdparty/printf.h>
#include <klyx/console.h>
#include <klyx/kernel.h>
#include <klyx/sched.h>
#include <klyx/hw.h>
#include <klyx/mutex.h>
#include <klyx/traps.h>

int errno;

void test_a(void);
void test_b(void);
void test_c(void);

// TODO: add ring3.

volatile void panic(const char *msg, int_regs_t *regs) {
    printf("\n\n");
    for (int i = 0; i < 80; ++i) {
	printf("-");
    }
    if (regs) {
	printf("\tEAX: %016X\tEBX: %016X\n\tECX: %016X\tEDX: %016X\n\tESI: %016X\tEDI: %016X\n"
	       "\tEIP: %016X\tESP: %016X\n\tEBP: %016X\n"
	       "\tCS: %016X\tSS: %016X\n\tES: %016X\tDS: %016X\n"
	       "\tFS: %016X\tGS: %016X\n",
	       regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi,
	       regs->eip, regs->esp, regs->ebp,
	       regs->cs, regs->ss, regs->es, regs->ds,
	       regs->fs, regs->gs);
    }
    printf("\n\nKernel panic: `%s`.\n", msg);
    for(;;);
}

extern unsigned short con_cursors[4];
extern unsigned short *con_addr;
extern uint16_t vga_copies[4][80*25];

size_t con_putstr_rev(idx_t tty, const char *buf, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        char ch = buf[i];
        switch (ch) {
        case '\n': {
            con_cursors[tty] += 80-(con_cursors[tty]%80);
        } break;
        case '\r': {
            con_cursors[tty] -= (con_cursors[tty]%80);
        } break;
        case '\t': {
            con_cursors[tty] += 8-(con_cursors[tty]%80%8);
        } break;
        case '\v': {
            con_cursors[tty] += 80;
        } break;
        case '\b': {
            con_cursors[tty] -= 1;
        } break;
        default: {
	    vga_copies[tty][(79-con_cursors[tty]%80)+con_cursors[tty]/80*80] = 0x0F00 | ch;
            if (current_tty_displ == tty)
                con_addr[(79-con_cursors[tty]%80)+con_cursors[tty]/80*80] = 0x0F00 | ch;
            ++con_cursors[tty];
        }
        }
        if (con_cursors[tty] >= 80*25) {
            con_cursors[tty] = 80*24;
            memcpy((void *)vga_copies[tty], (void *)(vga_copies[tty]+80), (80*24)*2);
            memset((void *)(vga_copies[tty]+80*24), 0, 80*2);
            if (current_tty_displ == tty) {
                memcpy((void *)con_addr, (void *)(con_addr+80), (80*24)*2);
                memset((void *)(con_addr+80*24), 0, 80*2);
            }
        }
    }
    return count;
}


void kernel_start() {

    for (pid_t pid = 0; pid < TASKS_CAP; ++pid) {
        tasks[pid].status = TASK_DEAD;
    }

    memset((void *)0xB8000, 0, 25*80*2);
    
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
    ttys[3].write = con_putstr_rev;
    ttys[3].read = con_read;
    ttys[3].getcur = con_getcur;
    ttys[3].setcur = con_setcur;
    ttys[3].width = 80;
    ttys[3].height = 25;
    
    gdt_init();
    idt_init();
    
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

    asm volatile ("sti\n");

    sched_make_task((word_t)test_a, 0, 0, 0, 0x08, 0x10, false);
    sched_make_task((word_t)test_b, 1, 0, 0, 0x08, 0x10, false);
    sched_make_task((word_t)test_a, 2, 0, 0, 0x08, 0x10, false);
    sched_make_task((word_t)test_c, 3, 0, 0, 0x08, 0x10, false);
    current_task = 0;
    
    asm (
         "xor %eax, %eax\n"
         "int $0x80\n"
         );
    
    for (;;);
}

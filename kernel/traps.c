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



#include <thirdparty/printf.h>
#include <klyx/console.h>
#include <klyx/traps.h>
#include <klyx/sched.h>
#include <klyx/tty.h>


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

static volatile uint32_t ditch = 0;
static volatile bool is_tasking_avaliable = false;

void int_timer_impl(int_regs_t *regs) {
    if (is_tasking_avaliable && !tasks[current_task].yield_only) {
        sched_next_task(regs);
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
        sched_start_tasking(regs);
    } break;
    case 1: {
        if (current_task == 0) panic("Attempt to kill init.");
        tasks[current_task].status = TASK_DEAD;
        sched_next_task(regs);
    } break;
    case 2: {
	tty_write(tasks[current_task].tty, (void *)regs->ebx, regs->ecx);
    } break;
    case 3: {
        tty_read(tasks[current_task].tty, (void *)regs->ebx, regs->ecx);
    } break;
    case 4: {
        sched_next_task(regs);
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

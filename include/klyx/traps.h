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



#include <klyx/hw.h>

void x86_error_trap(const int_regs_t *regs, const char *name);

__attribute__((naked)) void x86_trap_do_divide_by_0();

__attribute__((naked)) void x86_trap_nmi();

#define DEBUGGING_TYPE_STEPPING 0
#define DEBUGGING_TYPE_ASYNC 1
#define DEBUGGING_TYPE DEBUGGING_TYPE_STEPPING

void x86_trap_breakpoint_impl(int_regs_t *regs);

__attribute__((naked)) void x86_trap_breakpoint();
__attribute__((naked)) void x86_trap_overflow();
__attribute__((naked)) void x86_trap_bound();
__attribute__((naked)) void x86_trap_invalid_opcode();
__attribute__((naked)) void x86_trap_device_no_avaliable();
__attribute__((naked)) void x86_trap_double_fault();
__attribute__((naked)) void x86_trap_coprocessor_segment_overrun();
__attribute__((naked)) void x86_trap_invalid_tss();
__attribute__((naked)) void x86_trap_segment_not_present();
__attribute__((naked)) void x86_trap_stack_segment_fault();
__attribute__((naked)) void x86_trap_general_protection();
__attribute__((naked)) void x86_trap_page_fault();
__attribute__((naked)) void x86_trap_x87_error();
__attribute__((naked)) void x86_trap_alignment();
__attribute__((naked)) void x86_trap_machine();
__attribute__((naked)) void x86_trap_SIMD();

void int_timer_impl(int_regs_t *regs);
__attribute__((naked)) void int_timer();
void syscall_handler_impl(int_regs_t *regs);
__attribute__((naked)) void syscall_handler();
__attribute__((naked)) void int_keyboard();

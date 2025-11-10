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



#pragma once
#define __KLYX_LIB__
#include <unistd.h>
#include <klyx/mutex.h>


mutex_lock_t test_lock = 0;

static _syscall_2(int, tty_write, char const *const, buf, size_t, count);

static _syscall_2(int, tty_read, char *const, buf, size_t, count);

static _syscall_0(int, exit);

static _syscall_0(int, yield);

void test_a(void) {
    for (;;) {
        tty_write("A", 1);
        yield();
    }
}

void test_b(void) {
    for (;;) {
        tty_write("B", 1);
        yield();
    }
}

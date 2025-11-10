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
#include <klyx/kernel.h>
#include <klyx/sched.h>

typedef pid_t mutex_lock_t;

static inline void mutex_lock(mutex_lock_t *const M) {
    if (!*M) {
	*M = current_task;
    }
}
static inline void mutex_unlock(mutex_lock_t *const M) {
    if (*M == current_task) {
	*M = 0;
    }
}
static inline void mutex_wait_unlock(mutex_lock_t const *const M) {
    if (tasks[current_task].yield_only)
	while (*M) asm volatile ("mov $4, %%eax;int $0x80" ::: "eax");
    else
	while (*M);
}
static inline void mutex_wait_lock(mutex_lock_t const *const M) {
    if (tasks[current_task].yield_only)
	while (!*M) asm volatile ("mov $4, %%eax;int $0x80" ::: "eax");
    else
	while (!*M);
}

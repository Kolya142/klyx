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
#include <klyx/kernel.h>
#include <signal.h>
#include <klyx/hw.h>
#include <stddef.h>

#define TASKS_CAP 256
#define TASK_STACK_CAP 8192
#define TASKS_STACK_START ((void *)0x40000000)

typedef enum task_status {
    TASK_DEAD = 0,
    TASK_NEW,
    TASK_ACTIVE,
} task_status_t;

typedef struct regs {
    word_t eax,
        ebx,
        ecx,
        edx,
        esi,
        edi,
        esp,
        ebp,
        eip,
        fs,
        gs,
        ds,
        es,
        ss,
        cs,
        eflags;
    // TODO: save x87 registers
} regs_t;

#define TASK_SIG_CAP 64

typedef struct task {
    regs_t regs;
    task_status_t status;
    idx_t tty;
    pid_t parent /*TODO: children*/;
    sig_t signals[TASK_SIG_CAP];
    void *signal_handlers[28];
    size_t signals_write_head, signals_read_head, signals_size;
    bool paused /* Is task paused by the kernel */;
    bool yield_only /* Is task currently cannot be switched by a timer tick. Not for user process. */;
    uid_t uid, euid, suid;
    gid_t gid, egid, sgid;
} task_t;

// Abnormal termination of the process.
#define SIG_T 0001
// Abnormal termination of the process with additional actions.
#define SIG_A 0002
// Ignore the signal.
#define SIG_I 0004
// Stop the process.
#define SIG_S 0010
// Continue the process, if it is stopped; otherwise, ignore the signal.
#define SIG_C 0020
// Hardware-triggered termination of the process.
#define SIG_MH 0040
// User/Application-triggered termination/pausing/continuing of the process.
#define SIG_MA 0100
// Data signal.
#define SIG_MD 0200
// Kernel called/process internal signal.
#define SIG_MI 0400


extern task_t tasks[TASKS_CAP];
extern pid_t current_task;
extern uint8_t sigtype[SIG_COUNT];

pid_t sched_make_task(word_t eip, idx_t tty, word_t fs, word_t gs, word_t cs, word_t generic_segment, bool yield_only);
void sched_next_task(int_regs_t *regs);
void sched_start_tasking(int_regs_t *regs);
void sched_kill_task(pid_t pid, sig_t sig);

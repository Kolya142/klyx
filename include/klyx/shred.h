#pragma once
#include <klyx/kernel.h>
#include <klyx/hw.h>

// TODO: rename to sched

#define TASKS_CAP 256
#define TASK_STACK_CAP 8192
#define TASKS_STACK_START ((void *)0x40000000)

typedef enum task_status {
    TASK_DEAD = 0,
    TASK_NEW,
    TASK_ACTIVE,
} task_status_t;

typedef unsigned long word_t;
typedef long pid_t;

typedef struct regs {
    word_t eax, // +0
        ebx,    // +4
        ecx,    // +8
        edx,    // +12
        esi,    // +16
        edi,    // +20
        esp,    // +24
        ebp,    // +28
        eip,    // +32
        fs,     // +36
        gs,     // +40
        generic_segment/*Not a register, but i think we're do not need separated ds, ss, es*/,
        cs,
        eflags;
    // TODO: save x87 registers
} regs_t;

typedef struct int_regs {
    word_t gs, fs, es, ss, ds, edi, esi, ebp, esp, ebx, edx, ecx, eax, eip, cs, eflags;
} int_regs_t;

typedef struct task {
    regs_t regs;
    task_status_t status;
    idx_t tty;
    pid_t parent /*TODO: children*/;
} task_t;

extern task_t tasks[TASKS_CAP];
extern pid_t current_task;

pid_t shred_make_task(word_t eip, idx_t tty, word_t fs, word_t gs, word_t cs, word_t generic_segment);
void shred_next_task(int_regs_t *regs);
void shred_start_tasking(int_regs_t *regs);
void shred_kill_task(int_regs_t *regs, pid_t pid);

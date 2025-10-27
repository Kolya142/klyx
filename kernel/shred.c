#include <klyx/shred.h>

task_t tasks[TASKS_CAP] = {0};
pid_t current_task = 0;

pid_t shred_make_task(word_t eip, idx_t tty, word_t fs, word_t gs, word_t cs, word_t generic_segment) {
    for (pid_t pid = 0; pid < TASKS_CAP; ++pid) {
        if (tasks[pid].status == TASK_DEAD) {
            memset((void *)&tasks[pid].regs, 0, sizeof(tasks[pid].regs)); // New process should start in an empty enviroment for some reasons.
            tasks[pid].regs.eip = eip;
            tasks[pid].regs.eflags = 0x206;
            tasks[pid].regs.fs = fs;
            tasks[pid].regs.gs = gs;
            tasks[pid].regs.cs = cs;
            tasks[pid].regs.generic_segment = generic_segment;
            tasks[pid].regs.esp = (word_t)(TASKS_STACK_START+TASK_STACK_CAP*pid);
            tasks[pid].regs.ebp = tasks[pid].regs.esp;
            tasks[pid].status = TASK_NEW;
            tasks[pid].tty = tty;
            tasks[pid].parent = 0; // TODO
            return pid;
        }
    }
    errno = ENOBUFS;
    return 0;
}

#include <thirdparty/printf.h>

void shred_next_task(int_regs_t *regs) {
    tasks[current_task].regs.fs              = regs->fs;
    tasks[current_task].regs.generic_segment = regs->ds;
    tasks[current_task].regs.cs              = regs->cs;
    tasks[current_task].regs.edi             = regs->edi;
    tasks[current_task].regs.esi             = regs->esi;
    tasks[current_task].regs.ebp             = regs->ebp;
    tasks[current_task].regs.esp             = regs->esp;
    tasks[current_task].regs.ebx             = regs->ebx;
    tasks[current_task].regs.edx             = regs->edx;
    tasks[current_task].regs.ecx             = regs->ecx;
    tasks[current_task].regs.eax             = regs->eax;
    tasks[current_task].regs.eflags          = regs->eflags;
    tasks[current_task].regs.eip             = regs->eip;
    for (word_t try = 0; try < TASKS_CAP - 1; ++try) {
        current_task = (current_task+1)%TASKS_CAP;
        if (tasks[current_task].status != TASK_DEAD) {
            regs->fs     = tasks[current_task].regs.fs;
            regs->es     = tasks[current_task].regs.generic_segment;
            regs->ss     = tasks[current_task].regs.generic_segment;
            regs->ds     = tasks[current_task].regs.generic_segment;
            regs->cs     = tasks[current_task].regs.cs;
            regs->edi    = tasks[current_task].regs.edi;
            regs->esi    = tasks[current_task].regs.esi;
            regs->ebp    = tasks[current_task].regs.ebp;
            regs->esp    = tasks[current_task].regs.esp;
            regs->ebx    = tasks[current_task].regs.ebx;
            regs->edx    = tasks[current_task].regs.edx;
            regs->ecx    = tasks[current_task].regs.ecx;
            regs->eax    = tasks[current_task].regs.eax;
            regs->eflags = tasks[current_task].regs.eflags;
            regs->eip    = tasks[current_task].regs.eip;
            return;
        }
    }
    printf("Kernel panic: no avaliable processes."); // TODO
    for(;;);
}

volatile void shred_start_tasking(int_regs_t *regs) {
    current_task = 0;
    if (tasks[current_task].status != TASK_DEAD) {
        regs->fs     = tasks[current_task].regs.fs;
        regs->es     = tasks[current_task].regs.generic_segment;
        regs->ss     = tasks[current_task].regs.generic_segment;
        regs->ds     = tasks[current_task].regs.generic_segment;
        regs->cs     = tasks[current_task].regs.cs;
        regs->edi    = tasks[current_task].regs.edi;
        regs->esi    = tasks[current_task].regs.esi;
        regs->ebp    = tasks[current_task].regs.ebp;
        regs->esp    = tasks[current_task].regs.esp;
        regs->ebx    = tasks[current_task].regs.ebx;
        regs->edx    = tasks[current_task].regs.edx;
        regs->ecx    = tasks[current_task].regs.ecx;
        regs->eax    = tasks[current_task].regs.eax;
        regs->eflags = tasks[current_task].regs.eflags;
        regs->eip    = tasks[current_task].regs.eip;
    }
    else {
        printf("Kernel panic: init not loaded now."); // TODO
        for(;;);
    }
}

void shred_kill_task(int_regs_t *regs, pid_t pid) {
    tasks[pid].status = TASK_DEAD; // TODO: send signal
    if (pid == current_task) {
        shred_next_task(regs);
    }
}

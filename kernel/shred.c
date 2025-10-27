#include <klyx/shred.h>

task_t tasks[TASKS_CAP] = {0};
pid_t current_task = 0;

pid_t shred_make_task(word_t eip, word_t fs, word_t gs, word_t generic_segment) {
    for (pid_t pid = 0; pid < TASKS_CAP; ++pid) {
        if (tasks[pid].status == TASK_DEAD) {
            memset((void *)&tasks[pid].regs, 0, sizeof(tasks[pid].regs)); // New process should start in an empty enviroment for some reasons.
            tasks[pid].regs.eip = eip;
            tasks[pid].regs.fs = fs;
            tasks[pid].regs.gs = gs;
            tasks[pid].regs.generic_segment = generic_segment;
            tasks[pid].regs.esp = (word_t)(TASKS_STACK_START+TASK_STACK_CAP*pid);
            tasks[pid].regs.ebp = tasks[pid].regs.esp;
            tasks[pid].status = TASK_NEW;
            tasks[pid].parent = 0; // TODO
            return pid;
        }
    }
    errno = ENOBUFS;
    return 0;
}

#include <thirdparty/printf.h>

void shred_next_task(void) {
    // TODO: implement timer-based shred instead of trust-me-bro-based.
    // TODO: use all registers.
    asm volatile("mov %0, %%eax" :: "r"(&tasks[current_task].regs) : "eax");
    asm volatile("mov %%ebx, +4(%%eax)" ::: "ebx", "eax");
    asm volatile("mov %%ecx, +8(%%eax)" ::: "ecx", "eax");
    asm volatile("mov %%edx, +12(%%eax)" ::: "edx", "eax");
    asm volatile("mov %%esi, +16(%%eax)" ::: "esi", "eax");
    asm volatile("mov %%edi, +20(%%eax)" ::: "edi", "eax");
    asm volatile("mov %%esp, +24(%%eax)" ::: "eax");
    asm volatile("mov %%ebp, +28(%%eax)" ::: "ebp", "eax");
    word_t is_back = 0;
    asm volatile("mov $0, %%eax\n"
                 "mov $._go_back, %0\n"
                 "._go_back:"
                 "mov %%eax, %1": "=r"(tasks[current_task].regs.eip), "=r"(is_back) :: "eax");
    if (is_back) {
        return;
    }
    for (word_t try = 0; try < TASKS_CAP - 1; ++try) {
        current_task = (current_task+1)%TASKS_CAP;
        if (tasks[current_task].status != TASK_DEAD) {
            asm volatile("mov %0, %%eax" :: "r"(&tasks[current_task].regs) : "eax");
            asm volatile("mov +4(%%eax), %%ebx" ::: "ebx", "eax");
            asm volatile("mov +8(%%eax), %%ecx" ::: "ecx", "eax");
            asm volatile("mov +12(%%eax), %%edx" ::: "edx", "eax");
            asm volatile("mov +16(%%eax), %%esi" ::: "esi", "eax");
            asm volatile("mov +20(%%eax), %%edi" ::: "edi", "eax");
            asm volatile("mov +24(%%eax), %%esp" ::: "eax");
            asm volatile("mov +28(%%eax), %%ebp" ::: "ebp", "eax");
            // for (;;);
            asm volatile("mov +32(%eax), %eax\n"
                         "jmp *%eax");
        }
    }
    printf("Kernel panic: no avaliable processes.");
    for(;;);
}

volatile void shred_start_tasking(void) {
    // TODO: implement timer-based shred instead of trust-me-bro-based.
    // TODO: use all registers.
    current_task = 0;
    if (tasks[current_task].status != TASK_DEAD) {
        asm volatile("mov %0, %%eax" :: "r"(&tasks[current_task].regs) : "eax");
        asm volatile("mov +4(%%eax), %%ebx" ::: "ebx", "eax");
        asm volatile("mov +8(%%eax), %%ecx" ::: "ecx", "eax");
        asm volatile("mov +12(%%eax), %%edx" ::: "edx", "eax");
        asm volatile("mov +16(%%eax), %%esi" ::: "esi", "eax");
        asm volatile("mov +20(%%eax), %%edi" ::: "edi", "eax");
        asm volatile("mov +24(%%eax), %%esp" ::: "eax");
        asm volatile("mov +28(%%eax), %%ebp" ::: "ebp", "eax");
        // for (;;);
        asm volatile("mov +32(%eax), %eax\n"
                     "jmp *%eax");
    }
    else {
        printf("Kernel panic: init isn't loaded now.");
        for(;;);
    }
}

void shred_kill_task(pid_t pid) {
    tasks[pid].status = TASK_DEAD; // TODO: send signal
    if (pid == current_task) {
        shred_next_task();
    }
}

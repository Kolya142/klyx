#include <thirdparty/printf.h>
#include <klyx/console.h>
#include <klyx/kernel.h>
#include <klyx/shred.h>
#include <klyx/hw.h>

int errno;

void a(void) {
    for (;;) {
        con_putchar('A');
        shred_next_task();
    }
}

void b(void) {
    for (;;) {
        con_putchar('B');
        shred_next_task();
    }
}

void kernel_start() {
    for (pid_t pid = 0; pid < TASKS_CAP; ++pid) {
        tasks[pid].status = TASK_DEAD;
    }
    shred_make_task((word_t)a, 0, 0, 0x10);
    shred_make_task((word_t)b, 0, 0, 0x10);
    shred_start_tasking();
    for (;;);
}

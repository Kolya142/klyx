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




#include <klyx/sched.h>

task_t tasks[TASKS_CAP] = {0};
pid_t current_task = 0;

pid_t sched_make_task(word_t eip, idx_t tty, word_t fs, word_t gs, word_t cs, word_t generic_segment, bool yield_only) {
    for (pid_t pid = 0; pid < TASKS_CAP; ++pid) {
        if (tasks[pid].status == TASK_DEAD) {
	    memset((void *)&tasks[pid], 0, sizeof(tasks[pid])); // New process must start in an empty enviroment for some boring reasons.
            tasks[pid].regs.eip = eip;
            tasks[pid].regs.eflags = 0x206;
            tasks[pid].regs.fs = fs;
            tasks[pid].regs.gs = gs;
            tasks[pid].regs.cs = cs;
            tasks[pid].regs.ds = generic_segment;
            tasks[pid].regs.es = generic_segment;
            tasks[pid].regs.ss = generic_segment;
            tasks[pid].regs.esp = (word_t)(TASKS_STACK_START+TASK_STACK_CAP*pid);
            tasks[pid].regs.ebp = tasks[pid].regs.esp;
            tasks[pid].status = TASK_NEW;
            tasks[pid].tty = tty;
            tasks[pid].yield_only = yield_only;
            tasks[pid].parent = 0; // TODO
            return pid;
        }
    }
    errno = ENOBUFS;
    return 0;
}

#include <thirdparty/printf.h>

void sched_next_task(int_regs_t *regs) {
    task_t *task = &tasks[current_task];
    
    task->regs.fs              = regs->fs;
    task->regs.gs              = regs->gs;
    task->regs.ds              = regs->ds;
    task->regs.ss              = regs->ss;
    task->regs.es              = regs->es;
    task->regs.cs              = regs->cs;
    task->regs.edi             = regs->edi;
    task->regs.esi             = regs->esi;
    task->regs.ebp             = regs->ebp;
    task->regs.esp             = regs->esp;
    task->regs.ebx             = regs->ebx;
    task->regs.edx             = regs->edx;
    task->regs.ecx             = regs->ecx;
    task->regs.eax             = regs->eax;
    task->regs.eflags          = regs->eflags;
    task->regs.eip             = regs->eip;
    
    while (task->signals_size && task->status != TASK_DEAD && !task->paused) {
        --task->signals_size;
        switch (task->signals[task->signals_read_head]) {
        case SIGABRT: {
            printf("TODO: SIGABRT\n");goto sig_die;
        } break;
        case SIGALRM: {
            printf("TODO: SIGALRM\n");goto sig_die;
        } break;
        case SIGBUS: {
            printf("TODO: SIGBUS\n");goto sig_die;
        } break;
        case SIGCHLD: {
            printf("TODO: SIGCHLD\n");goto sig_die;
        } break;
        case SIGCONT: {
            goto sig_cont;
        } break;
        case SIGFPE: {
            printf("Floating point exception(Process finished)\n");goto sig_die;
        } break;
        case SIGHUP: {
            printf("TODO: SIGHUP\n");goto sig_die;
        } break;
        case SIGILL: {
            printf("Illegal instruction(Process finished)\n");goto sig_die;
        } break;
        case SIGINT: {
            printf("TODO: SIGINT\n");goto sig_die;
        } break;
        case SIGKILL: {
            goto sig_die;
        } break;
        case SIGPIPE: {
            printf("TODO: SIGPIPE\n");goto sig_die;
        } break;
        case SIGQUIT: {
            printf("TODO: SIGQUIT\n");goto sig_die;
        } break;
        case SIGSEGV: {
            printf("Segmentation fault(Process finished)\n");goto sig_die;
        } break;
        case SIGSTOP: {
            goto sig_stop;
        } break;
        case SIGTERM: {
            printf("TODO: SIGTERM\n");goto sig_die;
        } break;
        case SIGTSTP: {
            printf("TODO: SIGTSTP\n");goto sig_die;
        } break;
        case SIGTTIN: {
            printf("TODO: SIGTTIN\n");goto sig_die;
        } break;
        case SIGTTOU: {
            printf("TODO: SIGTTOU\n");goto sig_die;
        } break;
        case SIGUSR1: {
            printf("TODO: SIGUSR1\n");goto sig_die;
        } break;
        case SIGUSR2: {
            printf("TODO: SIGUSR2\n");goto sig_die;
        } break;
        case SIGPOLL: {
            printf("TODO: SIGPOLL\n");goto sig_die;
        } break;
        case SIGPROF: {
            printf("TODO: SIGPROF\n");goto sig_die;
        } break;
        case SIGSYS: {
            printf("Invalid syscall(Process finished)\n");goto sig_die;
        } break;
        case SIGTRAP: {
            printf("TODO: SIGTRAP\n");goto sig_die;
        } break;
        case SIGURG: {
            printf("TODO: SIGURG\n");goto sig_die;
        } break;
        case SIGVTALRM: {
            printf("TODO: SIGVTALRM\n");goto sig_die;
        } break;
        case SIGXCPU: {
            printf("TODO: SIGXCPU\n");goto sig_die;
        } break;
        case SIGXFSZ: {
            printf("TODO: SIGXFSZ\n");goto sig_die;
        } break;
        }
	goto sig_end;
      sig_die:
	if (!current_task) panic("Attempt to kill init.", regs);
	task->status = TASK_DEAD;
	goto sig_end;
      sig_cont:
	task->paused = false;
	goto sig_end;
      sig_stop:
	task->paused = true;
	goto sig_end;
      sig_end:
        task->signals_read_head = (task->signals_read_head+1)%TASK_SIG_CAP;
    }
    
    for (word_t try = 0; try < TASKS_CAP; ++try) {
        current_task = (current_task+1)%TASKS_CAP;
        
        if (tasks[current_task].status != TASK_DEAD && !tasks[current_task].paused) {
            regs->fs     = tasks[current_task].regs.fs;
            regs->gs     = tasks[current_task].regs.gs;
            regs->es     = tasks[current_task].regs.es;
            regs->ss     = tasks[current_task].regs.ss;
            regs->ds     = tasks[current_task].regs.ds;
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
    panic("Kernel panic: no avaliable processes.", regs);
}

volatile void sched_start_tasking(int_regs_t *regs) {
    current_task = 0;
    if (tasks[current_task].status != TASK_DEAD) {
        regs->fs     = tasks[current_task].regs.fs;
        regs->gs     = tasks[current_task].regs.gs;
        regs->es     = tasks[current_task].regs.es;
        regs->ss     = tasks[current_task].regs.ss;
        regs->ds     = tasks[current_task].regs.ds;
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
	panic("Kernel panic: init is not loaded.", regs);
    }
}
void sched_kill_task(pid_t pid, sig_t sig) {
    task_t *task = &tasks[pid];
    if (task->signals_size >= TASK_SIG_CAP-1) return;
    ++task->signals_size;
    task->signals[task->signals_write_head] = sig;
    task->signals_write_head = (task->signals_write_head+1)%TASK_SIG_CAP;
}

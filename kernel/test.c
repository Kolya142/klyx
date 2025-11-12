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
#define __NO_POSIX_FNDF
#include <unistd.h>
#include <string.h>
#include <klyx/mutex.h>
#include <sys/utsname.h>
#include <thirdparty/printf.h>


mutex_lock_t test_lock = 0;

static char *crypt(const char *a, const char *b) {
    static char enc[8193];
    if (!b || !a || !*a || !b[0] || !b[1]) return NULL;
    const char *bi = b;
    int i = 0;
    while (*a) {
	enc[i++] = (~(*a|*bi))|(*a&(~*bi));
	++a;
	++bi;
	if (!*bi) {
	    bi = b;
	}
    }
    enc[i] = 0;
    return enc;
}

static _syscall_2(int, tty_write, char const *, buf, size_t, count);

static _syscall_2(int, tty_read, char *, buf, size_t, count);

static _syscall_1(int, uname, struct utsname *, buf);

static time_t time(time_t *buf) {
    time_t __result;		
    asm volatile (		
	_syscall_call	
	:: "a"(_SCN_time),
	 _syscall_arg0(&__result)	
	);
    if (buf) {
	memcpy(&buf, &__result, sizeof(time_t));
    }
    return __result;
}

static _syscall_0(int, exit);

static _syscall_0(int, yield);

static _syscall_1(int, setuid, uid_t, uid);

static _syscall_1(int, setgid, gid_t, gid);

static _syscall_0(uid_t, getuid);

static _syscall_0(gid_t, getgid);

static _syscall_0(pid_t, getpid);

static _syscall_2(int, kill, pid_t, pid, sig_t, sig);

#define RAISE_UD2 asm("ud2")

void test_init(void);
void test_a(void);
void test_b(void);
void test_c(void);

void test_init(void) {
    for (;;) {
	// Stupid and unsafe but i still have no fork().
	if (kill(2, 0) != 0) {
	    sched_make_task((word_t)test_a, 0, 0, 0, 0x08, 0x10, false);
	}
	if (kill(4, 0) != 0) {
	    sched_make_task((word_t)test_a, 2, 0, 0, 0x08, 0x10, false);
	}
	if (kill(5, 0) != 0) {
	    sched_make_task((word_t)test_c, 3, 0, 0, 0x08, 0x10, false);
	}
	yield();
    }
}

static void login(void) {
    char buf[512];
    if (getuid() || getgid()) {
	printf("login: login must be runned as root\n");
	return;
    }

    for (;;) {
	memset(buf, 0, 512);
	printf("You need to login first.\n"
	       "Avaliable accounts: root alice bob eva.\n"
	       "Login: ");
	tty_read(buf, 511);
	if (!strcmp(buf, "root")) {
	    break; // Already root
	}
	if (!strcmp(buf, "alice")) {
	    setuid(1001);
	    setgid(1001);
	    break;
	}
	if (!strcmp(buf, "bob")) {
	    setuid(1002);
	    setgid(1002);
	    break;
	}
	if (!strcmp(buf, "eva")) {
	    setuid(1000);
	    setgid(1000);
	    break;
	}
    }
}

static void shell(void) {
    char buf[512];
    
    for (;;) {
	memset(buf, 0, 512);
	printf("$ ");
	tty_read(buf, 511);
	if (!*buf) {
	}
	else if (!strcmp(buf, "help")) {
	    printf("List of commands:\n"
		   "\thelp ud exit uname time\n"
		   "\tfib enc strcpy kdbg\n"
		   "\tkill stop cont pid\n"
		   "\tuid gid su\n"
		   "Kernel info:\n"
		   "\tF1-F4 - switch to TTY\n"
		   "\tSource code: https://github.com/Kolya142/klyx\n"
		   "\tLicense: GNU GPL2.0\n");
	}
	else if (!strcmp(buf, "ud")) {
	    RAISE_UD2;
	}
	else if (!strcmp(buf, "exit")) {
	    return;
	}
	else if (!strcmp(buf, "kill")) {
	    kill(3, SIGKILL);
	    printf("check tty2\n");
	}
	else if (!strcmp(buf, "stop")) {
	    kill(3, SIGSTOP);
	    printf("check tty2\n");
	}
	else if (!strcmp(buf, "cont")) {
	    kill(3, SIGCONT);
	    printf("check tty2\n");
	}
	else if (!strcmp(buf, "uid")) {
	    printf("UID: %d\n", getuid());
	}
	else if (!strcmp(buf, "gid")) {
	    printf("GID: %d\n", getgid());
	}
	else if (!strcmp(buf, "pid")) {
	    printf("PID: %d\n", getpid());
	}
	else if (!strcmp(buf, "uname")) {
	    struct utsname osname;
	    uname(&osname);
	    printf("Kernel version: %s\n", osname.version);
	}
	else if (!strcmp(buf, "fib")) {
	    int a = 1, b = 1, c;
	    while (a < 100) {
		printf("%d\n", a);
		c = a + b;
		b = a;
		a = c;
	    }
	}
	else if (!strcmp(buf, "time")) {
	    printf("%d\n", time(NULL));
	}
	else if (!strcmp(buf, "enc")) {
	    static char pass_saver[8192] =
#include <pass_saver.inc>
		;
	    
	    char *cr;
	    memset(buf, 0, 512);
	    printf("Enter a password: ");
	    tty_read(buf, 511);
	    if (!*buf) goto cend;
	    static char encp[8192];
	    printf("crypt() work. please wait...\n");
	    cr = crypt(pass_saver, buf);
	    if (!cr) goto cend;
	    printf("%p\n", encp);
	    strcpy(encp, cr);
	    encp[strlen(buf)] = 0;
	    memset(buf, 0, 512);
	    printf("Enter the same password: ");
	    tty_read(buf, 511);
	    if (!*buf) goto cend;
	    cr = crypt(buf, encp);
	    if (!cr) goto cend;
	    strcpy(encp, cr);
	    printf("crypt() work. please wait...\n");
	    int plen = strlen(buf);
	    printf("%d.\n", plen);
	    printf("Passwords are %sthe same.\n", strcmp(buf, encp) ? "not " : "");
	}
	else if (!strcmp(buf, "strcpy")) {
	    strcpy((char *)0xB8000, "Y o u   c a n   p r i n t   t e x t   u s i n g   s t r c p y ! ");
	}
	else if (!strcmp(buf, "kdbg")) {
	    INT3;
	}
	else if (!strcmp(buf, "su")) {
	    // Stupid but i still have no suid/sgid, fork and file system
	    uid_t old_uid = tasks[current_task].uid;
	    uid_t old_gid = tasks[current_task].gid;
	    tasks[current_task].uid = 0;
	    tasks[current_task].gid = 0;
	    shell();
	    tasks[current_task].uid = old_uid;
	    tasks[current_task].gid = old_gid;
	}
	else {
	    printf("Unknown command: `%s`\n", buf);
	}
      cend:
	printf("\n");
    }
}

void test_a(void) {
    login();
    shell();
    exit();
}

void test_b(void) {
    for (;;) {
        tty_write("B", 1);
        yield();
    }
}

void test_c(void) {
    printf("Force of inverse!\n!esrevni fo ecroF\n");
    test_a();
}

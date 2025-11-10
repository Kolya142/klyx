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
#include <stddef.h>
// I know about POSIX only that it very hard to implement... but any OS that doesn't implement POSIX just unusable.
#define _POSIX_VERSION 198808L
// TODO: understand and define _POSIX2_VERSION.
#define STDIN_FILENO 0
#define STDOUT_FILENO 0
#define STDERR_FILENO 0
#define NULL ((void *)0)

#define _USEVAR(X) ((void)X)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern int errno;
#define _Noreturn
#define _Kernel
#define _User

#ifdef __KLYX_LIB__
#define __STR_CONCAT(x,y) x ## y
#define __STRING(x) #x
#define __STR1(x) #x
#define __STR(x) __STR1(x)
// We're not using C++
#define __THROW(x)

// Thanks https://www.felixcloutier.com/documents/gcc-asm.html#examples.
#define _syscall_no "a"
#define _syscall_arg0 "b"
#define _syscall_arg1 "c"
#define _syscall_arg2 "d"
#define _syscall_arg3 "S"
#define _syscall_arg4 "D" 
#define _syscall_call "int $0x80"

#define _SCN_setup 0
#define _SCN_exit 1
#define _SCN_tty_write 2
#define _SCN_tty_read 3
#define _SCN_yield 4

#define _syscall_0(r,n)				\
    r n(void) {					\
	r __result;				\
	asm volatile (				\
	    _syscall_call			\
	    : "=a"(__result)			\
	    : "0"(_SCN_##n)			\
	    );					\
    }

#define _syscall_1(r,n,at,an)			\
    r n(at an) {				\
	r __result;				\
	asm volatile (				\
	    _syscall_call			\
	    : "=a"(__result)			\
	    : "0"(_SCN_##n),			\
	      _syscall_arg0(an)			\
	    );					\
    }

#define _syscall_2(r,n,at,an,bt,bn)		\
    r n(at an,bt bn) {				\
	r __result;				\
	asm volatile (				\
	    _syscall_call			\
	    : "=a"(__result)			\
	    : "0"(_SCN_##n),			\
	      _syscall_arg0(an),		\
	      _syscall_arg1(bn)			\
	    );					\
    }

#define _syscall_3(r,n,at,an,bt,bn,ct,cn)	\
    r n(at an,bt bn,ct cn) {			\
	r __result;				\
	asm volatile (				\
	    _syscall_call			\
	    : "=a"(__result)			\
	    : "0"(_SCN_##n),			\
	      _syscall_arg0(an),		\
	      _syscall_arg1(bn),		\
	      _syscall_arg2(cn)			\
	    );					\
    }

#define _syscall_4(r,n,at,an,bt,bn,ct,cn,dt,dn)	\
    r n(at an,bt bn,ct cn,dt dn) {		\
	r __result;				\
	asm volatile (				\
	    _syscall_call			\
	    : "=a"(__result)			\
	    : "0"(_SCN_##n),			\
	      _syscall_arg0(an),		\
	      _syscall_arg1(bn),		\
	      _syscall_arg2(cn),		\
	      _syscall_arg3(dn)			\
	    );					\
    }

#define _syscall_5(r,n,at,an,bt,bn,ct,cn,dt,dn,et,en)	\
    r n(at an,bt bn,ct cn,dt dn,et en) {		\
	r __result;					\
	asm volatile (					\
	    _syscall_call				\
	    : "=a"(__result)				\
	    : "0"(_SCN_##n),				\
	      _syscall_arg0(an),			\
	      _syscall_arg1(bn),			\
	      _syscall_arg2(cn),			\
	      _syscall_arg3(dn),			\
	      _syscall_arg4(en)				\
	    );						\
    }

#endif // __KLYX_LIB__

#ifndef __NO_POSIX_FNDF
int          access(const char *, int);
unsigned     alarm(unsigned);
int          chdir(const char *);
int          chown(const char *, uid_t, gid_t);
int          close(int);
size_t       confstr(int, char *, size_t);
char        *crypt(const char *, const char *);
int          dup(int);


int          dup2(int, int);
int          dup3(int, int, int);
_Noreturn void
             _exit(int);
// void         encrypt(char [64], int); // No idea WTF is this
int          execl(const char *, const char *, ...);
int          execle(const char *, const char *, ...);
int          execlp(const char *, const char *, ...);
int          execv(const char *, char *const []);
int          execve(const char *, char *const [], char *const []);
int          execvp(const char *, char *const []);
int          faccessat(int, const char *, int, int);
int          fchdir(int);
int          fchown(int, uid_t, gid_t);
int          fchownat(int, const char *, uid_t, gid_t, int);
int          fdatasync(int);
int          fexecve(int, char *const [], char *const []);
pid_t        _Fork(void);
pid_t        fork(void);
long         fpathconf(int, int);
int          fsync(int);
int          ftruncate(int, off_t);
char        *getcwd(char *, size_t);
gid_t        getegid(void);
int          getentropy(void *, size_t);
uid_t        geteuid(void);
gid_t        getgid(void);
int          getgroups(int, gid_t []);
long         gethostid(void);
int          gethostname(char *, size_t);
char        *getlogin(void);
int          getlogin_r(char *, size_t);
int          getopt(int, char * const [], const char *);
pid_t        getpgid(pid_t);
pid_t        getpgrp(void);
pid_t        getpid(void);
pid_t        getppid(void);
int          getresgid(gid_t *restrict, gid_t *restrict,
                 gid_t *restrict);
int          getresuid(uid_t *restrict, uid_t *restrict,
                 uid_t *restrict);
pid_t        getsid(pid_t);
uid_t        getuid(void);
int          isatty(int);
int          lchown(const char *, uid_t, gid_t);
int          link(const char *, const char *);
int          linkat(int, const char *, int, const char *, int);
int          lockf(int, int, off_t);
off_t        lseek(int, off_t, int);
int          nice(int);
long         pathconf(const char *, int);
int          pause(void);
int          pipe(int [2]);
int          pipe2(int [2], int);
int          posix_close(int, int);
ssize_t      pread(int, void *, size_t, off_t);
ssize_t      pwrite(int, const void *, size_t, off_t);
ssize_t      read(int, void *, size_t);
ssize_t      readlink(const char *restrict, char *restrict, size_t);
ssize_t      readlinkat(int, const char *restrict, char *restrict,
                 size_t);
int          rmdir(const char *);
int          setegid(gid_t);
int          seteuid(uid_t);
int          setgid(gid_t);


int          setpgid(pid_t, pid_t);
int          setregid(gid_t, gid_t);
int          setresgid(gid_t, gid_t, gid_t);
int          setresuid(uid_t, uid_t, uid_t);
int          setreuid(uid_t, uid_t);
pid_t        setsid(void);
int          setuid(uid_t);
unsigned     sleep(unsigned);
void         swab(const void *restrict, void *restrict, ssize_t);
int          symlink(const char *, const char *);
int          symlinkat(const char *, int, const char *);
void         sync(void);
long         sysconf(int);
pid_t        tcgetpgrp(int);
int          tcsetpgrp(int, pid_t);
int          truncate(const char *, off_t);
char        *ttyname(int);
int          ttyname_r(int, char *, size_t);
int          unlink(const char *);
int          unlinkat(int, const char *, int);
ssize_t      write(int, const void *, size_t);

#endif // __NO_POSIX_FNDF

extern char  *optarg;
extern int    opterr, optind, optopt;

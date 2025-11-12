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

// Thanks me for tools/enum.bc

#define SIGNOPE   0
#define SIGABRT   1 
#define SIGALRM   2 
#define SIGBUS    3 
#define SIGCHLD   4 
#define SIGCONT   5 
#define SIGFPE    6 
#define SIGHUP    7 
#define SIGILL    8 
#define SIGINT    9 
#define SIGKILL   10
#define SIGPIPE   11
#define SIGQUIT   12
#define SIGSEGV   13
#define SIGSTOP   14
#define SIGTERM   15
#define SIGTSTP   16
#define SIGTTIN   17
#define SIGTTOU   18
#define SIGUSR1   19
#define SIGUSR2   20
#define SIGPOLL   21
#define SIGPROF   22
#define SIGSYS    23
#define SIGTRAP   24
#define SIGURG    25
#define SIGVTALRM 26
#define SIGXCPU   27
#define SIGXFSZ   28
#ifdef __KLYX_LIB__
#define SIG_COUNT 29
#endif __KLYX_LIB__

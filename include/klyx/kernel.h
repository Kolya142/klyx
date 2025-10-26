#pragma once

#define NULL ((void *)0)
#define NONE 0
#define FALSE 0
#define TRUE 1

#define EOK 0
#define EAGAIN 1
#define EBUSY 2
#define EEMPTY 3
#define ENOSPACE 4

extern int errno;
void kernel_start();
void _start();

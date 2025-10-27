#pragma once
#include <stdint.h>
#include <stddef.h>
#include <errno.h>

#define NULL ((void *)0)
#define NONE 0
#define FALSE 0
#define TRUE 1

extern int errno;
void kernel_start();
void _start();

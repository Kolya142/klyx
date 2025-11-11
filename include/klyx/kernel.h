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
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <klyx/hw.h>

#define KLYX_VERSION_MAJOR 0
#define KLYX_VERSION_MINOR 0
#define KLYX_VERSION_PATCH 1
#define KLYX_VERSION_STR "" __STR(KLYX_VERSION_MAJOR) "." __STR(KLYX_VERSION_MINOR) "." __STR(KLYX_VERSION_PATCH)

extern int errno;
volatile void panic(const char *msg, int_regs_t *regs);
void kernel_start();
void _start();

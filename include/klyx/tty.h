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
#include <klyx/kernel.h>

typedef int tty_char;

typedef struct tty {
    size_t(*write)(idx_t, const char *, size_t);
    void(*setcur)(idx_t, int);
    uint32_t(*getcur)(idx_t);
    enum tty_ck(*getck)(idx_t);
    tty_char(*read)(idx_t);
    uint32_t(*getsc)(idx_t);
    int width, height;
    // TODO: list of signals
} tty_t;

enum {
    TTY_CC_EOF   = -1,
    TTY_CC_NOTCC   = -2,
    TTY_CC_LSHIFT  = -3,
    TTY_CC_RSHIFT  = -4,
    TTY_CC_LCTRL   = -5,
    TTY_CC_RCTRL   = -6,
    TTY_CC_LALT    = -7,
    TTY_CC_RALT    = -8,
    TTY_CC_CAPS    = -9,
    TTY_CC_NUM     = -10,
    TTY_CC_SCROLL  = -11,
    TTY_CC_F1      = -12,
    TTY_CC_F2      = -13,
    TTY_CC_F3      = -14,
    TTY_CC_F4      = -15,
    TTY_CC_F5      = -16,
    TTY_CC_F6      = -17,
    TTY_CC_F7      = -18,
    TTY_CC_F8      = -19,
    TTY_CC_F9      = -20,
    TTY_CC_F10     = -21,
    TTY_CC_F11     = -22,
    TTY_CC_F12     = -23,
};

extern tty_t ttys[];

extern idx_t current_tty_displ;

size_t tty_write(idx_t td, const char *buf, size_t count);
size_t tty_read(idx_t td, char *buf, size_t count);
uint32_t tty_getcur(idx_t td);
void tty_setcur(idx_t td, uint32_t cur);

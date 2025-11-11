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



#include <thirdparty/printf.h>
#include <klyx/sched.h>
#include <klyx/tty.h>
#define TTYS_CAP 4

tty_t ttys[TTYS_CAP];

idx_t current_tty_displ;

size_t tty_write(idx_t td, const char *buf, size_t count) {
    if (td >= TTYS_CAP) {
        errno = EFAULT;
        return 0;
    }
    if (ttys[td].write == NULL) {
        errno = ENOSYS;
        return 0;
    }
    errno = ENO;
    return ttys[td].write(td, buf, count);
}

size_t tty_read(idx_t td, char *buf, size_t count) {
    if (td >= TTYS_CAP) {
        errno = EFAULT;
        return 0;
    }
    if (ttys[td].read == NULL) {
        errno = ENOSYS;
        return 0;
    }
    size_t text_cur = 0;
    int end_of_loop = 0;
    for(;;) {
        errno = ENO;
        tty_char res = ttys[td].read(td);
        if (errno == EAGAIN) continue;
        if (res < 0) continue;
        switch (res) {
        case '\n':
            ttys[td].write(td, (char[]) {res}, 1);
            /* fall-through */
        case TTY_CC_EOF:
            end_of_loop = 1;
            break;
        case 0:
            break;
        case '\b':
            if (text_cur!=0) {
                buf[--text_cur] = 0;
                ttys[td].write(td, "\b \b", 3);
            }
            break;
        default:
            if (text_cur<count) {
                buf[text_cur++] = res;
                ttys[td].write(td, (char[]) {res}, 1);
            }
            break;
        }
        if (end_of_loop) break;
    }
    return text_cur;
}

uint32_t tty_getcur(idx_t td) {
    if (td >= TTYS_CAP) {
        errno = EFAULT;
        return 0;
    }
    if (ttys[td].read == NULL) {
        errno = ENOSYS;
        return 0;
    }
    return ttys[td].getcur(td);
}

void tty_setcur(idx_t td, uint32_t cur) {
    if (td >= TTYS_CAP) {
        errno = EFAULT;
        return;
    }
    if (ttys[td].read == NULL) {
        errno = ENOSYS;
        return;
    }
    ttys[td].setcur(td, cur);
}

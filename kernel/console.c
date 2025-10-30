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
#include <klyx/console.h>
#include <klyx/shred.h>
#include <stdbool.h>

// so let's for now assume that ESC=EOF.

char scan_code_table_default[128] =
    "\0"
    "\e"
    "1234567890"
    "-="
    "\b\t"
    "qwertyuiop"
    "[]"
    "\n"
    "\0"
    "asdfghjkl"
    ";"
    "'"
    "`"
    "\0"
    "\\"
    "zxcvbnm"
    ",."
    "/"
    "\0"
    "*"
    "\0"
    " "
    "\0"
    "\0\0\0\0\0\0\0\0\0\0"
    "\0\0"
    "7894561230 ."
    "\0\0";

// Cringe
tty_char scan_code_table_special[128] = {
    0,
    TTY_CC_EOF,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    TTY_CC_F1,
    TTY_CC_F2,
    TTY_CC_F3,
    TTY_CC_F4,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

char keys_state[256] = {0};

#define INPUT_BUF_CAP 1024

static unsigned char input_scancode_buf[4][INPUT_BUF_CAP];
static size_t input_scancode_buf_write_head[4] = {0};
static size_t input_scancode_buf_read_head[4] = {0};
static size_t input_scancode_buf_size[4] = {0};

static unsigned char pop_scancode() {
    if (input_scancode_buf_size[current_tty_displ] == 0) {
        errno = EAGAIN;
        return 0;
    }
    --input_scancode_buf_size[current_tty_displ];
    unsigned char c = input_scancode_buf[current_tty_displ][input_scancode_buf_read_head[current_tty_displ]];
    input_scancode_buf_read_head[current_tty_displ] = (input_scancode_buf_read_head[current_tty_displ]+1)%INPUT_BUF_CAP;
    return c;
}

static bool push_scancode(unsigned char ch) {
    if (input_scancode_buf_size[current_tty_displ] == INPUT_BUF_CAP-1) {
        return false;
    }
    ++input_scancode_buf_size[current_tty_displ];
    input_scancode_buf[current_tty_displ][input_scancode_buf_write_head[current_tty_displ]] = ch;
    input_scancode_buf_write_head[current_tty_displ] = (input_scancode_buf_write_head[current_tty_displ]+1)%INPUT_BUF_CAP;
    return true;
}

tty_char con_read(idx_t tty) {
    if (tty != current_tty_displ) {
        return TTY_CC_EOF;
    }
    unsigned char sc = pop_scancode();
    return scan_code_table_special[sc] ? scan_code_table_special[sc] : scan_code_table_default[sc];
}

tty_char con_handle_input() {
    if (!(inb(0x64)&1)) {
        return 0;
    }
    unsigned char c = inb(0x60);
    if (c&0x80) {
        keys_state[c&0x7F] = false;
        return 0;
    }
    if (keys_state[c]) {
        return 0;
    }
    keys_state[c] = true;
    push_scancode(c);
    return scan_code_table_special[c] ? scan_code_table_special[c] : scan_code_table_default[c];
}

unsigned short con_cursors[4] = {0};
unsigned short *con_addr = (void *)0xB8000;

void con_setcur(idx_t tty, int cur) {
    if (cur>=80*25) return;
    con_cursors[tty] = cur;
}

uint32_t con_getcur(idx_t tty) {
    return con_cursors[tty];
}

uint16_t vga_copies[4][80*25] = {0};

size_t con_putstr(idx_t tty, const char *buf, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        char ch = buf[i];
        switch (ch) {
        case '\n': {
            con_cursors[tty] += 80-(con_cursors[tty]%80);
        } break;
        case '\r': {
            con_cursors[tty] -= (con_cursors[tty]%80);
        } break;
        case '\t': {
            con_cursors[tty] += 8-(con_cursors[tty]%80%8);
        } break;
        case '\v': {
            con_cursors[tty] += 80;
        } break;
        case '\b': {
            con_cursors[tty] -= 1;
        } break;
        default: {
            vga_copies[tty][con_cursors[tty]] = 0x0F00 | ch;
            if (current_tty_displ == tty)
                con_addr[con_cursors[tty]] = 0x0F00 | ch;
            ++con_cursors[tty];
        }
        }
        if (con_cursors[tty] >= 80*25) {
            con_cursors[tty] = 80*24;
            memcpy((void *)vga_copies[tty], (void *)(vga_copies[tty]+80), (80*24)*2);
            memset((void *)(vga_copies[tty]+80*24), 0, 80*2);
            if (current_tty_displ == tty) {
                memcpy((void *)con_addr, (void *)(con_addr+80), (80*24)*2);
                memset((void *)(con_addr+80*24), 0, 80*2);
            }
        }
    }
    return count;
}

void con_putchar(char ch) {
    tty_write(tasks[current_task].tty, (char[]) {ch}, 1);
}

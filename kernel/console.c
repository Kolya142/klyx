#include <klyx/console.h>

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

char keys_state[256] = {0};

char con_read() {
    if (!(inb(0x64)&1)) {
        errno = EEMPTY;
        return 0;
    }
    unsigned char c = inb(0x60);
    if (c&0x80) {
        c = scan_code_table_default[c&0x7F];
        keys_state[c] = 0;
        errno = EEMPTY;
        return 0;
    }
    c = scan_code_table_default[c&0x7F];
    if (keys_state[c]) {
        errno = EEMPTY;
        return 0;
    }
    keys_state[c] = !(c&0x80);
    errno = EOK;
    return c;
}

static unsigned short con_cursor = 0;
static unsigned short *con_addr = (void *)0xB8000;

void con_putchar(char ch) {
    switch (ch) {
    case '\n': {
        con_cursor += 80-(con_cursor%80);
    } break;
    case '\r': {
        con_cursor -= (con_cursor%80);
    } break;
    case '\t': {
        con_cursor += 8-(con_cursor%80%8);
    } break;
    case '\v': {
        con_cursor += 80;
    } break;
    case '\b': {
        con_cursor -= 1;
    } break;
    default: {
        con_addr[con_cursor] = 0x0F00 | ch;
        ++con_cursor;
    }
    }
    if (con_cursor >= 80*25) {
        con_cursor = 80*24;
        memcpy((void *)con_addr, (void *)(con_addr+80), (80*24)*2);
        memset((void *)(con_addr+80*24), 0, 80*2);
    }
}

void con_setcur(unsigned short cur) {
    if (cur>=80*25) return;
    con_cursor = cur;
}

unsigned short con_getcur() {
    return con_cursor;
}

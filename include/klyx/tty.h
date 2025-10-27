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

#define TTY_CHAR_EOF -1

typedef enum tty_ck {
    TTY_CC_NOTCC,
    TTY_CC_LSHIFT,
    TTY_CC_RSHIFT,
    TTY_CC_LCTRL,
    TTY_CC_RCTRL,
    TTY_CC_LALT,
    TTY_CC_RALT,
    TTY_CC_CAPS,
    TTY_CC_NUM,
    TTY_CC_SCROLL,
    TTY_CC_F1,
    TTY_CC_F2,
    TTY_CC_F3,
    TTY_CC_F4,
    TTY_CC_F5,
    TTY_CC_F6,
    TTY_CC_F7,
    TTY_CC_F8,
    TTY_CC_F9,
    TTY_CC_F10,
    TTY_CC_F11,
    TTY_CC_F12,
} tty_ck_t;

extern tty_t ttys[];

extern idx_t current_tty_displ;
extern idx_t current_tty;

size_t tty_write(idx_t td, const char *buf, size_t count);
size_t tty_read(idx_t td, char *buf, size_t count);
uint32_t tty_getcur(idx_t td);
void tty_setcur(idx_t td, uint32_t cur);

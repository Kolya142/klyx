#pragma once
#include <klyx/kernel.h>
#include <klyx/tty.h>
#include <klyx/hw.h>


extern char scan_code_table_default[128];

extern char keys_state[256];

tty_char con_read();

void con_putchar(char ch);

void con_setcur(idx_t tty, int cur);

uint32_t con_getcur(idx_t tty);

size_t con_putstr(idx_t tty, const char *buf, size_t count);

void con_handle_input();

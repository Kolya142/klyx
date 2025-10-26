#pragma once
#include <klyx/kernel.h>
#include <klyx/hw.h>


extern char scan_code_table_default[128];

extern char keys_state[256];

char con_read();

void con_putchar(char ch);

void con_setcur(unsigned short cur);

unsigned short con_getcur();

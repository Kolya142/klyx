#pragma once
void memcpy(unsigned char *dest, unsigned char *src, unsigned long count);
void memset(unsigned char *dest, unsigned char c, unsigned long count);

void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
void outw(unsigned short port, unsigned short data);
unsigned short inw(unsigned short port);
void outd(unsigned short port, unsigned int data);
unsigned int ind(unsigned short port);

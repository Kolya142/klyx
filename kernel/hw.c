#include <klyx/kernel.h>
#include <klyx/hw.h>


void memcpy(unsigned char *dest, unsigned char *src, unsigned long count) {
    for (unsigned long i = 0; i < count; ++i) {
        *dest++ = *src++;
    }
}

void memset(unsigned char *dest, unsigned char c, unsigned long count) {
    for (unsigned long i = 0; i < count; ++i) {
        *dest++ = c;
    }
}

void outb(unsigned short port, unsigned char data) {
    __asm__ __volatile__("outb %0, %1" : : "a"(data), "Nd"(port));
}
unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
void outw(unsigned short port, unsigned short data) {
    __asm__ __volatile__("outw %0, %1" : : "a"(data), "Nd"(port));
}
unsigned short inw(unsigned short port) {
    unsigned short ret;
    __asm__ __volatile__("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
void outd(unsigned short port, unsigned int data) {
    __asm__ __volatile__("out %0, %1" : : "a"(data), "Nd"(port));
}
unsigned int ind(unsigned short port) {
    unsigned int ret;
    __asm__ __volatile__("in %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

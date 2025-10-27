#include <klyx/kernel.h>
#include <thirdparty/printf.h>
#include <klyx/hw.h>


void memcpy(void *dest, void *src, unsigned long count) {
    asm(
        "cld\n"
        "rep\n"
        "movsb" :: "c"(count), "S"(src), "D"(dest));
}

void memset(void *dest, unsigned char c, unsigned long count) {
    asm(
        "cld\n"
        "rep\n"
        "stosb" :: "c"(count), "a"(c), "D"(dest));
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

uint32_t tss[26];

#define IDT_CAP 256
#define GDT_CAP 128
__attribute__((aligned(0x10)))
idt_desc_t idt_descriptors[IDT_CAP];
idt_reg_t idtr;
__attribute__((aligned(0x10)))
gdt_desc_t gdt_descriptors[GDT_CAP];
gdt_reg_t gdtr;

__attribute__((naked))
void idt_nothing() {
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
    asm volatile ("iret");
}

void idt_init() {
    idtr.size = sizeof(idt_descriptors)-1;
    idtr.descriptors = (uint32_t)idt_descriptors;

    for (int i = 0; i < IDT_CAP; ++i) {
        idt_set(i, 0x08, idt_nothing, IDT_GATE_32BIT_INT, 0);
    }

    asm volatile ("lidt %0" :: "m"(idtr));
}

void idt_set(uint8_t interrupt_id, int seg, void *handler, int gate, int ring) {
    idt_desc_t *d = &idt_descriptors[interrupt_id];
    d->offset_1 = (uint32_t)(handler);
    d->segment = seg;
    d->zero1 = 0;
    d->gate_type = gate;
    d->zero2 = 0;
    d->ring = ring;
    d->present = 1;
    d->offset_2 = (uint32_t)(handler)>>16;
}

void gdt_init() {
    asm volatile ("cli");
    gdtr.size = sizeof(gdt_descriptors)-1;
    gdtr.descriptors = (uint32_t)gdt_descriptors;
    memset(tss, 0, sizeof(tss));
    tss[1] = 0x1000000;
    tss[0] = 0x10;

    memset(&gdt_descriptors[0], 0, sizeof(gdt_descriptors[0]));
    for (size_t i = 1; i < GDT_CAP; ++i) {
        gdt_set(i, 0xFFFFF, 0, false, false, 0);
    }
    gdt_set(1, 0xFFFFF, 0, true, false, 0);
    gdt_set(2, 0xFFFFF, 0, false, false, 0);
    gdt_desc_t *tssd = &gdt_descriptors[3];
    tssd->limit_lo = sizeof(tss);
    tssd->base_lo = (uint32_t)tss;
    tssd->base_mid = (uint32_t)tss>>16;
    tssd->base_hi = (uint32_t)tss>>24;
    tssd->access = 0x89;
    tssd->limit_hi_and_flags = 0x40;
    gdt_reload();
    asm volatile (
                  "movw $0x18, %%ax\n"
                  "ltr %%ax\n" ::: "eax"
                  );
}

void gdt_set(uint16_t id, uint32_t limit, uint32_t base, bool is_executable, bool is_tss, uint8_t ring) {
    if (id >= GDT_CAP) return; // TODO: verbose errors.
    limit >>= 12;
    gdt_desc_t *d = &gdt_descriptors[id];

    // Usable stuff
    d->limit_lo = limit;
    d->limit_hi = limit>>16;
    d->base_lo = base;
    d->base_mid = base>>16;
    d->base_hi = base>>24;
    d->is_executable = is_executable;
    d->isnt_tss = !is_tss;
    d->ring = ring;

    // Useless stuff
    d->a = 1;
    d->rw = 1;
    d->dc = 0;
    d->p = 1;
    d->zero = 0;
    d->is_64_bit = 0;
    d->is_32_bit = 1;
    d->is_limit_in_pages = 1;
}

void gdt_reload() {
    asm volatile (
                  "lgdt %0\n"
                  "movw $0x10, %%ax\n"
                  "movw %%ax, %%ds\n"
                  "movw %%ax, %%ss\n"
                  "movw %%ax, %%es\n"
                  "jmpl $0x08, $.cs\n"
                  ".cs:"
                  :: "m"(gdtr): "eax"
                  );
}

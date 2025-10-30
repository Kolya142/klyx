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
#include <stdbool.h>
#include <stdint.h>

void memcpy(void *dest, void *src, unsigned long count);
void memset(void *dest, unsigned char c, unsigned long count);

void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
void outw(unsigned short port, unsigned short data);
unsigned short inw(unsigned short port);
void outd(unsigned short port, unsigned int data);
unsigned int ind(unsigned short port);

typedef struct gdt_desc {
    uint16_t limit_lo;
    
    uint16_t base_lo;
    
    uint8_t base_mid;

    union {
        struct {
            uint8_t a : 1; // Always set to 1
            uint8_t rw : 1; // Always set to 1
            uint8_t dc : 1; // Always set to 0
            uint8_t is_executable : 1;
            uint8_t isnt_tss : 1;
            uint8_t ring : 2;
            uint8_t p : 1; // Always set to 1 if descriptor mapped
        };
        uint8_t access;
    };

    union {
        struct {
            uint8_t limit_hi : 4;
            uint8_t zero : 1; // Always set zero to 0
            uint8_t is_64_bit : 1; // Always set to 0
            uint8_t is_32_bit : 1; // Always set to 1
            uint8_t is_limit_in_pages : 1; // Always set to 1. TODO: add support for lower limits
        };
        uint8_t limit_hi_and_flags;
    };
        
    uint8_t base_hi;
} __attribute__((packed)) gdt_desc_t;

typedef struct idt_desc {
    uint16_t offset_1;
    uint16_t segment;
    uint8_t zero1;
    uint8_t gate_type : 4;
    uint8_t zero2 : 1;
    uint8_t ring : 2; // you can touch interrupt only if your_current_ring<=idt_desc.ring.
    uint8_t present : 1;
    uint16_t offset_2;
} __attribute__((packed)) idt_desc_t;

_Static_assert(sizeof(idt_desc_t)==8, "idt descriptor size must be 8 bytes");
_Static_assert(sizeof(gdt_desc_t)==8, "gdt descriptor size must be 8 bytes");

typedef struct gdt_reg {
    uint16_t size;
    uint32_t descriptors;
} __attribute__((packed)) gdt_reg_t;

typedef struct idt_reg {
    uint16_t size;
    uint32_t descriptors;
} __attribute__((packed)) idt_reg_t;

#define IDT_GATE_TASK 0x05
#define IDT_GATE_16BIT_INT 0x06
#define IDT_GATE_16BIT_TRAP 0x07
#define IDT_GATE_32BIT_INT 0x0E
#define IDT_GATE_32BIT_TRAP 0x0F

extern idt_desc_t idt_descriptors[];
extern idt_reg_t idtr;
extern gdt_desc_t gdt_descriptors[];
extern gdt_reg_t gdtr;

void idt_init();
void idt_set(uint8_t interrupt_id, int seg, void *handler, int gate, int ring);
void gdt_init();
void gdt_set(uint16_t id, uint32_t limit, uint32_t base, bool is_executable, bool is_tss, uint8_t ring);
void gdt_reload();

// TODO: learn about LDT, LIDT

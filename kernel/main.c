#include <thirdparty/printf.h>
#include <klyx/console.h>
#include <klyx/kernel.h>
#include <klyx/shred.h>
#include <klyx/hw.h>

int errno;

void a(void) {
    current_tty = 0;
    printf(": ");
    char buf[64] = {0};
    tty_read(0, buf, 63);
    printf("\n%s\n", buf);
    for (;;) {
        current_tty = 0;
        shred_next_task();
    }
}

void b(void) {
    current_tty = 1;
    printf("B\n");
    for (;;) {
        shred_next_task();
    }
}

void kernel_ticker(void) {
    for (;;) {
        shred_next_task();
    }
}

volatile void panic(const char *msg) {
    printf("\n\nKernel panic: `%s`.\nTODO: be more verbose.", msg);
    for(;;);
}

__attribute__((naked)) void x86_trap_do_divide_by_0() {
    panic("divide by 0");
}

__attribute__((naked)) void x86_trap_nmi() {
    panic("NMI. WTF?");
}

__attribute__((naked)) void x86_trap_breakpoint() {
    panic("TODO: add int3 debugging");
}

__attribute__((naked)) void x86_trap_overflow() {
    panic("x86 overflow?");
}

__attribute__((naked)) void x86_trap_bound() {
    panic("x86 bound?");
}

__attribute__((naked)) void x86_trap_invalid_opcode() {
    panic("invalid opcode");
}

__attribute__((naked)) void x86_trap_device_no_avaliable() {
    panic("device no avaliable");
}

__attribute__((naked)) void x86_trap_double_fault() {
    panic("double fault");
}

__attribute__((naked)) void x86_trap_coprocessor_segment_overrun() {
    panic("coprocessor segment overrun");
}

__attribute__((naked)) void x86_trap_invalid_tss() {
    panic("invalid TSS");
}

__attribute__((naked)) void x86_trap_segment_not_present() {
    panic("segment XXXX not present");
}

__attribute__((naked)) void x86_trap_stack_segment_fault() {
    panic("stack segment fault");
}

__attribute__((naked)) void x86_trap_general_protection() {
    panic("general protection");
}

__attribute__((naked)) void x86_trap_page_fault() {
    panic("page fault");
}

__attribute__((naked)) void x86_trap_x87_error() {
    panic("FPU error");
}

__attribute__((naked)) void x86_trap_alignment() {
    panic("alignment");
}

__attribute__((naked)) void x86_trap_machine() {
    panic("x86 machine?");
}

__attribute__((naked)) void x86_trap_SIMD() {
    panic("SIMD");
}

volatile uint32_t ditch = 0;

__attribute__((naked))
void int_timer() {
    asm volatile ("cli\npushal");
    ++ditch;
    asm volatile ("mov $0x20, %al\n"
                  "outb %al, $0x20\n");
    asm volatile ("sti\npopal\niret");
}
__attribute__((naked))
void int_keyboard() {
    asm volatile ("cli\npushal");
    con_handle_input();
    asm volatile ("mov $0x20, %al\n"
                  "outb %al, $0x20\n");
    asm volatile ("sti\npopal\niret");
}

void kernel_start() {
    for (pid_t pid = 0; pid < TASKS_CAP; ++pid) {
        tasks[pid].status = TASK_DEAD;
    }
    ttys[0].write = con_putstr;
    ttys[0].read = con_read;
    ttys[0].getcur = con_getcur;
    ttys[0].setcur = con_setcur;
    ttys[0].width = 80;
    ttys[0].height = 25;
    ttys[1].write = con_putstr;
    ttys[1].read = con_read;
    ttys[1].getcur = con_getcur;
    ttys[1].setcur = con_setcur;
    ttys[1].width = 80;
    ttys[1].height = 25;
    
    gdt_init();
    idt_init();

    // idt_set(33, 0x08, test, IDT_GATE_32BIT_INT, 0);

    asm volatile ("sti");
    
    printf("Yeah!");
        
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);

    printf("Yeah!");

    uint16_t pit_div = 1193182 / 60;
    idt_set(32, 0x08, int_timer, IDT_GATE_32BIT_INT, 0);
    idt_set(33, 0x08, int_keyboard, IDT_GATE_32BIT_INT, 0);
    
    outb(0x43, 0x36);
    outb(0x40, pit_div & 0xff);
    outb(0x40, (pit_div>>8) & 0xff);
    outb(0x21, inb(0x21) & ~1);

    idt_set(0x00, 0x08, x86_trap_do_divide_by_0             , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x02, 0x08, x86_trap_nmi                        , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x03, 0x08, x86_trap_breakpoint                 , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x04, 0x08, x86_trap_overflow                   , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x05, 0x08, x86_trap_bound                      , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x06, 0x08, x86_trap_invalid_opcode             , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x07, 0x08, x86_trap_device_no_avaliable        , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x08, 0x08, x86_trap_double_fault               , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x09, 0x08, x86_trap_coprocessor_segment_overrun, IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x0A, 0x08, x86_trap_invalid_tss                , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x0B, 0x08, x86_trap_segment_not_present        , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x0C, 0x08, x86_trap_stack_segment_fault        , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x0D, 0x08, x86_trap_general_protection         , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x0E, 0x08, x86_trap_page_fault                 , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x10, 0x08, x86_trap_x87_error                  , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x11, 0x08, x86_trap_alignment                  , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x12, 0x08, x86_trap_machine                    , IDT_GATE_32BIT_TRAP, 0);
    idt_set(0x13, 0x08, x86_trap_SIMD                       , IDT_GATE_32BIT_TRAP, 0);
    
    printf("Yeah!");
        
    shred_make_task((word_t)a, 0, 0, 0x10);
    shred_make_task((word_t)b, 0, 0, 0x10);
    shred_make_task((word_t)kernel_ticker, 0, 0, 0x10);
    shred_start_tasking();
    for (;;);
}

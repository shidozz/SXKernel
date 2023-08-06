#include "../include/idt.h"
#include "../include/kio.h"
#include "../include/serial.h"
#include "../include/term.h"
#include "../include/convert.h"
#include "../include/term.h"
#include "../include/graph.h"
#include <stdint.h>


struct IdtDesc
{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));;

struct IdtEntry
{
    uint16_t offset_low;
    uint16_t segment;
    uint8_t ist;
    uint8_t attributes;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct Idt
{
    struct IdtEntry entries[256];
} __attribute__((packed));

extern uintptr_t __interrupt_vector[256];
extern void idt_update(struct IdtDesc *idt_descriptor);
static char *_exception_messages[32] = {
    "Divide by Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Detected Overflow",
    "Out Of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad Tss",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};


void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};
#define IDT_ENTRY_COUNT 256

static struct Idt _idt = {};

static struct IdtDesc _idt_desc = {
    .size = sizeof(struct Idt) - 1,
    .offset = (uintptr_t)&_idt,
};

struct IdtEntry idt_entry(uintptr_t handler, uint8_t ist, uint8_t idt_flags)
{
    return (struct IdtEntry){
        .attributes = idt_flags,
        .ist = ist,

        .offset_low = (handler),
        .offset_middle = (handler >> 16),
        .offset_high = (handler >> 32),

        .segment = 1 * 0x08,
        .zero = 0,
    };
}

int idt_install() {
    asm("cli");
    irq_remap();
    for (int i = 0; i < 256; i++)
    {
        _idt.entries[i] = idt_entry(__interrupt_vector[i], 0, IDT_GATE);
    }
    // Chargement de la table d'IDT
    idt_update(&_idt_desc);
    asm("sti");
    return 0;
}

void isrs_Handler(regs64_t *r){
    fb_clear();

    change_terminal_color(0, RGB(255, 0, 0));

    serial_printf("code d'erreur: %u\n",r->error_code);

    terminal_printf("code d'erreur: %u\n",r->error_code);

    serial_printf("%s Exception. System Halted!\n", _exception_messages[r->interrupt_number]);
    
    terminal_printf("%s Exception. System Halted!\n", _exception_messages[r->interrupt_number]);
    
    for(;;);
}

void irq_Handler(regs64_t *r){
    uint64_t irq = r->interrupt_number - 32;
    serial_printf("%u\n", irq);

    void (*handler)(regs64_t *r);
    handler = irq_routines[irq];

    if (handler)
    {
        handler(r);
    }
}

void pic_eoi(uint8_t interrupt){
    if (interrupt >= 8) {

        outb(0xA0, 0x20);

    }

    outb(0x20, 0x20);
}

void irq_remap(void)
{
    outb(0x11, 0x20);
    outb(0x11, 0xA0);

    outb(0x20, 0x21);
    outb(0x28, 0xA1);

    outb(0x04, 0x21);
    outb(0x02, 0xA1);

    outb(0x01, 0x21);
    outb(0x01, 0xA1);

    outb(0x0, 0x21);
    outb(0x0, 0xA1);
}

void irq_install_handler(int irq, void (*handler)(regs64_t *r))
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

uint64_t interrupt_handler(uint64_t rsp){

    regs64_t *regs = (regs64_t *)rsp;
    
    if (regs->interrupt_number < 32)
    {
        isrs_Handler(regs);
    }
    
    if (regs->interrupt_number >= 32 && regs->interrupt_number < 48)
    {
        irq_Handler(regs);
    }

    pic_eoi(regs->interrupt_number);
    return rsp;
}
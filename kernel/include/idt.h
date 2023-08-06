#ifndef __IDT_H__
#define __IDT_H__
#include <stdint.h>
#include "common.h"
#include "serial.h"
#include "kio.h"

enum
{
    IDT_TRAP = 0xeF,
    IDT_USER = 0x60,
    IDT_GATE = 0x8e,
};


// Déclaration de la fonction de traitement des interruptions
typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t interrupt_number, error_code;
    uint64_t rip, cs, rflags, rsp, ss;
} regs64_t;


void irq_remap(void);
int idt_install();
void irq_install_handler(int irq, void (*handler)(regs64_t *r));
void irq_uninstall_handler(int irq);
void pic_eoi(uint8_t irq);
#endif
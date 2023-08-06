#include "../include/gdt.h"
#include <stdint.h>

struct gdt_ptr
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

gdt_entry_t gdt[5];
struct gdt_ptr gp;

extern void gdtr_install();

void gdt_set_gate(gdt_entry_t* entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    entry->base_low = base & 0xFFFF;
    entry->base_middle = (base >> 16) & 0xFF;
    entry->base_high = (base >> 24) & 0xFF;

    entry->limit_low = limit & 0xFFFF;
    entry->granularity = (limit >> 16) & 0x0F;

    entry->granularity |= granularity & 0xF0;
    entry->access = access;
}

int gdt_install()
{
// Calcul de la taille de la GDT
    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint64_t)&gdt;
    // Initialisation des descripteurs de segment

    // Segment null (0)
    gdt_set_gate(&gdt[0], 0, 0, 0, 0);

    // Segment code du kernel (1)
    gdt_set_gate(&gdt[1], 0, 0xFFFFFFFF, GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE, GDT_GRANULARITY_64BIT);

    // Segment data du kernel (2)
    gdt_set_gate(&gdt[2], 0, 0xFFFFFFFF, GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA, GDT_GRANULARITY_64BIT);

    
    // Segment code des utilisateurs (3)
    gdt_set_gate(&gdt[3], 0, 0xFFFFFFFF, GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_CODE, GDT_GRANULARITY_64BIT);

    // Segment data des utilisateurs (4)
    gdt_set_gate(&gdt[4], 0, 0xFFFFFFFF, GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_DATA, GDT_GRANULARITY_64BIT);

    // Chargement de la GDT
    __asm__ volatile("lgdt %0" : : "m" (gp));
    
    // Chargement des segments de code et de données
    __asm__ volatile(
        "mov $0x10, %ax \n\t"
        "mov %ax, %ds \n\t"
        "mov %ax, %es \n\t"
        "mov %ax, %ss \n\t"
        "pushq $0x8 \n\t"
        "leaq 1f(%rip), %rax \n\t"
        "pushq %rax \n\t"
        "lretq \n\t"
        "1: \n\t"
    );


    //gdtr_install();
    return 0;
}
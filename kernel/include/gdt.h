#ifndef __GDT_H__
#define __GDT_H__

#include <stdint.h>
typedef struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} gdt_entry_t;
// Constantes pour les types de segments
#define GDT_ACCESS_PRESENT 0x80
#define GDT_ACCESS_RING0   0x00
#define GDT_ACCESS_RING3   0x60
#define GDT_ACCESS_CODE    0x1A
#define GDT_ACCESS_DATA    0x12
#define GDT_GRANULARITY_64BIT 0x20

void gdt_set_gate(gdt_entry_t* entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
int gdt_install();

#endif
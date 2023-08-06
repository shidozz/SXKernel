#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "common.h"

void init_serial();
void serial_putchar(char c);
void serial_puts(const char* str);
void serial_dec32(u32int number);
void serial_hex32(u32int number);
void serial_unsigned32(u32int number);
void serial_dec64(u64int number);
void serial_hex64(u64int number);
void serial_unsigned64(u64int number);
void serial_printf(const char* format, ...);
#endif
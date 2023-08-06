#ifndef __TERM_H__
#define __TERM_H__
#include <stdarg.h>
#include "common.h"
void terminal_nl();
void terminal_putchar(char str);
void terminal_puts(const char* str);
void terminal_hex64(u32int number);
void terminal_dec64(u64int number);
void terminal_unsigned64(u64int number);
void terminal_printf(const char* format, ...);
void change_terminal_color(uint32_t bg, uint32_t fg);
void change_terminal_bg(uint32_t bg);
void change_terminal_fg(uint32_t fg);
void change_terminal_fonts(int fonts);
#endif
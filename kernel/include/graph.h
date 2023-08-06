#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdint.h>
#include <stddef.h>

void drawPixel(uint32_t x, uint32_t y, uint32_t color);
void drawRectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void drawChar(uint8_t c, uint8_t font[4096], uint32_t x, uint32_t y, uint32_t bgColor, uint32_t fgColor);
void drawString(const char *string, uint8_t font[4096], uint16_t pos_x, uint16_t pos_y, uint32_t fgColor, uint32_t bgColor);
void fb_clear_color(int r, int g, int b);
void fb_clear();
#endif
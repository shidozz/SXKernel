#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include "../include/serial.h"
#include "../include/kio.h"
#include "../include/convert.h"
#include "../include/graph.h"
#include "../include/fonts.h"

// Function to clear the framebuffer
void fb_clear() {
    fb_clear_color(0, 0, 0);
}

// Function to clear the framebuffer from a color
void fb_clear_color(int r, int g, int b) {
    uint32_t *fb_ptr = framebuffer->address;
    for (size_t i = 0; i < framebuffer->height; i++) {
        for (size_t j = 0; j < framebuffer->width; j++) {
            fb_ptr[i * (framebuffer->pitch / 4) + j] = RGB(r, g, b);
        }
    }
}

// Function to draw a rectangle
void drawRectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
    uint32_t *fb_ptr = framebuffer->address;
    for (size_t i = y; i < y + height; i++) {
        for (size_t j = x; j < x + width; j++) {
            fb_ptr[i * (framebuffer->pitch / 4) + j] = color;
        }
    }
}

void drawPixel(uint32_t x, uint32_t y, uint32_t color) {
    uint32_t *fb_ptr = framebuffer->address;
    fb_ptr[y * (framebuffer->pitch / 4) + x] = color;
}

void drawChar(uint8_t c, uint8_t font[4096], uint32_t x, uint32_t y, uint32_t bgColor, uint32_t fgColor) {
    int32_t mask[8] = {128, 64, 32, 16, 8, 4, 2, 1};

    uint8_t *glyph = font + (int32_t)c * 16;

	for (uint8_t cy = 0; cy < 16; cy++) {
		for (uint8_t cx = 0; cx < 8; cx++) {
			drawPixel(x+cx, y+cy-12, glyph[cy]&mask[cx]?fgColor:bgColor);
		}
	}
}

void drawString(const char *string, uint8_t font[4096], uint16_t pos_x, uint16_t pos_y, uint32_t fgColor, uint32_t bgColor) {
	
	uint16_t length = strlen(string);
	for(uint16_t i = 0; i < length; i++) {
		drawChar(string[i], font, pos_x, pos_y, fgColor, bgColor);
		pos_x += 8;
	}
}
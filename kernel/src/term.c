#include <stdarg.h>
#include "../include/kio.h"
#include "../include/common.h"
#include "../include/graph.h"
#include "../include/fonts.h"
#include "../include/convert.h"
#include "../include/term.h"

uint32_t x = 4;
uint32_t y = 20;
uint32_t bgColor = ((uint32_t)0 << 16) | ((uint32_t)0 << 8) | (uint32_t)0;
uint32_t fgColor = ((uint32_t)255 << 16) | ((uint32_t)255 << 8) | (uint32_t)255;
uint8_t curFonts[4096];

/*
void change_terminal_fonts(int fonts){
    if(fonts == NULL) fonts = 3;
    if(fonts == 1)
    {
        memcpy(curFonts, g_8x16_font, sizeof(g_8x16_font));
    }
    if(fonts == 2)
    {
        memcpy(curFonts, et4000w32isa_8x16, sizeof(et4000w32isa_8x16));
    }
    if(fonts == 3)
    {
        memcpy(curFonts, BLADE3D_AGP_HIS__8x16, sizeof(BLADE3D_AGP_HIS__8x16));
    }
}
*/

void change_terminal_fonts(int fonts){
    switch (fonts)
    {
    case 2:
        memcpy(curFonts, g_8x16_font, sizeof(g_8x16_font));
        break;
    case 3:
        memcpy(curFonts, et4000w32isa_8x16, sizeof(et4000w32isa_8x16));
        break;
    case 1:
    default:
        memcpy(curFonts, BLADE3D_AGP_HIS__8x16, sizeof(BLADE3D_AGP_HIS__8x16));
        break;
    }
}

void change_terminal_color(uint32_t bg, uint32_t fg){
    bgColor = bg;
    fgColor = fg;
}

void change_terminal_bg(uint32_t bg){
    bgColor = bg;
}

void change_terminal_fg(uint32_t fg){
    fgColor = fg;
}

void terminal_nl(){
    x = 4;
    y += 20;
}

void terminal_putchar(char c) {

    if(c == '\n' || c == '\r')
        terminal_nl();
    else{
        drawChar((uint8_t)c, curFonts, x, y, bgColor, fgColor);
        x += 8;
    }
}

void terminal_puts(const char* str) {
    while (*str) {
        terminal_putchar(*str);
        str++;
    }
}

void terminal_hex64(u32int number){
    
    // Envoyer les chiffres hexadécimaux un par un (en commençant par les bits les plus significatifs)
    for (int i = 28; i >= 0; i -= 4) {
        u8int hexDigit = (number >> i) & 0xF;  // Extraire le chiffre hexadécimal
        u8int asciiDigit = hexDigit < 10 ? '0' + hexDigit : 'A' + (hexDigit - 10);
        drawChar(asciiDigit, curFonts, x, y, bgColor, fgColor);  // Envoyer le chiffre hexadécimal via le port série
        x += 8;
    }
}

void terminal_dec64(u64int number){
    // Conversion du nombre en un tableau de bytes
    uint8_t bytes[sizeof(u64int)];
    for (int i = 0; i < sizeof(u64int); i++) {
        bytes[i] = (number >> (i * 8)) & 0xFF;
    }
    
    // Envoi des bytes via le port série
    for (u64int i = sizeof(u64int) - 1; i >= 0; i--) {
        drawChar(bytes[i], curFonts, x, y, bgColor, fgColor);
        x += 8;
    }
}
void terminal_dec32(u32int number){
    // Conversion du nombre en un tableau de bytes
    uint8_t bytes[sizeof(u32int)];
    for (int i = 0; i < sizeof(u32int); i++) {
        bytes[i] = (number >> (i * 8)) & 0xFF;
    }
    
    // Envoi des bytes via le port série
    for (u32int i = sizeof(u32int) - 1; i >= 0; i--) {
        drawChar(bytes[i], curFonts, x, y, bgColor, fgColor);
        x += 8;
    }
}

void terminal_unsigned64(u64int number) {
    // Conversion du nombre en chaîne de caractères pour l'affichage
    char buffer[12]; // Pour stocker le nombre sous forme de chaîne de caractères (32 bits peuvent avoir au maximum 10 chiffres + '\0')
    int index = 11;
    buffer[index--] = '\0';

    if (number == 0) {
        buffer[index--] = '0';
    } else {
        while (number > 0 && index >= 0) {
            buffer[index--] = '0' + (number % 10);
            number /= 10;
        }
    }

    terminal_puts(&buffer[index + 1]);
}

void terminal_unsigned32(u32int number) {
    // Conversion du nombre en chaîne de caractères pour l'affichage
    char buffer[12]; // Pour stocker le nombre sous forme de chaîne de caractères (32 bits peuvent avoir au maximum 10 chiffres + '\0')
    int index = 11;
    buffer[index--] = '\0';

    if (number == 0) {
        buffer[index--] = '0';
    } else {
        while (number > 0 && index >= 0) {
            buffer[index--] = '0' + (number % 10);
            number /= 10;
        }
    }

    // Affichage de la chaîne de caractères
    terminal_puts(&buffer[index + 1]);
}

void terminal_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'c': {
                    char c = va_arg(args, int);
                    terminal_putchar(c);
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    terminal_puts(str);
                    break;
                }
                case 'd': {
                    u64int number = va_arg(args, u64int);
                    terminal_dec64(number);
                    break;
                }
                case 't': {
                    u32int number = va_arg(args, u32int);
                    terminal_dec32(number);
                    break;
                }
                case 'x': {
                    u64int number = va_arg(args, u64int);
                    terminal_hex64(number);
                    break;
                }
                case 'u': {
                    u64int number = va_arg(args, u64int);
                    terminal_unsigned64(number);
                    break;
                }
                case 'z': {
                    u32int number = va_arg(args, u32int);
                    terminal_unsigned32(number);
                    break;
                }
                default:
                    terminal_putchar(*format);
            }
        } else {
            terminal_putchar(*format);
        }
        format++;
    }

    va_end(args);
}
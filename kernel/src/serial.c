#include "../include/serial.h"
#include "../include/kio.h"
#include "../include/common.h"
#include <stdarg.h>

/* Définition des adresses des ports */
#define COM1_PORT 0x3F8

/* Fonction pour initialiser le port série */
void init_serial() {
    /* Configuration des registres de contrôle */
    outb(COM1_PORT + 1, 0x00);    /* Désactiver toutes les interruptions */
    outb(COM1_PORT + 3, 0x80);    /* Activer le DLAB (Divisor Latch Access Bit) */
    outb(COM1_PORT + 0, 0x03);    /* Réglage de la fréquence de transmission (baud rate) - partie basse (diviseur = 3) */
    outb(COM1_PORT + 1, 0x00);    /* Réglage de la fréquence de transmission (baud rate) - partie haute */
    outb(COM1_PORT + 3, 0x03);    /* 8 bits de données, pas de parité, 1 bit d'arrêt */
    outb(COM1_PORT + 2, 0xC7);    /* Configuration du FIFO (First-In-First-Out) : activer, vidage du contenu, FIFO 14 octets */
    outb(COM1_PORT + 4, 0x0B);    /* Activer les signaux DTR, RTS, OUT2 */
}

/* Fonction pour envoyer un caractère via le port série */
void serial_putchar(char c) {
    while ((inb(COM1_PORT + 5) & 0x20) == 0) {}   /* Attendre que le buffer de transmission soit prêt */
    if(c == '\n' || c == '\r'){
        outb(COM1_PORT, '\n');                           /* Envoyer le caractère */
        outb(COM1_PORT, '\r');                           /* Envoyer le caractère */
    }else{
        outb(COM1_PORT, c);                           /* Envoyer le caractère */
    }
}

/* Fonction pour envoyer une chaîne de caractères via le port série */
void serial_puts(const char* str) {
    while (*str) {
        serial_putchar(*str);
        str++;
    }
}
void serial_hex32(u32int number){
    while ((inb(COM1_PORT + 5) & 0x20) == 0) {}  // Attendre que le buffer de transmission soit prêt
    
    // Envoyer les chiffres hexadécimaux un par un (en commençant par les bits les plus significatifs)
    for (u32int i = 28; i >= 0; i -= 4) {
        u8int hexDigit = (number >> i) & 0xF;  // Extraire le chiffre hexadécimal
        u8int asciiDigit = hexDigit < 10 ? '0' + hexDigit : 'A' + (hexDigit - 10);
        outb(COM1_PORT, asciiDigit);  // Envoyer le chiffre hexadécimal via le port série
    }
}
void serial_hex64(u64int number){
    while ((inb(COM1_PORT + 5) & 0x20) == 0) {}  // Attendre que le buffer de transmission soit prêt
    
    // Envoyer les chiffres hexadécimaux un par un (en commençant par les bits les plus significatifs)
    for (int i = 28; i >= 0; i -= 4) {
        u8int hexDigit = (number >> i) & 0xF;  // Extraire le chiffre hexadécimal
        u8int asciiDigit = hexDigit < 10 ? '0' + hexDigit : 'A' + (hexDigit - 10);
        outb(COM1_PORT, asciiDigit);  // Envoyer le chiffre hexadécimal via le port série
    }
}
void serial_dec32(u32int number){
    while ((inb(COM1_PORT + 5) & 0x20) == 0) {}   // Attendre que le buffer de transmission soit prêt
    
    // Conversion du nombre en un tableau de bytes
    uint8_t bytes[sizeof(u32int)];
    for (int i = 0; i < sizeof(u32int); i++) {
        bytes[i] = (number >> (i * 8)) & 0xFF;
    }
    
    // Envoi des bytes via le port série
    for (u32int i = sizeof(u32int) - 1; i >= 0; i--) {
        outb(COM1_PORT, bytes[i]);
    }
}


void serial_dec64(u64int number){
    while ((inb(COM1_PORT + 5) & 0x20) == 0) {}   // Attendre que le buffer de transmission soit prêt
    
    // Conversion du nombre en un tableau de bytes
    uint8_t bytes[sizeof(u64int)];
    for (int i = 0; i < sizeof(u64int); i++) {
        bytes[i] = (number >> (i * 8)) & 0xFF;
    }
    
    // Envoi des bytes via le port série
    for (u64int i = sizeof(u64int) - 1; i >= 0; i--) {
        outb(COM1_PORT, bytes[i]);
    }
}

void serial_unsigned32(u32int number) {
    while ((inb(COM1_PORT + 5) & 0x20) == 0) {}   // Attendre que le buffer de transmission soit prêt

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
    serial_puts(&buffer[index + 1]);
}

void serial_unsigned64(u64int number) {
    while ((inb(COM1_PORT + 5) & 0x20) == 0) {}   // Attendre que le buffer de transmission soit prêt

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
    serial_puts(&buffer[index + 1]);
}


void serial_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'c': {
                    char c = va_arg(args, int);
                    serial_putchar(c);
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    serial_puts(str);
                    break;
                }
                case 'd': {
                    u64int number = va_arg(args, u64int);
                    serial_dec64(number);
                    break;
                }
                case 'x': {
                    u64int number = va_arg(args, u64int);
                    serial_hex64(number);
                    break;
                }
                case 'u': {
                    u64int number = va_arg(args, u64int);
                    serial_unsigned64(number);
                    break;
                }
                default:
                    serial_putchar(*format);
            }
        } else {
            serial_putchar(*format);
        }
        format++;
    }

    va_end(args);
}
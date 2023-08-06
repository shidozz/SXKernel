#include <stdint.h>
#include <stdbool.h>
#include "../include/keyboard.h"
#include "../include/term.h"
#include "../include/idt.h"
#include "../include/graph.h"
#include "../include/kio.h"
#include "../include/serial.h"

unsigned char KB_CURRENT_KEY = 0;
bool KB_KEY_PRESSED = false;
bool KB_KEY_RELEASED = false;

bool up_pressed = false;
bool down_pressed = false;
bool left_pressed = false;
bool right_pressed = false;

bool up_released = false;
bool down_released = false;
bool left_released = false;
bool right_released = false;

// Définition des variables pour le terminal
char command[256];
int commandIndex = 0;
bool inGui = false;
void executeCommand() {

    terminal_printf("\n");
    
    char cmd[50][255];    // Tableau de sous-chaînes
    for (int i = 0; i < 50; i++){
        cmd[i][0] = '\0';
    } 
    int count = 0;    // Nombre de sous-chaînes
    splitString(command, cmd, &count);
    if (strcmp(cmd[0], "hello") == 0) {
        terminal_printf("comment tu vas ?\n");
    }else if ((strcmp(cmd[0], "halt") == 0) || (strcmp(cmd[0], "hlt") == 0) || (strcmp(cmd[0], "shutdown") == 0)) {
        //shutdown();
    }else if ((strcmp(cmd[0], "restart") == 0) || (strcmp(cmd[0], "rst") == 0)) {
        //restart();
    }else if ((strcmp(cmd[0], "clear") == 0) || (strcmp(cmd[0], "cls") == 0)) {
        if((strcmp(cmd[1], "\0") != 0) && (strcmp(cmd[2], "\0") != 0) && (strcmp(cmd[3], "\0") != 0))
            fb_clear_color((int)cmd[1], (int)cmd[2], (int)cmd[3]);
        else
            fb_clear();
    }else if (strcmp(cmd[0], "help") == 0) {
        if (count > 1) {
            if ((strcmp(cmd[1], "help") == 0) || (strcmp(cmd[1], "?") == 0)) {
                terminal_printf(" - help: Display available commands\n");
                terminal_printf("   usage: \n");
                terminal_printf("     1)   help \n");
                terminal_printf("     2)   help hello \n");
                terminal_printf("     3)   help clear \n");
            }else if (strcmp(cmd[1], "hello") == 0) {
                terminal_printf(" - hello: Display a greeting message\n");
                terminal_printf("   usage: \n");
                terminal_printf("     1)   hello \n");
            }else if ((strcmp(cmd[1], "clear") == 0) || (strcmp(cmd[1], "cls") == 0)) {
                terminal_printf(" - clear: Clear the screen\n");
                terminal_printf("   usage: \n");
                terminal_printf("     1)   clear \n");
                terminal_printf("     2)   cls \n");
            }else if ((strcmp(cmd[1], "halt") == 0) || (strcmp(cmd[1], "hlt") == 0) || (strcmp(cmd[1], "shutdown") == 0)) {
                terminal_printf(" - halt: halt the cpu\n");
                terminal_printf("   usage: \n");
                terminal_printf("     1)   halt \n");
                terminal_printf("     2)   hlt \n");
                terminal_printf("     3)   shutdown \n");
            } else {
                terminal_printf("Help: la commande '");
                terminal_printf(cmd[1]);
                terminal_printf("' est inconnue !\n");
            }
        } else {
            terminal_printf("Available commands:\r\n");
            terminal_printf(" - hello: Display a greeting message\n");
            terminal_printf(" - help: Display available commands\n");
            terminal_printf(" - clear: Clear the screen\n");
            terminal_printf(" - halt: halt the cpu\n");
        }
    }else{
        terminal_printf("Unknown '");
        terminal_printf(cmd[0]);
        terminal_printf("' command. Type 'help' for available commands.\n");
    }
    terminal_printf("\n> ");
    // Réinitialiser la commande
    commandIndex = 0;
    memset((unsigned char *)command, 0, sizeof(command));
}

unsigned char kbdfr[128] =
{
    0,  27, '&', 'e', '"', '\'', '(', '-', 'e', '_',	/* 9 */
    'c', 'a', ')', '=', '\b',	/* Backspace */
    '\t',			/* Tab */
    'a', 'z', 'e', 'r',	/* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',	/* Enter key */
    0,			/* 29   - Control */
    'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',	/* 39 */
    'u', 0,   0,		/* Left shift */
    '*', 'w', 'x', 'c', 'v', 'b', 'n',			/* 49 */
    ',', ';', ':', '!',   0,				/* Right shift */
    '*',
    0,	/* Alt */
    ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,    0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
     0,	/* Page Up */
    '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned char kbdfr_shift[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
    '9', '0', '\'', '+', '\b',	/* Backspace */
    '\t',			/* Tab */
    'A', 'Z', 'E', 'R',	/* 19 */
    'T', 'Y', 'U', 'I', 'O', 'P', '^', '$', '\n',	/* Enter key */
    0,			/* 29   - Control */
    'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M',	/* 39 */
    'U', 0,   0,		/* Left shift */
    '*', 'W', 'X', 'C', 'V', 'B', 'N',			/* 49 */
    '?', '.', '/', '!',   0,				/* Right shift */
    '*',
    0,	/* Alt */
    ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,    0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
     0,	/* Page Up */
    '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};



void keyboard_handler(regs64_t *r)
{
    serial_printf("keyboard\n");
    unsigned char scancode;
    static int shift = 0;
    static int capslock = 0;
    static int numlock = 0;

    scancode = inb(0x60);
    if(inGui == false){
        if (scancode & 0x80)
        {
            //if ((inb(0x64) & 2) == 0) return;
            // Touche relâchée
            scancode &= 0x7F;
            if (scancode == 42 || scancode == 54){
                shift = 0; // Shift relâché
            }
            else if (scancode == 75)
            {
                left_released = true;
                right_released = false;
                up_released = false;
                down_released = false;
            }
            else if (scancode == 77)
            {
                left_released = false;
                right_released = true;
                up_released = false;
                down_released = false;
            }
            else if (scancode == 72)
            {
                up_released = true;
                left_released = false;
                right_released = false;
                down_released = false;
            }
            else if (scancode == 80)
            {
                left_released = false;
                right_released = false;
                up_released = false;
                down_released = true;
            }

            KB_KEY_PRESSED = false;
            KB_KEY_RELEASED = true;
        }
        else
        {
            
            if (scancode == 28) {
                // Touche Entrée
                executeCommand();
            }
            else if (scancode == 83 || ((capslock || shift == 1) && 211))
            {
                commandIndex++;
                command[commandIndex] = '\0';
                KB_CURRENT_KEY = ' ';
            }
            else if (scancode == 14)
            {
                commandIndex--;
                command[commandIndex] = '\0';
                KB_CURRENT_KEY = ' ';
            }
            else if (scancode == 42 || scancode == 54)
            {
                shift = 1; // Shift enfoncé
            }
            else if (scancode == 58)
            {
                capslock = !capslock; // Inversion de l'état de CAP LOCK
                outb(0x60, 0xED);
                outb(0x60, capslock ? 0x02 : 0x00);
            }
            else if (scancode == 69)
            {
                numlock = !numlock; // Inversion de l'état de NUM LOCK
                outb(0x60, 0xED);
                outb(0x60, numlock ? 0x01 : 0x00);
            }
            else if (scancode == 75)
            {
                left_pressed = true;
                right_pressed = false;
                up_pressed = false;
                down_pressed = false;
            }
            else if (scancode == 77)
            {
                left_pressed = false;
                right_pressed = true;
                up_pressed = false;
                down_pressed = false;
            }
            else if (scancode == 72)
            {
                up_pressed = true;
                left_pressed = false;
                right_pressed = false;
                down_pressed = false;
            }
            else if (scancode == 80)
            {
                left_pressed = false;
                right_pressed = false;
                up_pressed = false;
                down_pressed = true;
            }
            else
            {
                // Autres touches
                if(shift && capslock){
                    // Ajouter le caractère à la commande
                    command[commandIndex] = kbdfr[scancode];
                    commandIndex++;
                    command[commandIndex] = '\0';
                    KB_CURRENT_KEY = kbdfr[scancode];
                }
                else if (shift || capslock)
                {
                    // Ajouter le caractère à la commande
                    command[commandIndex] = kbdfr_shift[scancode];
                    commandIndex++;
                    command[commandIndex] = '\0';
                    KB_CURRENT_KEY = kbdfr_shift[scancode];
                }
                else
                {
                    // Ajouter le caractère à la commande
                    command[commandIndex] = kbdfr[scancode];
                    commandIndex++;
                    command[commandIndex] = '\0';
                    KB_CURRENT_KEY = kbdfr[scancode];
                    serial_printf("%c", kbdfr[scancode]);
                }
                KB_KEY_PRESSED = true;
                KB_KEY_RELEASED = false;
            }
        }
    }
}
int keyboard_install(){
    irq_install_handler(1, keyboard_handler);
    return 0;
}

bool is_key_pressed(unsigned char key) {
    return KB_KEY_PRESSED && (KB_CURRENT_KEY == key);
}

bool is_key_released(unsigned char key) {
    return KB_KEY_RELEASED && (KB_CURRENT_KEY == key);
}

bool is_up_released(){
    return up_released;
}
bool is_down_released(){
    return down_released;
}
bool is_left_released(){
    return left_released;
}
bool is_right_released(){
    return right_released;
}

bool is_up_pressed(){
    return up_pressed;
}
bool is_down_pressed(){
    return down_pressed;
}
bool is_left_pressed(){
    return left_pressed;
}
bool is_right_pressed(){
    return right_pressed;
}
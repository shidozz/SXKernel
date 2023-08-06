#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include "idt.h"
int keyboard_install();
void keyboard_handler(regs64_t *r);
bool is_key_pressed(unsigned char key);
bool is_key_released(unsigned char key);
bool is_up_pressed();
bool is_down_pressed();
bool is_left_pressed();
bool is_right_pressed();
bool is_up_released();
bool is_down_released();
bool is_left_released();
bool is_right_released();
#endif
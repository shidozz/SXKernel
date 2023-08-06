#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Windows structure importantes
struct Window{
    uint32_t posX;
    uint32_t posY;
    uint32_t Width;
    uint32_t Height;
    bool isOpen;
    const char *titre;
};
struct Window* create_Window(const char *titre, uint32_t posX, uint32_t posY, uint32_t Width, uint32_t Height);
void draw_window(struct Window* window);
#endif
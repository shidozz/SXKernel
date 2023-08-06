#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include "../include/serial.h"
#include "../include/kio.h"
#include "../include/convert.h"
#include "../include/graph.h"
#include "../include/window.h"

void draw_window(struct Window* window){
    if(!window->isOpen) window->isOpen = true;
    drawRectangle(window->posX, window->posY, window->Width, window->Height, RGB(255, 0, 255));
}

struct Window* create_Window(const char *titre, uint32_t posX, uint32_t posY, uint32_t Width, uint32_t Height){
    static struct Window w;
    w.Height  = Height;
    w.Width = Width;
    w.posX = posX;
    w.posY = posY;
    w.isOpen = true;
    w.titre = titre;

    return &w;
}
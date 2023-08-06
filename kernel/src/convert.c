#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include "../include/convert.h"


uint32_t RGB(int R, int G, int B) {
    return ((uint32_t)R << 16) | ((uint32_t)G << 8) | (uint32_t)B;
}

uint32_t RGB_T(rgb_t color) {
    return ((uint32_t)color.R << 16) | ((uint32_t)color.G << 8) | (uint32_t)color.B;
}

rgb_t int_To_RGB(uint32_t color) {
    return (rgb_t){.R = ((int)color >> 16), .G = ((int)color >> 8), .B = (int)color};
}
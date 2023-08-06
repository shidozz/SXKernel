#ifndef __CONVERT_H__
#define __CONVERT_H__

#include <stdint.h>
#include <stddef.h>
typedef struct {
    int R;
    int G;
    int B;
} rgb_t;
uint32_t RGB(int R, int G, int B);
uint32_t RGB_T(rgb_t color);
rgb_t int_To_RGB(uint32_t color);
#endif
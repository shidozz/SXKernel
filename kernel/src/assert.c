#include "../include/assert.h"
#include "../include/serial.h"
#include "../include/assert.h"

void _Assert(char *msg)
{
    serial_printf("%s", msg);
    serial_printf(" -- assertion failed");
    for(;;);
}
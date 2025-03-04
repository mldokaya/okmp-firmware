#ifndef OKMP_DISPLAY_H
#define OKMP_DISPLAY_H

#include "sh1106.h"
#include "spi.h"
#include "FreeRTOS.h"
#include "modes.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DIGIT_WIDTH 8
#define DIGIT_HEIGHT 16

typedef struct{
    const uint8_t spacing;
    const uint8_t bytes[10][DIGIT_WIDTH * DIGIT_HEIGHT / 8];
}display_digits;

void display_task(void *argument);

#endif
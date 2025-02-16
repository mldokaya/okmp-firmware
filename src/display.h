#ifndef DISPLAY_H
#define DISPLAY_H

#include "sh1106.h"
#include "gui.h"
#include "spi.h"
#include "FreeRTOS.h"
#include "modes.h"

typedef struct{
    omp_mode mode;
    uint8_t val;
}display_event;

void display_task(void *argument);

#endif
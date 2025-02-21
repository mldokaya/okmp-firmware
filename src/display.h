#ifndef OKMP_DISPLAY_H
#define OKMP_DISPLAY_H

#include "sh1106.h"
#include "gui.h"
#include "spi.h"
#include "FreeRTOS.h"
#include "modes.h"

void display_task(void *argument);

#endif
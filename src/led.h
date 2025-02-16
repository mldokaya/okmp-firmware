#ifndef OPAD_LED_H
#define OPAD_LED_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os2.h"
#include <stdbool.h>
#include "is31fl3731.h"
#include "i2c.h"
#include "modes.h"

#define LED_DEFAULT_PWM 60
#define LED_DEFAULT_STATE LED_DISPLAY_OFF

void led_task(void *argument);

#endif
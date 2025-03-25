#ifndef OKMP_LED_H
#define OKMP_LED_H

#include <stdbool.h>
#include "is31fl3731.h"
#include "i2c.h"
#include "modes.h"
#include "display.h"

typedef enum led_mode{
    LED_MODE_DISPLAY,
    LED_MODE_PWM,
    LED_N_MODES
}led_mode;

#define LED_DEFAULT_PWM 60
#define LED_DEFAULT_DISPLAY LED_DISPLAY_OFF

typedef struct led_state{
    led_mode mode;
    led_display_state display;
    uint8_t pwm;
}led_state;

void led_task(void *argument);

#endif
#ifndef OKMP_MODES_H
#define OKMP_MODES_H

#include <stdint.h>

typedef enum okmp_mode{
    MODE_LED_DISPLAY,
    MODE_LED_PWM,
    MODE_VOL,
    N_OKMP_MODES
}okmp_mode;

typedef enum input_action{
    ENCODER_SW_PRESSED,
    ENCODER_CW,
    ENCODER_CCW,
    ENCODER_NONE
}input_action;

typedef struct encoder_event{
    input_action action;
    okmp_mode mode;
}encoder_event;

typedef struct{
    okmp_mode mode;
    uint8_t val;
}display_event;

typedef enum led_display_state{
    LED_DISPLAY_ON,
    LED_DISPLAY_BLINK,
    LED_DISPLAY_OFF
}led_display_state;

#endif
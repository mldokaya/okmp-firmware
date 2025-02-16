#ifndef MODES_H
#define MODES_H

typedef enum omp_mode{
    MODE_LED_DISPLAY,
    MODE_LED_PWM,
    N_ENCODER_MODES
}omp_mode;

typedef enum encoder_action{
    ENCODER_SW_PRESSED,
    ENCODER_CW,
    ENCODER_CCW
}encoder_action;

typedef struct encoder_event{
    encoder_action action;
    omp_mode mode;
}encoder_event;

typedef enum led_display_state{
    LED_DISPLAY_ON,
    LED_DISPLAY_BLINK,
    LED_DISPLAY_OFF
}led_display_state;

#endif
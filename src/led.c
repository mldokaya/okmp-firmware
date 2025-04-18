#include "led.h"
#include "freertos.h"
#include "printf.h"

is31fl3731_dev issi;

int issi_write(void *i2c, uint8_t reg, uint8_t byte){
    i2c_write(reg, &byte, 1);
}

int issi_write_buf(void *i2c, uint8_t reg, uint8_t *buf, uint8_t n_bytes){
    i2c_write(reg, buf, n_bytes);
}

void led_task(void *argument){
    led_state state = {.mode = LED_MODE_DISPLAY, .display = LED_DEFAULT_DISPLAY, .pwm = LED_DEFAULT_PWM};
    display_event d_event = {.mode = MODE_LED_DISPLAY, .val = LED_DEFAULT_DISPLAY};
    input_action action;
    uint8_t led_x[16] = {8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11};
    uint8_t led_y[16] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
    issi.write_single = issi_write;
    issi.write_buf = issi_write_buf;
    led_init(&issi, led_x, led_y);
    while(1){
        if(osMessageQueueGet(led_queue_id, (void *)&action, NULL, portMAX_DELAY) == osOK){
            if(action == ENCODER_SW_PRESSED){
                state.mode++;
                state.mode %= LED_N_MODES;
            }
            switch(state.mode){
                case LED_MODE_DISPLAY:
                    if(action == ENCODER_SW_PRESSED){
                        d_event.mode = MODE_LED_DISPLAY;
                    }
                    else if(action == ENCODER_CW){
                        state.display = state.display == LED_DISPLAY_ON ? LED_DISPLAY_OFF : state.display - 1;
                    }
                    else if(action == ENCODER_CCW){
                        state.display = state.display == LED_DISPLAY_OFF ? LED_DISPLAY_ON : state.display + 1;
                    }
                    
                    if(state.display == LED_DISPLAY_ON){
                        issi_set_led_all(&issi, ISSI_FRAME1, led_x, led_y, true, true);
                        issi_set_function(&issi, ISSI_FUNC_DISPLAY, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_DISABLE | 2, true);
                    }
                    else if(state.display == LED_DISPLAY_BLINK){
                        issi_set_led_all(&issi, ISSI_FRAME1, led_x, led_y, true, true);
                        issi_set_function(&issi, ISSI_FUNC_DISPLAY, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_ENABLE | 2, true);
                    }
                    else if(state.display == LED_DISPLAY_OFF){
                        issi_set_led_all(&issi, ISSI_FRAME1, led_x, led_y, false, true);
                        issi_set_function(&issi, ISSI_FUNC_DISPLAY, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_DISABLE | 2, true);
                    }
                    d_event.val = state.display;
                    break;
                case LED_MODE_PWM:
                    if(action == ENCODER_SW_PRESSED){
                        // Make sure LEDs are on and not blinking
                        d_event.mode = MODE_LED_PWM;
                        issi_set_led_all(&issi, ISSI_FRAME1, led_x, led_y, true, false);
                        issi_set_function(&issi, ISSI_FUNC_DISPLAY, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_DISABLE | 2, true);
                    }
                    else{
                        action == ENCODER_CW ? state.pwm++ : state.pwm--;
                    }
                    issi_set_pwm_all(&issi, ISSI_FRAME1, led_x, led_y, state.pwm, true);
                    d_event.val = state.pwm;
                    break;
                default:
                    break;
            }
            osMessageQueuePut(display_queue_id, (void *)&d_event, (int)NULL, portMAX_DELAY);
        }
    }
}

int led_init(struct is31fl3731_dev *issi, uint8_t *led_x, uint8_t *led_y){
    display_event d_event = {.mode = MODE_LED_DISPLAY, .val = LED_DEFAULT_DISPLAY};
    issi_set_function(issi, ISSI_FUNC_SHUTDOWN, ISSI_NORMAL, true);
    issi_set_led_all(issi, ISSI_FRAME1, led_x, led_y, false, false);
    issi_set_blink_all(issi, ISSI_FRAME1, led_x, led_y, true, false);
    issi_set_pwm_all(issi, ISSI_FRAME1, led_x, led_y, 64, true);
    issi_set_function(issi, ISSI_FUNC_DISPLAY, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_DISABLE | 2, true);
    osMessageQueuePut(display_queue_id, (void *)&d_event, (int)NULL, portMAX_DELAY);
}
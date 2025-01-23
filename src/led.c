#include "led.h"
#include "printf.h"
#include "cmsis_os2.h"
#include "display.h"
issi_ctx issi;

int issi_write(uint8_t reg, uint8_t byte){
    i2c_write(reg, &byte, 1);
}

int issi_write_buf(uint8_t reg, uint8_t *buf, uint8_t n_bytes){
    i2c_write(reg, buf, n_bytes);
}

static void update_led_state(issi_ctx *issi, bool *state, uint8_t *led_x, uint8_t *led_y, uint8_t function){
    issi_set_page(issi, ISSI_FRAME1);
    issi_set_led_region(issi, ISSI_FRAME1, led_x, led_y, state, true);
    issi_set_page(issi, ISSI_FUNCTION);
    issi_set_function(issi, ISSI_FUNC_DISPLAY, function, true);
}

void led_task(void *argument){
    osMessageQueueId_t led_queue_id = ((osMessageQueueId_t *)argument)[0];
    osMessageQueueId_t display_queue_id = ((osMessageQueueId_t *)argument)[1];
    // led_event event;
    encoder_event event;
    issi.write_single = issi_write;
    issi.write_buf = issi_write_buf;
    issi_set_page(&issi, ISSI_FUNCTION);
    issi_set_function(&issi, ISSI_FUNC_SHUTDOWN, ISSI_NORMAL, true);
    issi_set_page(&issi, ISSI_FRAME1);
    uint8_t led_x[16] = {8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11};
    uint8_t led_y[16] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
    bool led[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
    bool off[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
    bool blink[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
    uint8_t pwm[16] = {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};
    issi_set_led_region(&issi, ISSI_FRAME1, led_x, led_y, led, true);
    issi_set_blink_region(&issi, ISSI_FRAME1, led_x, led_y, blink, true);
    issi_set_pwm_region(&issi, ISSI_FRAME1, led_x, led_y, pwm, true);
    issi_set_page(&issi, ISSI_FUNCTION);
    issi_set_page(&issi, ISSI_FUNCTION);
    issi_set_page(&issi, ISSI_FRAME1);
    issi_update_frame(&issi, ISSI_FRAME1);
    issi_set_page(&issi, ISSI_FUNCTION);
    issi_set_function(&issi, ISSI_FUNC_DISPLAY, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_DISABLE | 2, true);
    uint8_t pwm_val = 60;
    led_display_state state = LED_DISPLAY_ON;
    display_event d_event = {.mode = MODE_LED_DISPLAY, .val = LED_DISPLAY_ON};
    osMessageQueuePut(display_queue_id, (void *)&d_event, (int)NULL, portMAX_DELAY);
    while(1){
        if(osMessageQueueGet(led_queue_id, (void *)&event, NULL, portMAX_DELAY) == osOK){
            printf("EVENT\n");
            switch(event.mode){
                case MODE_LED_DISPLAY:
                        if(event.action == ENCODER_CW){
                            state = state == LED_DISPLAY_ON ? LED_DISPLAY_OFF : state - 1;
                        }
                        else if(event.action == ENCODER_CCW){
                            state = state == LED_DISPLAY_OFF ? LED_DISPLAY_ON : state + 1;
                        }
                        
                        if(state == LED_DISPLAY_ON){
                            // printf("ON\n");
                            update_led_state(&issi, led, led_x, led_y, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_DISABLE | 2);
                        }
                        else if(state == LED_DISPLAY_BLINK){
                            // printf("BLINK\n");
                            update_led_state(&issi, led, led_x, led_y, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_ENABLE | 2);
                        }
                        else if(state == LED_DISPLAY_OFF){
                            // printf("OFF\n");
                            update_led_state(&issi, off, led_x, led_y, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_DISABLE | 2);
                        }
                    d_event.mode = MODE_LED_DISPLAY;
                    d_event.val = state;
                    osMessageQueuePut(display_queue_id, (void *)&d_event, (int)NULL, portMAX_DELAY);
                    break;
                case MODE_LED_PWM:
                    if(event.action == ENCODER_SW_PRESSED){
                        // change OLED to PWM screen
                    }
                    else{
                        event.action == ENCODER_CW ? pwm_val++ : pwm_val--;
                        // printf("PWM: %d\n", pwm_val);
                        for(int i = 0; i < 16; i++){
                            pwm[i] = pwm_val;
                        }
                        d_event.val = pwm_val;
                    }
                    d_event.mode = MODE_LED_PWM;
                    d_event.val = pwm_val;
                    osMessageQueuePut(display_queue_id, (void *)&d_event, (int)NULL, portMAX_DELAY);
                    // Update PWM value
                    issi_set_page(&issi, ISSI_FRAME1);
                    issi_set_led_region(&issi, ISSI_FRAME1, led_x, led_y, led, true);
                    issi_set_pwm_region(&issi, ISSI_FRAME1, led_x, led_y, pwm, true);
                    issi_update_frame(&issi, ISSI_FRAME1);
                    issi_set_page(&issi, ISSI_FUNCTION);
                    issi_set_function(&issi, ISSI_FUNC_DISPLAY, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_DISABLE | 2, true);
                    break;
                default:
                    break;
            }
        }
    }
}
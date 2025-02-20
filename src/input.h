#ifndef OPAD_INPUT_H
#define OPAD_INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include "printf.h"
#include "cmsis_os2.h"
#include "gpio.h"
#include "led.h"
#include "modes.h"
#include "freertos.h"

#define N_ROWS 4
#define N_COLS 4
#define N_KEYS N_ROWS * N_COLS
#define ROW_GPIO_PORT GPIOB
#define COL_GPIO_PORT GPIOB
#define ROW_PIN_MASK 0x000000F0
#define RE_PIN_MASK 0x00000402
#define ROW_PIN_OFFSET 4
#define PUSH_THRESHOLD 10
#define SW_PUSH_THRESHOLD 5
#define ENCODER_MODES 2

typedef enum encoder_status{
    RE_NO_CHANGE,
    RE_SW_CHANGE,
    RE_POS_CHANGE
}encoder_status;

typedef enum encoder_dir{
    ENCODER_DIR_CCW,
    ENCODER_DIR_CW
}encoder_dir;

typedef struct key{
    uint8_t keycode;
    uint8_t bitmap_pos;
    uint8_t bitmap_val;
    bool pressed;
    int count;
}Key;

typedef struct key_report{
    uint8_t report[N_KEYS];
    uint8_t *modifier;
    uint8_t *bitmap;
}key_report;

typedef struct rotary_encoder{
    uint16_t old_state;
    uint16_t state;
    int raw;
    int pos;
    encoder_dir dir;
    uint8_t sw_state;
    bool sw_pressed;
    omp_mode mode;
    GPIO_TypeDef *port_AB;
    GPIO_TypeDef *port_SW;
    unsigned long pin_A;
    unsigned long pin_B;
    unsigned long pin_SW;
}rotary_encoder;

void input_init(Key *keys, struct key_report *report, rotary_encoder *re, const uint8_t *keycodes);
void task_input_update(void *argument);
void input_usb_update(Key *keys, struct key_report *report);
int input_update_keys(Key *keys);
int input_update_encoder(rotary_encoder *re);
int input_update_mode(omp_mode *mode, uint8_t val);

#endif

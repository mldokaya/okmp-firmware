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

typedef struct led_event{
    encoder_mode mode;
    encoder_mode prev;
    uint8_t val;
}led_event;

// int i2c_write(void *i2c, uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t byte, uint8_t n_bytes);
// int i2c_read(void *i2c, uint8_t addr, uint8_t reg, uint8_t *data);
// int issi_write(uint8_t reg, uint8_t *buf, uint8_t n_bytes);
// int issi_write_single(uint8_t reg, uint8_t byte);
// int issi_write_buf(uint8_t reg, uint8_t start, uint8_t n_bytes);
void led_task(void *argument);

/*

*/

#endif
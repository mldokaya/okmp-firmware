#ifndef OKMP_I2C_H
#define OKMP_I2C_H

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_gpio.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include <stdbool.h>

#define DMA_THRESHOLD 10000 // Totally arbitrary right now, might try to find a better value later

void i2c_init();
void i2c_write(uint8_t reg, uint8_t *data, size_t len);
void i2c_dma_tx_init();
void i2c_dma_write(const uint8_t *data, size_t len);

#endif
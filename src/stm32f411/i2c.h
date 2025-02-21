#ifndef OKMP_I2C_H
#define OKMP_I2C_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include <stdbool.h>

#define DMA_THRESHOLD 10000 // Totally arbitrary right now, might try to find a better value later

typedef struct i2c_config{
    osThreadId_t task_id;
    I2C_TypeDef *i2c;
    uint8_t addr;
    uint8_t *buffer;
}I2CConfig;

typedef enum i2c_write_mode{
    I2C_SINGLE_BYTE,
    I2C_BUFFER_BYTES
}I2CTransmitType;

typedef struct i2c_write_req{
    enum i2c_write_mode mode;
    uint8_t reg;
    union{
        uint8_t byte;
        struct{
            uint8_t start;
            uint8_t n_bytes;
        }range;
    };
}I2CTransmitReq;

void i2c_init();
void i2c_write(uint8_t reg, uint8_t *data, size_t len);
void i2c_dma_tx_init();
void i2c_dma_write(const uint8_t *data, size_t len);
void i2c_transmit_task(void *argument);

#endif
#ifndef OUTPUT_H
#define OUTPUT_H

#include "is31fl3731.h"
#include "i2c.h"
#include "sh1106.h"
#include "gui.h"
#include "spi.h"

typedef struct{
    uint8_t val;
}output_event;

int issi_write(uint8_t reg, uint8_t byte);
int issi_write_buf(uint8_t reg, uint8_t *buf, uint8_t n_bytes);
void output_task(void *argument);

#endif
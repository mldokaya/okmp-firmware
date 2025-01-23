#ifndef SPI2_H
#define SPI2_H

#include <stdint.h>

void cs_low();
void cs_high();
void spi_init();
void spi_transmit(uint8_t *data, uint8_t size);

#endif
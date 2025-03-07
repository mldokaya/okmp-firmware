#ifndef SPI2_H
#define SPI2_H

#include <stdint.h>
#include "cmsis_os2.h"
extern osSemaphoreId_t spi_sem_id;
extern osThreadId_t spi_thread_id;

void cs_low();
void cs_high();
void spi_init();
void spi_dma_init();
void spi_dma_transmit(uint8_t *data, uint32_t size);
void spi_transmit(uint8_t *data, uint8_t size);
void print_dma_status();

#endif
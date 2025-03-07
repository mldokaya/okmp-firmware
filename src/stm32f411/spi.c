#include "spi.h"
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "gpio.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_dma.h"
#include "printf.h"
#include <stdbool.h>

osThreadId_t spi_thread_id;

void spi_init(){
	SPI1->CR1 |= SPI_CR1_MSTR | // Set to Master
                 SPI_CR1_SSI |
                 SPI_CR1_SSM;
    
    SPI1->CR1 &= ~(SPI_CR1_CPHA | // Set polarity low
                   SPI_CR1_CPOL | // Set first data capture edge to first clock transition
                   SPI_CR1_BR | // Set baud rate to f_PCLK/2
                   SPI_CR1_LSBFIRST | // Set to MSB first
                   SPI_CR1_RXONLY | // Set to Full duplex
                   SPI_CR1_DFF); // Set to 8-bit format
    
    SPI1->CR2 |= SPI_CR2_TXDMAEN; // Enable DMA
	SPI1->CR1 |= SPI_CR1_SPE; // Enable SPI
    (void)SPI1->SR;

    spi_thread_id = osThreadGetId();
    osThreadFlagsSet(spi_thread_id, 0x1); // Set flag to indicate everything is ready
}

void spi_dma_init(){
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

    // Reset control register and wait until it's definitely disabled
    DMA2_Stream2->CR = 0x00;
    while(DMA2_Stream2->CR & DMA_SxCR_EN){}
    
    DMA2_Stream2->CR |= DMA_SxCR_CHSEL_1 | // Set channel 2
                        DMA_SxCR_DIR_0 | // Set direction to memory-to-peripheral mode
                        DMA_SxCR_MINC; // Set to memory increment mode
    DMA2_Stream2->PAR = (uint32_t)& SPI1->DR;
    LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);

    NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    NVIC_SetPriority(DMA2_Stream2_IRQn, 14);
}

void DMA2_Stream2_IRQHandler(void){
    if(DMA2->LISR & DMA_LISR_TCIF2){
        DMA2_Stream2->CR &= ~DMA_SxCR_EN;
        DMA2->LIFCR = DMA_LIFCR_CTCIF2; // Clear flag
        cs_high();
        osThreadFlagsSet(spi_thread_id, 0x1); // Set flag (the write function must wait for it)
    }
}

void spi_dma_transmit(uint8_t *data, uint32_t size){
    cs_low();
    DMA2->LIFCR = DMA_LIFCR_CTCIF2; // Clear flag
    DMA2_Stream2->PAR = (uint32_t)& SPI1->DR;
    DMA2_Stream2->M0AR = (uint32_t)data;
    DMA2_Stream2->NDTR = size;
    DMA2_Stream2->CR |= DMA_SxCR_EN; // Start transmission
}

void cs_low(){
    GPIOB->BSRR = GPIO_BSRR_BR0;
}

void cs_high(){
    GPIOB->BSRR = GPIO_BSRR_BS0;
}

void spi_transmit(uint8_t *data, uint8_t size){
    cs_low();

    for(int i = 0; i < size; i++){
        while(!(SPI1->SR & SPI_SR_TXE)){} // Wait for previous byte to be transferred
        SPI1->DR = data[i];
    }

    // Wait for SPI transmission to finish
    while(!(SPI1->SR & SPI_SR_TXE)){}
    while((SPI1->SR & SPI_SR_BSY)){}
    
    // Clear flags
	(void) SPI1->DR;
    (void) SPI1->SR;

    cs_high();
}
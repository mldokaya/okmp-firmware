#include "spi.h"
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "gpio.h"
#include "stm32f4xx_ll_spi.h"
#include "printf.h"

// OLED CS: PB0

void spi_init(){
    //Enable Port A clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    __NOP();
    __NOP();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    __NOP();
    __NOP();

    MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODE0_Msk, GPIO_MODER_MODE0_0);

    MODIFY_REG(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED0_Msk, GPIO_OSPEEDR_OSPEED0_1);

    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE7_Msk, 
               GPIO_MODER_MODE5_1 | GPIO_MODER_MODE7_1);

    MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED5_Msk | GPIO_OSPEEDR_OSPEED7_Msk, 
               GPIO_OSPEEDR_OSPEED5_1 | GPIO_OSPEEDR_OSPEED7_1);

    MODIFY_REG(GPIOA->AFR[0], GPIO_AFRL_AFSEL5_Msk | GPIO_AFRL_AFSEL7_Msk, 
               GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL5_2 | GPIO_AFRL_AFSEL7_0 | GPIO_AFRL_AFSEL7_2);

    //Enable SPI Clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    __NOP();
    __NOP();
    //Mode: Master
	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSI;

    SPI1->CR1 &= ~SPI_CR1_CPHA;
    SPI1->CR1 &= ~SPI_CR1_CPOL;
	//Baud rate to (8MHz / 2 = 4MHz)
	SPI1->CR1 |= SPI_CR1_BR_1;
	//MSB first
	SPI1->CR1 &= ~(SPI_CR1_LSBFIRST);
	//Full duplex (Transmit/Receive)
	SPI1->CR1 &= ~(SPI_CR1_RXONLY);
	//Data format 8-bit
	SPI1->CR1 &= ~(SPI_CR1_DFF);
	//Software Slave select
	SPI1->CR1 |= SPI_CR1_SSI;
	SPI1->CR1 |= SPI_CR1_SSM;
	//SPI Enable
	SPI1->CR1 |= SPI_CR1_SPE;
    (void)SPI1->SR;
    printf("SPI initialized\n");
}

void cs_low(){
    GPIOB->BSRR = GPIO_BSRR_BR0;
}

void cs_high(){
    GPIOB->BSRR = GPIO_BSRR_BS0;
}

void spi_transmit(uint8_t *data, uint8_t size){
    for(int i = 0; i < size; i++){
        while(!((SPI1->SR) & (1 << 1))){};
        SPI1->DR = data[i];
    }
    while(!((SPI1->SR) & (1 << 1))){};
    while (((SPI1->SR) & (1 << 7))){};
	 /*Clear OVR flag*/
	(void) SPI1->DR;
    (void) SPI1->SR;
}
#include "i2c.h"
#include "printf.h"

#define ch1 (1<<25)

osSemaphoreId_t i2c_sem_id;

void i2c_init(){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    GPIOB->MODER |= 0xA0000;
    GPIOB->AFR[1] |= 0x44;
    GPIOB->OTYPER |= GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9;
    I2C1->CR1 = I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;
    I2C1->CR1 &= ~I2C_CR1_ENGC;
    I2C1->CR2 |= I2C_CR2_LAST;
    I2C1->CR2 |= I2C_CR2_DMAEN;
    I2C1->CR2 |= 16;
    I2C1->CCR = 80;
    I2C1->TRISE = 17;
    I2C1->CR1 |= I2C_CR1_PE;
}

void i2c_dma_tx_init(){
    i2c_sem_id = osSemaphoreNew(1, 1, NULL);
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    DMA1_Stream6->CR = 0x00;
    while((DMA1_Stream6->CR) & DMA_SxCR_EN);
    DMA1_Stream6->CR |= ch1 | DMA_SxCR_MINC | DMA_SxCR_DIR_0 | DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE;
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
    NVIC_SetPriority(DMA1_Stream6_IRQn, 14);
    printf("p: %d\n", NVIC_GetPriority(DMA1_Stream6_IRQn));
}

void DMA1_Stream6_IRQHandler(void){
    if(DMA1->HISR & DMA_HISR_TCIF6){
        I2C1->CR1 |= I2C_CR1_STOP;
        DMA1->HIFCR=DMA_HIFCR_CTCIF6;
        printf("Hi\n");
        osSemaphoreRelease(i2c_sem_id);
    }
}

void i2c_write(uint8_t reg, uint8_t *data, size_t len){
    // Wait for bus to be free
    while(I2C1->SR2 & I2C_SR2_BUSY);
    
    // Generate start
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));
    (void)I2C1->SR1;
    // Send address
    I2C1->DR = (0x74 << 1);
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;
    while(!(I2C1->SR1 & I2C_SR1_TXE));

    // Send reg address
    I2C1->DR = reg;
    while(!(I2C1->SR1 & I2C_SR1_TXE));

    if(len > DMA_THRESHOLD){
        // printf("Using DMA\n");
        i2c_dma_write(data, len);
        (void)I2C1->SR1;
        (void)I2C1->SR2;
    }
    else{
        for(unsigned int i = 0; i < len; i++){
            I2C1->DR = data[i];
            while(!(I2C1->SR1 & I2C_SR1_BTF));
        }
        // Stop
        I2C1->CR1 |= I2C_CR1_STOP;
    }
}

void i2c_dma_write(const uint8_t *data, size_t len){
    if(data != NULL){
        printf("Waiting\n");
        osSemaphoreAcquire(i2c_sem_id, osWaitForever);
        DMA1_Stream6->CR &= ~DMA_SxCR_EN;
        while((DMA1_Stream6->CR) & DMA_SxCR_EN);
        DMA1_Stream6->M0AR = (uint32_t)data;
        DMA1_Stream6->PAR=(uint32_t)&I2C1->DR;
        DMA1_Stream6->NDTR = len;
        DMA1->HIFCR = (DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6);
        DMA1_Stream6->CR |= DMA_SxCR_EN;
        printf("Done\n");
    }
}

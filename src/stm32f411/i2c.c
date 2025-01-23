#include "i2c.h"
#include "printf.h"

#define PB8_ALT (1<<17)
#define PB9_ALT (1<<19)
#define I2C_AF4 (0x04)

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

// void i2c_transmit_task(void *argument){
//     struct i2c_config *config = (struct i2c_config *)argument; 
//     i2c_task_id = config->task_id;
//     struct i2c_write_req req;
//     while(1){
//         if(osMessageQueueGet(i2c_queue_id, (void *)&req, NULL, portMAX_DELAY) == osOK){
//             if(req.mode == I2C_SINGLE_BYTE){
//                 // printf("-%02X\n", req.reg);
//                 i2c_write(req.reg, &req.byte, 1);
//             }
//             else{
//                 // printf("-%02X\n", req.reg);
//                 i2c_write(req.reg, &config->buffer[req.range.start], req.range.n_bytes);
//             }
//         }
//     }
// }

// static void i2c_read(uint8_t addr, uint8_t reg_addr, uint8_t cmd_reg, uint8_t *data){
//     // Wait for bus to be free
//     while(I2C1->SR2 & I2C_SR2_BUSY);

//     // Generate start
//     I2C1->CR1 |= I2C_CR1_START;
//     // Wait for SB flag
//     while(!(I2C1->SR1 & I2C_SR1_SB));
//     // Read SR1
//     (void)I2C1->SR1;

//     // Send address with write
//     I2C1->DR = (addr << 1 | 0);
//     // Wait for ADDR flag
//     while(((I2C1->SR1) & I2C_SR1_ADDR) == 0);
//     // Read SR1 and SR2
//     (void)I2C1->SR1;
//     (void)I2C1->SR2;
//     // Wait for TXE flag
//     while(I2C_SR1_TXE != (I2C_SR1_TXE & I2C1->SR1));

//     // Disable acknowledge
//     I2C1->CR1 &= ~I2C_CR1_ACK;

//     // Send command register address
//     I2C1->DR = 0xFD;
//     while(!(I2C_SR1_BTF & I2C1->SR1));

//     // Send command data
//     I2C1->DR = cmd_reg;
//     while(!(I2C_SR1_BTF & I2C1->SR1));

//     // Restart
//     I2C1->CR1 |= I2C_CR1_START;
//     while(I2C_SR1_SB != (I2C_SR1_SB & I2C1->SR1));
//     (void)I2C1->SR1;

//     // Send address with write
//     I2C1->DR = (addr << 1 | 0);
//     // Wait for ADDR flag
//     while(((I2C1->SR1) & I2C_SR1_ADDR) == 0);

//     // Send register address
//     I2C1->DR = reg_addr;
//     while(!(I2C_SR1_BTF & I2C1->SR1));

//     // Restart again
//     I2C1->CR1 |= I2C_CR1_START;
//     while(I2C_SR1_SB != (I2C_SR1_SB & I2C1->SR1));
//     (void)I2C1->SR1;

//     // Send address with read
//     I2C1->DR = (addr << 1 | (uint8_t)0x01);
//     while(((I2C1->SR1) & I2C_SR1_ADDR) == 0);
//     (void)I2C1->SR2;

//     // Stop
//     I2C1->CR1 |= I2C_CR1_STOP;
//     while(!(I2C1->SR1 & I2C_SR1_RXNE));
//     *data = I2C1->DR;
// }

// void task_i2c_process_events(void *argument){
//     uint8_t *buffer = (uint8_t *)argument;
//     struct i2c_event event;
//     while(1){
//         if(osMessageQueueGet(i2c_queue_id, (void *)&event, NULL, 0) == osOK){
//             if(event.n_bytes){
//                 printf("-: %02X = %02X\n", event.reg, buffer[event.byte]);
//                 i2c_write(event.addr, event.reg, &buffer[event.byte], event.n_bytes);
//             }
//             else{
//                 printf("-: %02X = %02X\n", event.reg, event.byte);
//                 i2c_write(event.addr, event.reg, &event.byte, 1);
//             }
//         }
//     }
// }
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "main.h"
#include "gpio.h"
#include "spi.h"

void sysclk_ll_init(){

    LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
    while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0){}

    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

    LL_RCC_HSE_Enable();
    while(LL_RCC_HSE_IsReady() != 1){}

    LL_RCC_HSI_SetCalibTrimming(16);
    LL_RCC_HSI_Enable();
    while(LL_RCC_HSI_IsReady() != 1){}

    LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_12, 72, LL_RCC_PLLQ_DIV_3);
    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady() != 1){}
    while (LL_PWR_IsActiveFlag_VOS() == 0){}
    
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI){}
    LL_SetSystemCoreClock(16000000);

    /* Update the time base */
    if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK){
        Error_Handler();
    }
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

void freertos_init();

int main(void){
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    sysclk_ll_init();

    /* Initialize all configured peripherals */
    gpio_init();
    i2c_init();

    /* Init scheduler */
    osKernelInitialize();

    /* Call init function for freertos objects (in cmsis_os2.c) */
    freertos_init();

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */

    /* Infinite loop */
    while (1){};
}

/**
    * @brief  Period elapsed callback in non blocking mode
    * @note   This function is called  when TIM2 interrupt took place, inside
    * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
    * a global variable "uwTick" used as application time base.
    * @param  htim : TIM handle
    * @retval None
    */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM2){
        HAL_IncTick();
    }
}
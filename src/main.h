#ifndef OKMP_MAIN_H
#define OKMP_MAIN_H

// STM32 HAL/LL headers
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"

#include "pin_defs.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"

// Error handler used in HAL functions
void Error_Handler(void);

#endif
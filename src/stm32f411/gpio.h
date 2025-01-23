#ifndef STM32F4_GPIO_H
#define STM32F4_GPIO_H

#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_wwdg.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_cortex.h"
#include "pin_defs.h"

#define GPIO_MODE_INPUT         0x00000000
#define GPIO_MODE_OUTPUT        GPIO_MODER_MODER0_0
#define GPIO_MODE_ALTERNATE     GPIO_MODER_MODER0_1
#define GPIO_MODE_ANALOG        (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1)

#define GPIO_OTYPE_PUSHPULL     0x00000000
#define GPIO_OTYPE_OPENDRAIN    GPIO_OTYPER_OT0_Msk

#define GPIO_OSPEED_LOW         0x00000000
#define GPIO_OSPEED_MEDIUM      GPIO_OSPEEDR_OSPEED0_0
#define GPIO_OSPEED_FAST        GPIO_OSPEEDR_OSPEED0_1
#define GPIO_OSPEED_HIGH        (GPIO_OSPEEDR_OSPEED0_0 | GPIO_OSPEEDR_OSPEED0_1)

#define GPIO_PUPD_NONE          0x00000000
#define GPIO_PUPD_PULLUP        GPIO_PUPDR_PUPD0_0
#define GPIO_PUPD_PULLDOWN      GPIO_PUPDR_PUPD0_1

typedef struct{
    uint32_t mode;
    uint32_t otype;
    uint32_t ospeed;
    uint32_t pupd;
    uint32_t af;
}gpio_config;

void gpio_init();

void gpio_pin_init(GPIO_TypeDef *port, uint32_t pins, gpio_config *config);

#endif
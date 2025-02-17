#ifndef OMP_PIN_DEFS_H
#define OMP_PIN_DEFS

#include "stm32f4xx_ll_gpio.h"

#define OLED_RES_Pin LL_GPIO_PIN_4
#define OLED_RES_GPIO_Port GPIOA
#define OLED_A0_Pin LL_GPIO_PIN_6
#define OLED_A0_GPIO_Port GPIOA
#define OLED_CS_Pin LL_GPIO_PIN_0
#define OLED_CS_GPIO_Port GPIOB
#define RE_A_Pin LL_GPIO_PIN_1
#define RE_A_GPIO_Port GPIOB
#define RE_B_Pin LL_GPIO_PIN_10
#define RE_B_GPIO_Port GPIOB
#define COL3_Pin LL_GPIO_PIN_12
#define COL3_GPIO_Port GPIOB
#define COL2_Pin LL_GPIO_PIN_13
#define COL2_GPIO_Port GPIOB
#define COL1_Pin LL_GPIO_PIN_14
#define COL1_GPIO_Port GPIOB
#define COL0_Pin LL_GPIO_PIN_15
#define COL0_GPIO_Port GPIOB
#define RE_SW_Pin LL_GPIO_PIN_8
#define RE_SW_GPIO_Port GPIOA
#define RE_SW_EXTI_IRQn EXTI9_5_IRQn
#define MCU_LED_Pin LL_GPIO_PIN_9
#define MCU_LED_GPIO_Port GPIOA
#define ROW3_Pin LL_GPIO_PIN_4
#define ROW3_GPIO_Port GPIOB
#define ROW2_Pin LL_GPIO_PIN_5
#define ROW2_GPIO_Port GPIOB
#define ROW1_Pin LL_GPIO_PIN_6
#define ROW1_GPIO_Port GPIOB
#define ROW0_Pin LL_GPIO_PIN_7
#define ROW0_GPIO_Port GPIOB

struct gpio_pin{
    void *port;
    unsigned int pin;
};

#endif
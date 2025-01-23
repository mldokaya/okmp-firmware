#include "gpio.h"

void gpio_init(){
    LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
    LL_GPIO_InitTypeDef gpio_struct = {0};

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    gpio_struct.Pin = OLED_RES_Pin;
    gpio_struct.Mode = LL_GPIO_MODE_OUTPUT;
    gpio_struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    gpio_struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_struct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &gpio_struct);

    /**/
    gpio_struct.Pin = OLED_A0_Pin;
    gpio_struct.Mode = LL_GPIO_MODE_OUTPUT;
    gpio_struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    gpio_struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_struct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOA, &gpio_struct);

    /**/
    gpio_struct.Pin = OLED_CS_Pin;
    gpio_struct.Mode = LL_GPIO_MODE_OUTPUT;
    gpio_struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    gpio_struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_struct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOB, &gpio_struct);

    /**/
    gpio_struct.Pin = RE_A_Pin|RE_B_Pin|ROW3_Pin|ROW2_Pin|ROW1_Pin|ROW0_Pin;
    gpio_struct.Mode = LL_GPIO_MODE_INPUT;
    gpio_struct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOB, &gpio_struct);

    /**/
    gpio_struct.Pin = COL3_Pin|COL2_Pin|COL1_Pin|COL0_Pin;
    gpio_struct.Mode = LL_GPIO_MODE_OUTPUT;
    gpio_struct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    gpio_struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_struct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &gpio_struct);

    /**/
    gpio_struct.Pin = MCU_LED_Pin;
    gpio_struct.Mode = LL_GPIO_MODE_OUTPUT;
    gpio_struct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    gpio_struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_struct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &gpio_struct);

    gpio_struct.Pin = RE_SW_Pin;
    gpio_struct.Mode = LL_GPIO_MODE_INPUT;
    gpio_struct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &gpio_struct);

    gpio_struct.Pin = LL_GPIO_PIN_11 | LL_GPIO_PIN_12;
    gpio_struct.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio_struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_struct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_struct.Alternate = LL_GPIO_AF_10;
    LL_GPIO_Init(GPIOA, &gpio_struct);

    gpio_struct.Pin = LL_GPIO_PIN_9;
    gpio_struct.Mode = LL_GPIO_MODE_INPUT;
    gpio_struct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &gpio_struct);

    gpio_struct.Pin = LL_GPIO_PIN_10;
    gpio_struct.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio_struct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    gpio_struct.Pull = LL_GPIO_PULL_UP;
    gpio_struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(GPIOA, &gpio_struct);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_OTGFS);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE8);

    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_8;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
    LL_EXTI_Init(&EXTI_InitStruct);


    NVIC_SetPriority(OTG_FS_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
    NVIC_EnableIRQ(OTG_FS_IRQn);
}

void gpio_pin_init(GPIO_TypeDef *port, uint32_t pins, gpio_config *config){
    int pin_num = 0;
    while(pins){
        int pin_offset = pin_num * 2;
        if(pins & 0x0001){
            MODIFY_REG(port->MODER, 0x3 << pin_offset, config->mode << pin_offset);
            MODIFY_REG(port->OTYPER, 0x1 << pin_num, config->otype << pin_num);
            MODIFY_REG(port->OSPEEDR, 0x3 << pin_offset, config->ospeed << pin_offset);
            MODIFY_REG(port->PUPDR, 0x3 << pin_offset, config->pupd << pin_offset);
            if(config->mode == GPIO_MODE_ALTERNATE){
                uint8_t reg = pin_num / 8;
                uint8_t offset = (pin_num % 8) * 4;
                MODIFY_REG(port->AFR[reg], 0xF << offset, config->af << offset);
            }
        }
        pin_num++;
        pins >>= 1;
    }
}
#include "gpio.h"

void Error_Handler(void);
PCD_HandleTypeDef hpcd_USB_OTG_FS;

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
    gpio_struct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_struct.Alternate = LL_GPIO_AF_10;
    LL_GPIO_Init(GPIOA, &gpio_struct);

    gpio_struct.Pin = LL_GPIO_PIN_9;
    gpio_struct.Mode = LL_GPIO_MODE_INPUT;
    gpio_struct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &gpio_struct);

    gpio_struct.Pin = LL_GPIO_PIN_10;
    gpio_struct.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio_struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    gpio_struct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    gpio_struct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &gpio_struct);

    gpio_struct.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_7;
    gpio_struct.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio_struct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_struct.Pull = LL_GPIO_PULL_NO;
    gpio_struct.Alternate = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOA, &gpio_struct);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_OTGFS);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

    NVIC_SetPriority(OTG_FS_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
    NVIC_EnableIRQ(OTG_FS_IRQn);

    hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
    hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
    hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
    hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;

    if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK){
        Error_Handler();
    }
}

void Error_Handler(void){
    __disable_irq();
    while(1){}
}
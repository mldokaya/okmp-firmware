#include "output.h"
int issi_write(uint8_t reg, uint8_t byte){
    i2c_write(reg, &byte, 1);
}

int issi_write_buf(uint8_t reg, uint8_t *buf, uint8_t n_bytes){
    i2c_write(reg, buf, n_bytes);
}

uint8_t buffer[8 * 128];
uint8_t init_cmds[] = {
    SH1106_SET_DISPLAY | 0x00,
    SH1106_SET_DIV_RATIO,
    0x50,
    SH1106_SET_MUX_RATIO,
    0x3F,
    SH1106_SET_DISPLAY_OFFSET,
    0x00,
    SH1106_SET_START_LINE | 0x00,
    SH1106_SET_REMAP | 0x00,
    SH1106_SET_COM_DIR | 0x00,
    SH1106_SET_COMMMON_PADS,
    0x12,
    SH1106_SET_CONTRAST,
    0x9F,
    SH1106_SET_CHARGE_PERIOD,
    0x22,
    SH1106_SET_VCOM,
    0x40,
    SH1106_SET_ENTIRE_DISPLAY | 0x00,
    SH1106_SET_NORMAL_DISPLAY | 0x00,
    SH1106_SET_DISPLAY | 0x01
};

void send_cmd(uint8_t cmd){
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
}

void send_data(uint8_t *data, uint8_t len){
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
  HAL_SPI_Transmit(&hspi1, data, len, 100);
}

void sh1106_rst(sh1106_pin *rst){
  HAL_GPIO_WritePin((GPIO_TypeDef *)rst->port, rst->pin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin((GPIO_TypeDef *)rst->port, rst->pin, GPIO_PIN_SET);
}

void sh1106_set(sh1106_pin *pin, bool state){
  HAL_GPIO_WritePin((GPIO_TypeDef *)pin->port, pin->pin, state);
}

void sh1106_write(sh1106_ctx *ctx, uint8_t *data, const uint8_t n_bytes){
  HAL_SPI_Transmit((SPI_HandleTypeDef *)(ctx->spi), data, n_bytes, 1000);
}

void output_task(void *argument){
    osMessageQueueId_t *output_queue_id = (osMessageQueueId_t *)argument;
    issi_ctx issi;
    issi.write_single = issi_write;
    issi.write_buf = issi_write_buf;
    issi_set_page(&issi, ISSI_FUNCTION);
    issi_set_function(&issi, ISSI_FUNC_SHUTDOWN, ISSI_NORMAL, true);
    issi_set_page(&issi, ISSI_FRAME1);
    uint8_t led_x[16] = {8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11};
    uint8_t led_y[16] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
    bool led[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
    bool blink[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
    uint8_t pwm[16] = {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};
    issi_set_led_region(&issi, ISSI_FRAME1, led_x, led_y, led, true);
    issi_set_blink_region(&issi, ISSI_FRAME1, led_x, led_y, blink, true);
    issi_set_page(&issi, ISSI_FUNCTION);
    issi_set_page(&issi, ISSI_FUNCTION);
    issi_set_page(&issi, ISSI_FRAME1);
    issi_update_frame(&issi, ISSI_FRAME1);
    issi_set_page(&issi, ISSI_FUNCTION);
    issi_set_function(&issi, ISSI_FUNC_DISPLAY, ISSI_DISPLAY_IC_FRAME1 | ISSI_DISPLAY_BLINK_DISABLE | 2, true);
    sh1106_ctx sh1106;
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    sh1106.a0.pin = OLED_A0_Pin;
    sh1106.a0.port = (void *)OLED_A0_GPIO_Port;
    sh1106.rst.pin = OLED_RES_Pin;
    sh1106.rst.port = OLED_RES_GPIO_Port;
    sh1106.cs.pin = OLED_CS_Pin;
    sh1106.cs.port = (void *)OLED_CS_GPIO_Port;
    sh1106.write = sh1106_write;
    sh1106.init = sh1106_rst;
    sh1106.set = sh1106_set;
    sh1106.spi = &hspi1;
    sh1106_init(&sh1106);
    sh1106_send_cmd_list(&sh1106, init_cmds, sizeof(init_cmds));
    for(int i = 0; i < 8 * 128; i++){
        buffer[i] = 0x00;
    }
    draw_letter(buffer, letter_p, 38, 14);
    draw_letter(buffer, letter_w, 56, 14);
    draw_letter(buffer, letter_m, 74, 14);
    sh1106_update_region(&sh1106, buffer, 0, 0, 128, 64);
    output_event event;
    while(1){
        // if(osMessageQueueGet(*output_queue_id, (void *)&event, NULL, portMAX_DELAY) == osOK){
        //     printf("val: %d\n", event.val);
        // }
    }
}
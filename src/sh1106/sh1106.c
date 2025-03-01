#include "sh1106.h"

void sh1106_init(struct sh1106_dev *sh1106){
    sh1106->set(&sh1106->rst, false);
    sh1106->delay(1);
    sh1106->set(&sh1106->rst, true);
}

void sh1106_send_data(struct sh1106_dev *sh1106, uint8_t *data, uint8_t n_bytes){
    sh1106->set(&sh1106->a0, true); // A0 = high for display data
    sh1106->write((void *)sh1106, data, n_bytes);
}

void sh1106_send_cmd(struct sh1106_dev *sh1106, uint8_t cmd){
    sh1106->set(&sh1106->a0, false); // A0 = low for commands
    sh1106->write(sh1106, &cmd, 1);
}

void sh1106_send_cmd_list(struct sh1106_dev *sh1106, uint8_t *cmds, uint8_t n_cmds){
    sh1106->set(&sh1106->a0, false); // A0 = low for commands
    sh1106->write(sh1106, cmds, n_cmds);
}

void sh1106_set_col(struct sh1106_dev *sh1106, uint8_t line){
    line += SH1106_COL_OFFSET; // Shifting to account for unused columns
    sh1106_send_cmd(sh1106, line & 0x0F);
    sh1106_send_cmd(sh1106, (line >> 4) | 0x10);
}

void sh1106_clear(uint8_t *buffer){
    for(int i = 0; i < SH1106_PIXELS; i++){
        buffer[i] = 0x00;
    }
}

void sh1106_fill(uint8_t *buffer){
    for(int i = 0; i < SH1106_PIXELS; i++){
        buffer[i] = 0xFF;
    }
}

void sh1106_update_region(struct sh1106_dev *sh1106, uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h){
    int start_page = y / 8;
    int end_page = (y + h - 1) / 8;
    x += SH1106_COL_OFFSET; // Shifting to account for unused columns
    for(int i = start_page; i <= end_page; i++){
        sh1106_send_cmd(sh1106, 0xB0 + i);
        sh1106_send_cmd(sh1106, x & 0x0F);
        sh1106_send_cmd(sh1106, (x >> 4) | 0x10);
        sh1106_send_data(sh1106, &buffer[i * 128 + x - 2], w);
    }
}

void sh1106_clear_region(struct sh1106_dev *sh1106, uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h){
    int start_page = y / 8;
    int end_page = (y + h) / 8;
    x += SH1106_COL_OFFSET; // Shifting to account for unused columns
    for(int i = start_page; i < end_page; i++){
        for(int j = 0; j < w; j++){
            buffer[i * 128 + x + w] = 0x00;
        }
        sh1106_send_cmd(sh1106, 0xB0 + i);
        sh1106_send_cmd(sh1106, x & 0x0F);
        sh1106_send_cmd(sh1106, (x >> 4) | 0x10);
        sh1106_send_data(sh1106, &buffer[i * 128 + x - 2], w);
    }

}
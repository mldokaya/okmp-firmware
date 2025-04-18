#include "sh1106.h"

int sh1106_init(struct sh1106_dev *sh1106){
    ASSERT(sh1106);
    if(!sh1106){
        return SH1106_ERROR_INVALID_PARAM;
    }
    sh1106->reset();
    return SH1106_OK;
}

int sh1106_send_data(struct sh1106_dev *sh1106, uint8_t *data, uint16_t n_bytes){
    ASSERT(sh1106 && data);
    if(!sh1106 || !data){
        return SH1106_ERROR_INVALID_PARAM;
    }
    sh1106->write((void *)sh1106, data, n_bytes, true);
    return SH1106_OK;
}

int sh1106_send_cmd(struct sh1106_dev *sh1106, uint8_t cmd){
    ASSERT(sh1106);
    if(!sh1106){
        return SH1106_ERROR_INVALID_PARAM;
    }
    sh1106->write(sh1106, &cmd, 1, false);
    return SH1106_OK;
}

int sh1106_send_cmd_list(struct sh1106_dev *sh1106, uint8_t *cmds, uint8_t n_cmds){
    ASSERT(sh1106 && cmds);
    if(!sh1106 || !cmds){
        return SH1106_ERROR_INVALID_PARAM;
    }
    sh1106->write(sh1106, cmds, n_cmds, false);
    return SH1106_OK;
}

int sh1106_set_page(struct sh1106_dev *sh1106, uint8_t page){
    ASSERT(sh1106 && SH1106_IS_VALID_PAGE(page));
    if(!sh1106 || !SH1106_IS_VALID_PAGE(page)){
        return SH1106_ERROR_INVALID_PARAM;
    }
    sh1106_send_cmd(sh1106, SH1106_SET_PAGE + page);
    return SH1106_OK;
}

int sh1106_set_col(struct sh1106_dev *sh1106, uint8_t col){
    ASSERT(sh1106 && SH1106_IS_VALID_COL(col));
    if(!sh1106 || !SH1106_IS_VALID_COL(col)){
        return SH1106_ERROR_INVALID_PARAM;
    }
    col += SH1106_COL_OFFSET; // Shifting to account for unused columns
    sh1106_send_cmd(sh1106, col & 0x0F);
    sh1106_send_cmd(sh1106, (col >> 4) | SH1106_COLUMN_HIGH_OFFSET);
    return SH1106_OK;
}

int sh1106_clear(struct sh1106_dev *sh1106){
    ASSERT(sh1106);
    if(!sh1106){
        return SH1106_ERROR_INVALID_PARAM;
    }
    for(int i = 0; i < SH1106_BYTES; i++){
        sh1106->buffer[i] = 0x00;
    }
    return SH1106_OK;
}

int sh1106_fill(struct sh1106_dev *sh1106){
    ASSERT(sh1106);
    if(!sh1106){
        return SH1106_ERROR_INVALID_PARAM;
    }
    for(int i = 0; i < SH1106_BYTES; i++){
        sh1106->buffer[i] = 0xFF;
    }
    return SH1106_OK;
}

int sh1106_update_display(struct sh1106_dev *sh1106){
    ASSERT(sh1106);
    if(!sh1106){
        return SH1106_ERROR_INVALID_PARAM;
    }
    sh1106_set_page(sh1106, 0);
    sh1106_set_col(sh1106, 0);
    sh1106_send_data(sh1106, sh1106->buffer, SH1106_BYTES);
    return SH1106_OK;
}

int sh1106_update_region(struct sh1106_dev *sh1106, uint8_t x, uint8_t y, uint8_t w, uint8_t h){
    ASSERT(sh1106 && SH1106_IS_VALID_POS(x, y));
    if(!sh1106 || !SH1106_IS_VALID_POS(x, y)){
        return SH1106_ERROR_INVALID_PARAM;
    }
    int start_page = y / 8;
    int end_page = (y + h - 1) / 8;
    for(int i = start_page; i <= end_page; i++){
        sh1106_set_page(sh1106, i);
        sh1106_set_col(sh1106, x);
        sh1106_send_data(sh1106, &sh1106->buffer[i * 128 + x], w);
    }
    return SH1106_OK;
}

int sh1106_clear_region(struct sh1106_dev *sh1106, uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool update){
    ASSERT(sh1106 && SH1106_IS_VALID_POS(x, y));
    if(!sh1106 || !SH1106_IS_VALID_POS(x, y)){
        return SH1106_ERROR_INVALID_PARAM;
    }
    int start_page = y / 8;
    int end_page = (y + h) / 8;
    for(int i = start_page; i <= end_page; i++){
        for(int j = 0; j < w; j++){
            sh1106->buffer[i * 128 + x + j] = 0x00;
        }
    }
    if(update){
        return sh1106_update_region(sh1106, x, y, w, h);
    }
    return SH1106_OK;
}
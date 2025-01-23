#include "gui.h"

const uint8_t letter_p[2 * 16] = {
    0xFF, 0xFF, 0xFF, 0xFF,
    0xCF, 0xCF, 0xCF, 0xCF, 
    0xCF, 0xCF, 0xCF, 0xCF,
    0xFF, 0xFF, 0xFF, 0xFF,

    0xFF, 0xFF, 0xFF, 0xFF,
    0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03
};

const uint8_t letter_w[2 * 16] = {
    0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xF0, 0xF0,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xF0, 0xF0,
    0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t letter_m[2 * 16] = {
    0xFF, 0xFF, 0xFF, 0xFF,
    0x0F, 0x0F,
    0xFF, 0xFF, 0xFF, 0xFF,
    0x0F, 0x0F,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF
};

uint8_t digits[10][2 * 8] = {
    {
        0xFF, 0xFF,
        0x0F, 0x0F, 0x0F, 0x0F,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xF0, 0xF0, 0xF0, 0xF0,
        0xFF, 0xFF
    },
    {
        0x0F, 0x0F, 0x0F,
        0xFF, 0xFF,
        0x00, 0x00, 0x00,
        0xF0, 0xF0, 0xF0,
        0xFF, 0xFF,
        0xF0, 0xF0, 0xF0
    },
    {
        0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF3
    },
    {
        0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF,
        0xFF, 0xFF,
        0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF3,
        0xFF, 0xFF
    },
    {
        0xFF, 0xFF,
        0xC0, 0xC0, 0xC0, 0xC0,
        0xFF, 0xFF,
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0xFF, 0xFF
    },
    {
        0xFF, 0xFF,
        0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF,
        0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF3,
        0xFF, 0xFF
    },
    {
        0xFF, 0xFF,
        0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF,
        0xFF, 0xFF,
        0xF3, 0xF3, 0xF3, 0xF3,
        0xFF, 0xFF
    },
    {
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
        0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF
    },
    {
        0xFF, 0xFF,
        0xCF, 0xCF, 0xCF, 0xCF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xF3, 0xF3, 0xF3, 0xF3,
        0xFF, 0xFF
    },
    {
        0xFF, 0xFF,
        0xCF, 0xCF, 0xCF, 0xCF,
        0xFF, 0xFF,
        0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF3,
        0xFF, 0xFF
    }
};

void gui_draw_graphic(uint8_t *buffer, const gui_graphic *graphic, int x, int y){
    int init_page = y / 8;
    int pages = (y + graphic->height) / 8 - init_page;
    int g_width = graphic->width;
    uint8_t y_shift = y % 8;
    for(int i = 0; i < pages; i++){
        for(int j = 0; j < g_width; j++){
            int buf_idx = (init_page + i) * DISPLAY_WIDTH + x + j;
            if(y_shift){
                buffer[buf_idx] &= ~(0xFF << y_shift);
                buffer[buf_idx + DISPLAY_WIDTH] &= ~(0xFF >> (8 - y_shift));
                uint8_t upper_bits = graphic->bytes[i * g_width + j] << y_shift;
                uint8_t lower_bits = graphic->bytes[i * g_width + j] >> (8 - y_shift);
                buffer[buf_idx] |= upper_bits;
                buffer[buf_idx + DISPLAY_WIDTH] |= lower_bits;
            }
            else{
                buffer[buf_idx] |= graphic->bytes[i * g_width + j];
            }
        }
    }
}

void gui_draw(uint8_t *buffer, uint8_t *bytes, uint8_t w, uint8_t h, int x, int y){
    int init_page = y / 8;
    int pages = (y + h) / 8 - init_page;
    uint8_t y_shift = y % 8;
    for(int i = 0; i < pages; i++){
        for(int j = 0; j < w; j++){
            int buf_idx = (init_page + i) * DISPLAY_WIDTH + x + j;
            if(y_shift){
                buffer[buf_idx] &= ~(0xFF << y_shift);
                buffer[buf_idx + DISPLAY_WIDTH] &= ~(0xFF >> (8 - y_shift));
                uint8_t upper_bits = bytes[i * w + j] << y_shift;
                uint8_t lower_bits = bytes[i * w + j] >> (8 - y_shift);
                buffer[buf_idx] |= upper_bits;
                buffer[buf_idx + DISPLAY_WIDTH] |= lower_bits;
            }
            else{
                buffer[buf_idx] = bytes[i * w + j];
            }
        }
    }
}

// void gui_draw_number(uint8_t *buffer, uint8_t **digits, uint8_t val, uint8_t size, uint8_t offset, uint8_t w, uint8_t h, int x, int y){
//     for(int i = size - 1; i >= 0; i--){
//         gui_draw(buffer, digits[val % 10], w, h, x + i * (w + offset), y);
//         val /= 10;
//     }
// }

void draw_letter(uint8_t *buffer, const uint8_t *letter, int x, int y){
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 16; j++){
            int buf_idx = (y/8 + i) * 128 + x + j;
            uint8_t shift = y % 8;
            if(shift){
                buffer[buf_idx] &= ~(0xFF << shift);
                buffer[buf_idx + 128] &= ~(0xFF >> (8 - shift));
                uint8_t upper_bits = letter[i * 16 + j] << shift;
                uint8_t lower_bits = letter[i * 16 + j] >> (8 - shift);
                buffer[buf_idx] |= upper_bits;
                buffer[buf_idx + 128] |= lower_bits;
            }
            else{
                buffer[i*128 + j + x] |= letter[i*16 + j];
            }
        }
    }
}

void draw_digit(uint8_t *buffer, uint8_t *digit, int x, int y){
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 8; j++){
            int buf_idx = (y/8 + i) * 128 + x + j;
            uint8_t shift = y % 8;
            if(shift){
                buffer[buf_idx] &= ~(0xFF << shift);
                buffer[buf_idx + 128] &= ~(0xFF >> (8 - shift));
                uint8_t upper_bits = digit[i * 8 + j] << shift;
                uint8_t lower_bits = digit[i * 8 + j] >> (8 - shift);
                buffer[buf_idx] |= upper_bits;
                buffer[buf_idx + 128] |= lower_bits;
            }
            else{
                buffer[i*128 + j + x] |= digit[i * 8 + j];
            }
        }
    }
}
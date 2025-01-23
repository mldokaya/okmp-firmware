#ifndef GUI_H
#define GUI_H

#include <stdint.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DIGIT_WIDTH 8
#define DIGIT_HEIGHT 16

// typedef struct font_letter{
//     uint8_t bitmap[LETTER_HEIGHT][LETTER_WIDTH];
// }font_letter;

typedef struct{
    const uint8_t height;
    const uint8_t width;
    const uint8_t bytes[];
}gui_graphic;

typedef struct{
    const uint8_t spacing;
    const uint8_t bytes[10][DIGIT_WIDTH * DIGIT_HEIGHT / 8];
}gui_digits;

extern const uint8_t letter_p[2 * 16];

extern const uint8_t letter_w[2 * 16];

extern const uint8_t letter_m[2 * 16];

extern uint8_t digits[10][2 * 8];

void gui_draw_graphic(uint8_t *buffer, const gui_graphic *graphic, int x, int y);
void gui_draw(uint8_t *buffer, uint8_t *bytes, uint8_t w, uint8_t h, int x, int y);
void gui_draw_number(uint8_t *buffer, uint8_t **digits, uint8_t val, uint8_t size, uint8_t offset, uint8_t w, uint8_t h, int x, int y);
void draw_letter(uint8_t *buffer, const uint8_t *letter, int x, int y);
void draw_digit(uint8_t *buffer, uint8_t *digit, int x, int y);

#endif
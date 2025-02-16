#ifndef SH1106_DEFS_H
#define SH1106_DEFS_H

#include <stdint.h>

#define SH1106_PAGES 8
#define SH1106_WIDTH 128
#define SH1106_HEIGHT 64
#define SH1106_PIXELS SH1106_PAGES * SH1106_WIDTH

typedef struct sh1106_pin{
    void *port;
    int pin;
}sh1106_pin;

typedef struct sh1106_ctx sh1106_ctx;

typedef void (*sh1106_init_func)(sh1106_pin *);
typedef void (*sh1106_io_func)(sh1106_pin *, bool);
typedef void (*sh1106_write_func)(sh1106_ctx *, uint8_t *, const uint8_t);
typedef void (*sh1106_delay_func)(uint32_t);


typedef struct sh1106_ctx{
    uint8_t buffer[SH1106_PAGES * SH1106_WIDTH];
    void *spi;
    sh1106_pin a0;
    sh1106_pin cs;
    sh1106_pin rst;
    sh1106_io_func set;
    sh1106_write_func write;
    sh1106_delay_func delay;
}sh1106_ctx;

#define SH1106_COLUMN_LOW_OFFSET 0x00
#define SH1106_COLUMN_HIGH_OFFSET 0x10

#define SH1106_START_LINE_OFFSET 0x40

#define SH1106_SET_COLUMN_LOW 0x00
#define SH1106_SET_COLUMN_HIGH 0x10

enum sh1106_pump_voltage{
    SH1106_PUMP_VOLTAGE_6_4 = 0x30,
    SH1106_PUMP_VOLTAGE_7_4,
    SH1106_PUMP_VOLTAGE_8_0,
    SH1106_PUMP_VOLTAGE_9_0
};

#define SH1106_SET_START_LINE 0x40
#define SH1106_SET_CONTRAST 0x81
// 2nd byte 0x00 - 0xFF: contrast level
#define SH1106_SET_REMAP 0xA0
#define SH1106_SET_ENTIRE_DISPLAY 0xA4
#define SH1106_SET_NORMAL_DISPLAY 0xA6
#define SH1106_SET_MUX_RATIO 0xA8
// 2nd byte 0x00 - 0x3F: multiplex ratio
#define SH1106_SET_DC_DC_MODE 0xAD
// 2nd byte mode
#define SH1106_SET_DISPLAY 0xAE
#define SH1106_SET_PAGE 0xB0
#define SH1106_SET_COM_DIR 0xC0
#define SH1106_SET_DISPLAY_OFFSET 0xD3
// 2nd byte 0x00 - 0x3F: offset data set
#define SH1106_SET_DIV_RATIO 0xD5
// 2nd byte 0x00 - 0xFF
#define SH1106_SET_CHARGE_PERIOD 0xD9
// 2nd byte 0x00 - 0xFF
#define SH1106_SET_COMMMON_PADS 0xDA
#define SH1106_SET_VCOM 0xDB

#endif
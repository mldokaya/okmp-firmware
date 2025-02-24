#ifndef SH1106_DEFS_H
#define SH1106_DEFS_H

#include <stdbool.h>
#include <stdint.h>
#include "pin_defs.h"

#define SH1106_PAGES 8
#define SH1106_WIDTH 128
#define SH1106_HEIGHT 64
#define SH1106_PIXELS SH1106_PAGES * SH1106_WIDTH
#define SH1106_COL_OFFSET 2 // While the SH1106 supports 132x64 pixels, most displays using it seem to be 128x64 with 2 unused columns on either side

struct sh1106_dev; // Forward declaration for function pointers

// Typedefs for platform-specific functions the driver needs
typedef void (*sh1106_io_func)(struct gpio_pin *, bool);
typedef void (*sh1106_write_func)(struct sh1106_dev *, uint8_t *, const uint8_t);
typedef void (*sh1106_delay_func)(uint32_t);

struct sh1106_dev{
    void *spi;
    struct gpio_pin a0; // Controls whether data goes to the display RAM (high) or command register (low)
    struct gpio_pin cs; // Active low CS for SPI
    struct gpio_pin rst; // Active low reset
    sh1106_io_func set; // platform-specific function for setting the state of GPIO pins
    sh1106_write_func write; // platform-specific function for sending data via SPI
    sh1106_delay_func delay; // platform-specific function for delaying (in ms)
};

#define SH1106_COLUMN_LOW_OFFSET 0x00
#define SH1106_COLUMN_HIGH_OFFSET 0x10

#define SH1106_START_LINE_OFFSET 0x40

#define SH1106_SET_COLUMN_LOW 0x00
#define SH1106_SET_COLUMN_HIGH 0x10
#define SH1106_SET_PUMP_VOLTAGE 0x30

enum sh1106_pump_voltage{
    SH1106_PUMP_VOLTAGE_6_4 = 0x00,
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

#ifndef IS31FL3731_DEFS_H
#define IS31FL3731_DEFS_H

#include <stdbool.h>
#include <stdint.h>
#include "printf.h"

typedef int (*issi_write_buf_func)(void *i2c, uint8_t reg, uint8_t *buf, uint8_t n_bytes); // For writing n bytes to n contiguous registers
typedef int (*issi_write_single_func)(void *i2c, uint8_t reg, uint8_t byte); // For writing to a single register
typedef int (*issi_read_func)(void *i2c, uint8_t addr, uint8_t reg, uint8_t *buf); // For reading from a register (not used yet)

// These are all constants based on info from the datasheet
#define ISSI_FRAMES 8 // Number of frames
#define ISSI_FUNCTIONS 13 // Number of function registers
#define ISSI_LED_ROWS 9 // Number of rows in the matrix
#define ISSI_MATRICES 2 // There are 2 matrices, A and B
#define ISSI_LED_LOCATIONS ISSI_LED_ROWS * ISSI_MATRICES // Total number of LED locations/groups
#define ISSI_LED_BYTES 1 // 1 bit per LED
#define ISSI_BLINK_BYTES 1 // 1 bit per LED
#define ISSI_PWM_BYTES 8 // 1 byte per LED
#define ISSI_BYTES_PER_LOCATION (ISSI_LED_BYTES + ISSI_BLINK_BYTES + ISSI_PWM_BYTES) // Number of bytes used to control the LEDs for each location
#define ISSI_BYTES_PER_FRAME ISSI_LED_LOCATIONS * ISSI_BYTES_PER_LOCATION // Total number of bytes per frame

#define ISSI_LED_INDEX(f, x, y) f * ISSI_BYTES_PER_FRAME + ISSI_MATRICES * ISSI_LED_BYTES * y + x / 8
#define ISSI_BLINK_INDEX(f, x, y) ISSI_LED_INDEX(f, x, y) + ISSI_BLINK_OFFSET
#define ISSI_PWM_INDEX(f, x, y) f * ISSI_BYTES_PER_FRAME + ISSI_MATRICES * ISSI_PWM_BYTES * y + x + ISSI_PWM_OFFSET

#define ISSI_PINS_USED 5
#define ISSI_LED_COUNT 16

//
typedef struct is31fl3731_dev{
    void *i2c;
    uint8_t current_page; // Current page
    issi_write_single_func write_single;
    issi_write_buf_func write_buf;
    uint8_t buffer[ISSI_FRAMES * ISSI_BYTES_PER_FRAME + ISSI_FUNCTIONS];
}is31fl3731_dev;

// Command register address
#define ISSI_CMD 0xFDu

// LED register addresses
#define ISSI_CA1 0x00u
#define ISSI_CB1 0x01u
#define ISSI_CA2 0x02u
#define ISSI_CB2 0x03u
#define ISSI_CA3 0x04u
#define ISSI_CB3 0x05u
#define ISSI_CA4 0x06u
#define ISSI_CB4 0x07u
#define ISSI_CA5 0x08u
#define ISSI_CB5 0x09u
#define ISSI_CA6 0x0Au
#define ISSI_CB6 0x0Bu
#define ISSI_CA7 0x0Cu
#define ISSI_CB7 0x0Du
#define ISSI_CA8 0x0Eu
#define ISSI_CB8 0x0Fu
#define ISSI_CA9 0x10u
#define ISSI_CB9 0x11u

// LED register offsets
#define ISSI_BLINK_OFFSET 0x12u
#define ISSI_PWM_OFFSET 0x24u

#define ISSI_FUNC_OFFSET ISSI_FRAMES * ISSI_BYTES_PER_FRAME

// Function register addresses
typedef enum issi_func{
    ISSI_FUNC_CONFIG,
    ISSI_FUNC_PICTURE,
    ISSI_FUNC_AUTOPLAY1,
    ISSI_FUNC_AUTOPLAY2,
    ISSI_FUNC_RESERVED,
    ISSI_FUNC_DISPLAY,
    ISSI_FUNC_AUDIO_SYNC,
    ISSI_FUNC_FRAME_STATE,
    ISSI_FUNC_BREATH1,
    ISSI_FUNC_BREATH2,
    ISSI_FUNC_SHUTDOWN,
    ISSI_FUNC_AGC,
    ISSI_FUNC_ADC
}ISSI_FUNC;

typedef enum issi_page{
    ISSI_FRAME1,
    ISSI_FRAME2,
    ISSI_FRAME3,
    ISSI_FRAME4,
    ISSI_FRAME5,
    ISSI_FRAME6,
    ISSI_FRAME7,
    ISSI_FRAME8,
    ISSI_FUNCTION = 0x0B
}ISSI_PAGE;

typedef enum issi_mode{
    ISSI_MODE_PICTURE = 0x00,
    ISSI_MODE_AUTO_FRAME = 0x01 << 3,
    ISSI_MODE_AUDIO_FRAME = 0x02 << 3
}ISSI_MODE;

typedef enum issi_cns{
    ISSI_LOOP_ENDLESS = 0x00,
    ISSI_LOOP1 = 0x01 << 4,
    ISSI_LOOP2 = 0x02 << 4,
    ISSI_LOOP3 = 0x03 << 4,
    ISSI_LOOP4 = 0x04 << 4,
    ISSI_LOOP5 = 0x05 << 4,
    ISSI_LOOP6 = 0x06 << 4,
    ISSI_LOOP7 = 0x07 << 4
}ISSI_CNS;

typedef enum issi_fns{
    ISSI_LOOP_ALL,
    ISSI_LOOP_FRAME1,
    ISSI_LOOP_FRAME2,
    ISSI_LOOP_FRAME3,
    ISSI_LOOP_FRAME4,
    ISSI_LOOP_FRAME5,
    ISSI_LOOP_FRAME6,
    ISSI_LOOP_FRAME7
}ISSI_FNS;

typedef enum issi_ic{
    ISSI_DISPLAY_IC_INDEPENDENT,
    ISSI_DISPLAY_IC_FRAME1 = 1 << 5
}ISSI_IC;

typedef enum issi_be{
    ISSI_DISPLAY_BLINK_DISABLE,
    ISSI_DISPLAY_BLINK_ENABLE = 1 << 3
}ISSI_BE;

typedef enum issi_ae{
    ISSI_AUDIO_SYNC_DISABLE,
    ISSI_AUDIO_SYNC_ENABLE
}ISSI_AE;

typedef enum issi_b_en{
    ISSI_BREATH_DISABLE,
    ISSI_BREATH_ENABLE = 1 << 4
}ISSI_B_EN;

typedef enum issi_ssd{
    ISSI_SHUTDOWN,
    ISSI_NORMAL
}ISSI_SSD;

typedef enum issi_agcm{
    ISSI_SLOW_MODE,
    ISSI_FAST_MODE = 1 << 4
}ISSI_AGCM;

typedef enum issi_agc{
    ISSI_AGC_DISABLE,
    ISSI_AGC_ENABLE = 1 << 3
}ISSI_AGC;

typedef enum issi_ags{
    ISSI_AGS_0dB,
    ISSI_AGS_3dB,
    ISSI_AGS_6dB,
    ISSI_AGS_9dB,
    ISSI_AGS_12dB,
    ISSI_AGS_15dB,
    ISSI_AGS_18dB,
    ISSI_AGS_21dB
}ISSI_AGS;

#endif
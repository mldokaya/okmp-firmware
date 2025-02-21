#ifndef IS31FL3731_H
#define IS31FL3731_H

#include "is31fl3731_defs.h"

void issi_init_frame(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *data);

void issi_update_page(struct is31fl3731_dev *issi, enum issi_page page);

void issi_update_region(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, uint8_t n_leds);

void issi_update_frame(struct is31fl3731_dev *issi, uint8_t frame);

void issi_update_leds(struct is31fl3731_dev *issi, uint8_t frame, uint8_t reg, uint8_t len);

void issi_update_function(struct is31fl3731_dev *issi, enum issi_func func, uint8_t len);

void issi_set_led(struct is31fl3731_dev *issi, uint8_t frame, uint8_t x, uint8_t y, bool state, bool update);
void issi_set_led_region(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool *state, bool update);
void issi_set_led_all(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool state, bool update);

void issi_set_blink(struct is31fl3731_dev *issi, uint8_t frame, uint8_t x, uint8_t y, bool state, bool update);
void issi_set_blink_region(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool *state, bool update);
void issi_set_blink_all(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool state, bool update);

void issi_set_pwm(struct is31fl3731_dev *issi, uint8_t frame, uint8_t x, uint8_t y, uint8_t val, bool update);
void issi_set_pwm_region(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, uint8_t *state, bool update);
void issi_set_pwm_all(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, uint8_t state, bool update);

void issi_set_function(struct is31fl3731_dev *issi, enum issi_func func, uint8_t val, bool update);

void issi_set_functions(struct is31fl3731_dev *issi, enum issi_func *funcs, uint8_t *vals, uint8_t n_funcs, bool update);

#endif
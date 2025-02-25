#ifndef IS31FL3731_H
#define IS31FL3731_H

#include "is31fl3731_defs.h"

/**
 * @brief Sets the initial state of a frame in the buffer
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame to be set
 * @param state State the frame should be set to, or NULL for all 0s 
 */
void issi_init_frame(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *state);

/**
 * @brief Switches to the given page if it's not already on it
 * @param issi Struct for storing buffer and IO functions
 * @param page Page to switch to
 */
void issi_switch_page(struct is31fl3731_dev *issi, enum issi_page page);

/**
 * @brief Updates the data for the given frame with the buffer data
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame to be updated
 * @note This might change later, but for now the frame being updated is always the frame being displayed
 */
void issi_update_frame(struct is31fl3731_dev *issi, uint8_t frame);

/**
 * @brief Updates the states of n consecutive led registers
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame to be displayed
 * @param reg First register to update
 * @param n_regs Number of consecutive registers to update
 */
void issi_update_leds(struct is31fl3731_dev *issi, uint8_t frame, uint8_t reg, uint8_t n_regs);

/**
 * @brief Updates n consecutive function registers
 * @param issi Struct for storing buffer and IO functions
 * @param func First function to update
 * @param n_funcs Number of consecutive functions to update
 */
void issi_update_functions(struct is31fl3731_dev *issi, enum issi_func func, uint8_t n_funcs);

/**
 * @brief Sets the state for an individual LED
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame the LED state should be set for
 * @param x X-coordinate of the LED
 * @param y Y-coordinate of the LED
 * @param state State for the LED
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_led(struct is31fl3731_dev *issi, uint8_t frame, uint8_t x, uint8_t y, bool state, bool update);

/**
 * @brief Sets the states for a group of LEDs
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame the LED state should be set for
 * @param x X-coordinates for the group of LEDs
 * @param y Y-coordinates for the group of LEDs
 * @param state State for each LED in the group
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_led_group(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool *state, bool update);

/**
 * @brief Sets a group of LEDs to the same state
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame the LED state should be set for
 * @param x X-coordinates for the group of LEDs
 * @param y Y-coordinates for the group of LEDs
 * @param state State for all LEDs in the group
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_led_all(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool state, bool update);

/**
 * @brief Sets the blink state for an individual LED
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame the blink state should be set for
 * @param x X-coordinate of the LED
 * @param y Y-coordinate of the LED
 * @param state State for the LED
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_blink(struct is31fl3731_dev *issi, uint8_t frame, uint8_t x, uint8_t y, bool state, bool update);

/**
 * @brief Sets the blink states for a group of LEDs
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame the blink state should be set for
 * @param x X-coordinates for the group of LEDs
 * @param y Y-coordinates for the group of LEDs
 * @param state State for each LED in the group
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_blink_group(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool *state, bool update);

/**
 * @brief Sets a group of LEDs to the same blink state
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame the blink state should be set for
 * @param x X-coordinates for the group of LEDs
 * @param y Y-coordinates for the group of LEDs
 * @param state State for all LEDs in the group
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_blink_all(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool state, bool update);

/**
 * @brief Sets the PWM value for an individual LED
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame the pwm value should be set for
 * @param x X-coordinate of the LED
 * @param y Y-coordinate of the LED
 * @param val PWM value for the LED
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_pwm(struct is31fl3731_dev *issi, uint8_t frame, uint8_t x, uint8_t y, uint8_t val, bool update);

/**
 * @brief Sets the PWM values for a group of LEDs
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame the pwm value should be set for
 * @param x X-coordinates for the group of LEDs
 * @param y Y-coordinates for the group of LEDs
 * @param state PWM value for each LED in the group
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_pwm_group(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, uint8_t *val, bool update);

/**
 * @brief Sets a group of LEDs to the same PWM value
 * @param issi Struct for storing buffer and IO functions
 * @param frame Frame the pwm value should be set for
 * @param x X-coordinates for the group of LEDs
 * @param y Y-coordinates for the group of LEDs
 * @param state PWM value for all LEDs in the group
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_pwm_all(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, uint8_t val, bool update);

/**
 * @brief Sets the value for a function
 * @param issi Struct for storing buffer and IO functions
 * @param func Function to set
 * @param val Value to set for the function
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_function(struct is31fl3731_dev *issi, enum issi_func func, uint8_t val, bool update);

/**
 * @brief Sets the values for multiple functions
 * @param issi Struct for storing buffer and IO functions
 * @param funcs Functions to set
 * @param vals Values to set for each function
 * @param n_funcs Number of functions to set
 * @param update Send new data to driver now (true) or only change the buffer (false)
 */
void issi_set_functions(struct is31fl3731_dev *issi, enum issi_func *funcs, uint8_t *vals, uint8_t n_funcs, bool update);

#endif
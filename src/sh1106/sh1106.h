#ifndef SH1106_H
#define SH1106_H

#include "sh1106_defs.h"

/**
 * @brief Sets the display to its initial state
 * @param sh1106 Struct for storing buffer and IO functions
 */
void sh1106_init(struct sh1106_dev *sh1106);

/**
 * @brief Sends data to the display
 * @param sh1106 Struct for storing buffer and IO functions
 * @param data Pointer to data
 * @param n_bytes Number of data bytes to transmit
 */
void sh1106_send_data(struct sh1106_dev *sh1106, uint8_t *data, uint8_t n_bytes);

/**
 * @brief Sends a command to the display
 * @param sh1106 Struct for storing buffer and IO functions
 * @param cmd Command to be sent
 */
void sh1106_send_cmd(struct sh1106_dev *sh1106, uint8_t cmd);

/**
 * @brief Sends multiple commands to the display
 * @param sh1106 Struct for storing buffer and IO functions
 * @param cmds Array of commands to send
 * @param n_cmds Number of commands being sent
 */
void sh1106_send_cmd_list(struct sh1106_dev *sh1106, uint8_t *cmds, uint8_t n_cmds);

/**
 * @brief Sets the page display data will be written to
 * @param sh1106 Struct for storing buffer and IO functions
 * @param page Page to set
 */
void sh1106_set_page(struct sh1106_dev *sh1106, uint8_t page);

/**
 * @brief Sets the column display data will be written to
 * @param sh1106 Struct for storing buffer and IO functions
 * @param col Column to set
 */
void sh1106_set_col(struct sh1106_dev *sh1106, uint8_t col);

/**
 * @brief Clears a buffer (does not actually update the display driver though)
 * @param buffer Buffer to be cleared
 */
void sh1106_clear(uint8_t *buffer);

/**
 * @brief Fills a buffer (does not actually update the display driver though)
 * @param buffer Buffer to be filled
 */
void sh1106_fill(uint8_t *buffer);

/**
 * @brief Updates a rectangular w x h region of the display
 * @param sh1106 Struct for storing buffer and IO functions
 * @param buffer Buffer to use for updating
 * @param x X-coordinate for the upper left corner of the region
 * @param y X-coordinate for the upper left corner of the region
 * @param w Width of the region
 * @param h Height of the region
 */
void sh1106_update_region(struct sh1106_dev *sh1106, uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

/**
 * @brief Clears a rectangular w x h region of the display
 * @param sh1106 Struct for storing buffer and IO functions
 * @param buffer Buffer to keep updated
 * @param x X-coordinate for the upper left corner of the region
 * @param y X-coordinate for the upper left corner of the region
 * @param w Width of the region
 * @param h Height of the region
 */
void sh1106_clear_region(struct sh1106_dev *sh1106, uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

#endif
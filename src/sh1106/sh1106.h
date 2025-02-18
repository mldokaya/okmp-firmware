#ifndef SH1106_H
#define SH1106_H

#include <stdbool.h>
#include "sh1106_defs.h"

void sh1106_init(struct sh1106_dev *ctx);
void sh1106_send_data(struct sh1106_dev *ctx, uint8_t *data, uint8_t n_bytes);
void sh1106_send_cmd(struct sh1106_dev *ctx, uint8_t cmd);
void sh1106_send_cmd_list(struct sh1106_dev *ctx, uint8_t *cmds, uint8_t n_cmds);
void sh1106_set_col(struct sh1106_dev *ctx, uint8_t col);
void sh1106_clear(uint8_t *buffer);
void sh1106_fill(uint8_t *buffer);
void sh1106_update_region(struct sh1106_dev *ctx, uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void sh1106_clear_region(struct sh1106_dev *ctx, uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

#endif
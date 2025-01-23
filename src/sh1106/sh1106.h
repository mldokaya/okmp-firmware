#ifndef SH1106_H
#define SH1106_H

#include <stdbool.h>
#include "sh1106_defs.h"

void sh1106_init(sh1106_ctx *ctx);
void sh1106_send_data(struct sh1106_ctx *ctx, uint8_t *data, uint8_t n_bytes);
void sh1106_send_cmd(struct sh1106_ctx *ctx, uint8_t cmd);
void sh1106_send_cmd_list(struct sh1106_ctx *ctx, uint8_t *cmds, uint8_t n_cmds);
void sh1106_set_col(sh1106_ctx *ctx, uint8_t col);
void sh1106_clear(uint8_t *buffer);
void sh1106_fill(uint8_t *buffer);
void sh1106_update_region(struct sh1106_ctx *ctx, uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void sh1106_clear_region(struct sh1106_ctx *ctx, uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

// void sh1106_set_column(struct sh1106_ctx *ctx, uint8_t addr);
// void sh1106_set_pump_voltage(struct sh1106_ctx *ctx, enum sh1106_pump_voltage voltage);
// void sh1106_set_start_line(struct sh1106_ctx *ctx, uint8_t line);
// void sh1106_set_contrast(struct sh1106_ctx *ctx, uint8_t contrast);
// void sh1106_set_seg_remap(struct sh1106_ctx *ctx, bool reverse);
// void sh1106_set_entire_display(struct sh1106_ctx *ctx, bool on);
// void sh1106_set_display_mode(struct sh1106_ctx *ctx, bool reverse);
// void sh1106_set_mux_ratio(struct sh1106_ctx *ctx, uint8_t ratio);
// void sh1106_set_dc_dc_mode(struct sh1106_ctx *ctx, bool on);
// void sh1106_set_display_state(struct sh1106_ctx *ctx, bool on);
// void sh1106_set_page(struct sh1106_ctx *ctx, uint8_t page);
// void sh1106_set_com_dir(struct sh1106_ctx *ctx, bool flip);
// void sh1106_set_display_offset(struct sh1106_ctx *ctx, uint8_t offset);
// void sh1106_set_div_ratio(struct sh1106_ctx *ctx, uint8_t ratio);
// void sh1106_set_charge_period(struct sh1106_ctx *ctx, uint8_t period);
// void sh1106_set_common_pads(struct sh1106_ctx *ctx, bool alt);
// void sh1106_set_vcom(struct sh1106_ctx *ctx, uint8_t val);

#endif
#include "sh1106.h"
#include "FreeRTOS.h"

void sh1106_init(sh1106_ctx *ctx){
    ctx->set(&ctx->rst, false);
    ctx->delay(1);
    ctx->set(&ctx->rst, true);
}

void sh1106_send_data(struct sh1106_ctx *ctx, uint8_t *data, uint8_t n_bytes){
    ctx->set(&ctx->a0, true);
    ctx->write((void *)ctx, data, n_bytes);
}

void sh1106_send_cmd(struct sh1106_ctx *ctx, uint8_t cmd){
    ctx->set(&ctx->a0, false);
    ctx->write(ctx, &cmd, 1);
}

void sh1106_send_cmd_list(struct sh1106_ctx *ctx, uint8_t *cmds, uint8_t n_cmds){
    for(int i = 0; i < n_cmds; i++){
        // printf("sending: %02X\n", cmds[i]);
    }
    ctx->write(ctx, cmds, n_cmds);
}

void sh1106_set_col(sh1106_ctx *ctx, uint8_t line){
    line += 2;
    sh1106_send_cmd(ctx, line & 0x0F);
    sh1106_send_cmd(ctx, (line >> 4) | 0x10);
}

void sh1106_clear(uint8_t *buffer){
    for(int i = 0; i < SH1106_PIXELS; i++){
        buffer[i] = 0x00;
    }
}

void sh1106_fill(uint8_t *buffer){
    for(int i = 0; i < SH1106_PIXELS; i++){
        buffer[i] = 0xFF;
    }
}

void sh1106_update_region(struct sh1106_ctx *ctx, uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h){
    int start_page = y / 8;
    int end_page = (y + h - 1) / 8;
    x += 2;
    for(int i = start_page; i <= end_page; i++){
        sh1106_send_cmd(ctx, 0xB0 + i);
        sh1106_send_cmd(ctx, x & 0x0F);
        sh1106_send_cmd(ctx, (x >> 4) | 0x10);
        sh1106_send_data(ctx, &buffer[i * 128 + x - 2], w);
    }
}

void sh1106_clear_region(struct sh1106_ctx *ctx, uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h){
    int start_page = y / 8;
    int end_page = (y + h) / 8;
    x += 2;
    for(int i = start_page; i < end_page; i++){
        for(int j = 0; j < w; j++){
            buffer[i * 128 + x + w] = 0x00;
        }
        sh1106_send_cmd(ctx, 0xB0 + i);
        sh1106_send_cmd(ctx, x & 0x0F);
        sh1106_send_cmd(ctx, (x >> 4) | 0x10);
        sh1106_send_data(ctx, &buffer[i * 128 + x - 2], w);
    }

}
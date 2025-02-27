#include "is31fl3731.h"

void issi_init_frame(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *state){
    ISSI_ASSERT(issi != NULL && state != NULL);
    if(state == NULL){
        for(int i = 0; i < ISSI_BYTES_PER_FRAME; i++){
            issi->buffer[frame + i] = 0;
        }
    }
    else{
        for(int i = 0; i < ISSI_BYTES_PER_FRAME; i++){
            issi->buffer[frame + i] = state[i];
        }
    }
}

void issi_switch_page(struct is31fl3731_dev *issi, enum issi_page page){
    ISSI_ASSERT(issi != NULL);
    if(issi->current_page != page){
        issi->current_page = page;
        issi->write_single(issi->i2c, ISSI_CMD, page);
    }
}

void issi_update_frame(struct is31fl3731_dev *issi, uint8_t frame){
    ISSI_ASSERT(issi != NULL);
    issi_switch_page(issi, frame);
    issi->write_buf(issi->i2c, ISSI_CA1, &issi->buffer[frame * ISSI_BYTES_PER_FRAME], ISSI_BYTES_PER_FRAME);
}

void issi_update_leds(struct is31fl3731_dev *issi, uint8_t frame, uint8_t reg, uint8_t n_regs){
    ISSI_ASSERT(issi != NULL);
    issi->write_buf(issi->i2c, reg, &issi->buffer[frame * ISSI_BYTES_PER_FRAME + reg], n_regs);
}

void issi_update_functions(struct is31fl3731_dev *issi, enum issi_func func, uint8_t n_funcs){
    ISSI_ASSERT(issi != NULL);
    issi_switch_page(issi, ISSI_FUNCTION);
    issi->write_buf(issi->i2c, func, &issi->buffer[ISSI_FUNC_OFFSET + func], n_funcs);
}

void issi_set_led(struct is31fl3731_dev *issi, uint8_t frame, uint8_t x, uint8_t y, bool state, bool update){
    ISSI_ASSERT(issi != NULL);
    // Get the buffer data for this led and update the relevant bit with its new state
    uint8_t *reg = &issi->buffer[ISSI_LED_INDEX(frame, x, y)];
    *reg ^= (-state ^ *reg) & (1 << (x % 8));
    if(update){
        issi_update_leds(issi, frame, 2 * y + x / 8, 1);
    }
}

void issi_set_led_group(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool *state, bool update){
    ISSI_ASSERT(issi != NULL && x != NULL && y != NULL && state != NULL);
    if(state != NULL){
        for(int i = 0; i < ISSI_LED_COUNT; i++){
            uint8_t *reg = &issi->buffer[ISSI_LED_INDEX(frame, x[i], y[i])];
            *reg ^= (-state[i] ^ *reg) & (1 << (x[i] % 8));
        }
        if(update){
            issi_update_frame(issi, frame);
        }
    }
}

void issi_set_led_all(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool state, bool update){
    ISSI_ASSERT(issi != NULL && x != NULL && y != NULL);
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        uint8_t *reg = &issi->buffer[ISSI_LED_INDEX(frame, x[i], y[i])];
        *reg ^= (-state ^ *reg) & (1 << (x[i] % 8));
    }
    if(update){
        issi_update_frame(issi, frame);
    }
}

void issi_set_blink(struct is31fl3731_dev *issi, uint8_t frame, uint8_t x, uint8_t y, bool state, bool update){
    ISSI_ASSERT(issi != NULL);
    uint8_t *reg = &issi->buffer[ISSI_BLINK_INDEX(frame, x, y)];
    *reg ^= (-state ^ *reg) & (1 << (x % 8));
    if(update){
        issi_update_leds(issi, frame, 2 * y + x / 8 + 0x12, 1);
    }
}

void issi_set_blink_group(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool *state, bool update){
    ISSI_ASSERT(issi != NULL && x != NULL && y != NULL && state != NULL);
    if(state != NULL){
        for(int i = 0; i < ISSI_LED_COUNT; i++){
            uint8_t *reg = &issi->buffer[ISSI_BLINK_INDEX(frame, x[i], y[i])];
            *reg ^= (-state[i] ^ *reg) & (1 << (x[i] % 8));
        }
        if(update){
            issi_update_frame(issi, frame);
        }
    }
}

void issi_set_blink_all(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, bool state, bool update){
    ISSI_ASSERT(issi != NULL && x != NULL && y != NULL);
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        uint8_t *reg = &issi->buffer[ISSI_BLINK_INDEX(frame, x[i], y[i])];
        *reg ^= (-state ^ *reg) & (1 << (x[i] % 8));
    }
    if(update){
        issi_update_frame(issi, frame);
    }
}

void issi_set_pwm(struct is31fl3731_dev *issi, uint8_t frame, uint8_t x, uint8_t y, uint8_t val, bool update){
    ISSI_ASSERT(issi != NULL);
    issi->buffer[ISSI_PWM_INDEX(frame, x, y)] = val;
}

void issi_set_pwm_group(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, uint8_t *val, bool update){
    ISSI_ASSERT(issi != NULL && x != NULL && y != NULL && val != NULL);
    if(val != NULL){
        for(int i = 0; i < ISSI_LED_COUNT; i++){
            issi->buffer[ISSI_PWM_INDEX(frame, x[i], y[i])] = val[i];
        }
        if(update){
            issi_update_frame(issi, frame);
        }
    }
}

void issi_set_pwm_all(struct is31fl3731_dev *issi, uint8_t frame, uint8_t *x, uint8_t *y, uint8_t val, bool update){
    ISSI_ASSERT(issi != NULL && x != NULL && y != NULL);
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        issi->buffer[ISSI_PWM_INDEX(frame, x[i], y[i])] = val;
    }
    if(update){
        issi_update_frame(issi, frame);
    }
}

void issi_set_function(struct is31fl3731_dev *issi, enum issi_func func, uint8_t val, bool update){
    ISSI_ASSERT(issi != NULL);
    issi->buffer[ISSI_FUNC_OFFSET + func] = val;
    if(update){
        issi_update_functions(issi, func, 1);
    }
}

void issi_set_functions(struct is31fl3731_dev *issi, enum issi_func *funcs, uint8_t *vals, uint8_t n_funcs, bool update){
    ISSI_ASSERT(issi != NULL && funcs != NULL && vals != NULL);
    for(int i = 0; i < n_funcs; i++){
        issi->buffer[ISSI_FUNC_OFFSET + funcs[i]] = vals[i];
    }
    if(update){
        issi_update_functions(issi, ISSI_FUNC_CONFIG, ISSI_FUNCTIONS); // There aren't many functions, so might as well just update them all
    }
}
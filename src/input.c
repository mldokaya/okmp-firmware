#include "input.h"
#include "freertos.h"
#include "tusb.h"

#define TIMEOUT_MS 5000

static encoder_status update_encoder(rotary_encoder *encoder);

const unsigned long col_pins[N_COLS] = {COL0_Pin, COL1_Pin, COL2_Pin, COL3_Pin};

const uint8_t DEFAULT_KEYCODES[N_KEYS] = {
    // 0x04, 0x08, 0x0C, 0x10,
    // 0x05, 0x09, 0x0D, 0x11,
    // 0x06, 0x0A, 0x0E, 0x12,
    // 0x07, 0x0B, 0x0F, 0x13
    0x24, 0x21, 0x1E, 0x2C,
    0x25, 0x22, 0x1F, 0x27,
    0x26, 0x23, 0x20, 0x2E,
    0x54, 0x55, 0x56, 0x57
    // 0x5F, 0x60, 0x61, 0x38,
    // 0x5C, 0x5D, 0x5E, 0x55,
    // 0x59, 0x5A, 0x5B, 0x56,
    // 0x2C, 0x62, 0x63, 0x57
};

void input_init(Key *keys, struct key_report *report, rotary_encoder *re, const uint8_t *keycodes){
    // Initialize the key report
    for(int i = 0; i < N_KEYS; i++){
        report->report[i] = 0;
    }
    report->modifier = &(report->report[0]);
    report->bitmap = &(report->report[1]);
    // Initialize the key matrix
    if(keycodes == NULL){
        keycodes = DEFAULT_KEYCODES;
    }
    for(int i = 0; i < N_KEYS; i++){
        keys[i].keycode = keycodes[i];
        keys[i].bitmap_pos = keycodes[i] >> 3;
        keys[i].bitmap_val = 1 << (keycodes[i] & 0x07);
        keys[i].count = 0;
        keys[i].pressed = false;
    }
    // Initialize the rotary encoder
    re->sw_state = 0;
    re->pos = 0;
    re->raw = 0;
    re->sw_pressed = false;
    re->port_AB = GPIOB;
    re->port_SW = GPIOA;
    re->pin_A = RE_A_Pin;
    re->pin_B = RE_B_Pin;
    re->pin_SW = RE_SW_Pin;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}


void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;
}



void task_input_update(void *argument){
    osMessageQueueId_t *id = (osMessageQueueId_t *)argument;
    rotary_encoder encoder;
    Key keys[N_KEYS];
    struct key_report report;
    input_init(keys, &report, &encoder, NULL);
    encoder_event event;
    event.mode = 0;
    int timeout = 0;
    bool active = true;
    while(1){
        if(input_update_keys(keys)){
            input_usb_update(keys, &report);
            tud_hid_report(0, &report.report, sizeof(report.report));
        }
        encoder_status status = update_encoder(&encoder);
        if(status & RE_SW_CHANGE && encoder.sw_pressed){
            encoder.mode++;
            encoder.mode %= N_OKMP_MODES;
            event.mode = encoder.mode;
            event.action = ENCODER_SW_PRESSED;
            if(active){
                osMessageQueuePut(*id, (void *)&event, 0, 0);
            }
            else{
                display_event d_event = {.mode = event.mode, .val = 0};
                osMessageQueuePut(display_queue_id, (void *)&d_event, 0, 0);
            }
            active = true;
            timeout = 0;
        }
        if(status & RE_POS_CHANGE){
            if(encoder.dir == ENCODER_DIR_CW){
                event.action = ENCODER_CW;
            }
            else{
                event.action = ENCODER_CCW;
            }
            if(active){
                osMessageQueuePut(*id, (void *)&event, 0, 0);
            }
            else{
                display_event d_event = {.mode = MODE_WAKEUP, .val = 0};
                osMessageQueuePut(display_queue_id, (void *)&d_event, 0, 0);
            }
            active = true;
            timeout = 0;
        }
        if(timeout >= TIMEOUT_MS && active){
            active = false;
        }
        timeout++;
        osDelay(1);
    }
}

void input_usb_update(Key *keys, struct key_report *report){
    for(int i = 0; i < N_KEYS; i++){
        report->bitmap[keys[i].bitmap_pos] ^= (-keys[i].pressed ^ report->bitmap[keys[i].bitmap_pos]) & keys[i].bitmap_val;
    }
}

int input_update_keys(Key *keys){
    int idx = 0;
    int status = 0;
    for(int i = 0; i < N_COLS; i++){
        COL_GPIO_PORT->BSRR = col_pins[i];
        uint8_t key_state = (uint8_t)(ROW_GPIO_PORT->IDR & ROW_PIN_MASK);
        for(int j = N_ROWS - 1; j >= 0; j--){
            if(key_state & (1 << (ROW_PIN_OFFSET + j))){
                if(!keys[idx].pressed){
                    keys[idx].count++;
                }
            }
            else if(keys[idx].pressed){
                keys[idx].count++;
            }
            if(keys[idx].count == PUSH_THRESHOLD){
                keys[idx].pressed = !keys[idx].pressed;
                keys[idx].count = 0;
                status = 1;
            }
            idx++;
        }
        COL_GPIO_PORT->BSRR |= (uint32_t)col_pins[i] << 16;
    }
    return status;
}

#define MASK 0b11000111

static void update_sw(uint8_t *history){
    *history = *history << 1;
    *history |= (RE_SW_GPIO_Port->IDR & RE_SW_Pin) == 0;
}

static uint8_t is_button_pressed(uint8_t *history){
    uint8_t pressed = 0;
    if((*history & MASK) == 0b00000111){
        pressed = 1;
        *history = 0b11111111;
    }
    return pressed;
}

static uint8_t is_button_released(uint8_t *history){
    uint8_t released = 0;
    if((*history & MASK) == 0b11000000){
        released = 1;
        *history = 0b00000000;
    }
    return released;
}

static encoder_status update_encoder(rotary_encoder *encoder){
    encoder_status status = RE_NO_CHANGE;
    update_sw(&encoder->sw_state);
    if(is_button_pressed(&encoder->sw_state) && !encoder->sw_pressed){
        encoder->sw_pressed = true;
        status = RE_SW_CHANGE;
    }
    else{
        encoder->sw_pressed = false;
    }
    // if(is_button_released(&encoder->sw_state) && encoder->sw_pressed){
    //     encoder->sw_pressed = false;
    //     status = RE_SW_CHANGE;
    // }
    // encoder->sw_state <<= 1;
    // encoder->sw_state |= (encoder->port_SW->IDR & encoder->pin_SW) == 0;
    // if(encoder->sw_state == 0xFFFF && !encoder->sw_pressed){
    //     encoder->sw_pressed = true;
    //     status += RE_SW_CHANGE;
    // }
    // else if(encoder->sw_state == 0x0000 && encoder->sw_pressed){
    //     encoder->sw_pressed = false;
    //     status += RE_SW_CHANGE;
    // }
    // if(~sw_state & encoder->pin_SW){
    //     if(!encoder->sw_pressed){
    //         encoder->sw_count++;
    //     }
    // }
    // else if(encoder->sw_pressed){
    //     encoder->sw_count++;
    // }
    // if(encoder->sw_count == SW_PUSH_THRESHOLD){
    //     encoder->sw_pressed = !encoder->sw_pressed;
    //     encoder->sw_count = 0;
    //     status += RE_SW_CHANGE;
    // }
    uint16_t state = (uint16_t)(LL_GPIO_ReadInputPort(encoder->port_AB) & RE_PIN_MASK);
    if(encoder->old_state == state){
        return status;
    }
    if(((state >> 1) & 1) ^ (encoder->old_state >> 10)){
        encoder->raw++;
        encoder->dir = ENCODER_DIR_CW;
    }
    else{
        encoder->raw--;
        encoder->dir = ENCODER_DIR_CCW;
    }
    if(encoder->pos != encoder->raw / 4){
        status += RE_POS_CHANGE;
    }
    encoder->pos = encoder->raw / 4;
    encoder->old_state = state;
    return status;
}

// int input_update_mode(omp_mode *mode, uint8_t val){
//     switch(*mode){
//         case MODE_LED_DISPLAY:
//             break;
//         case MODE_LED_PWM:
//             break;
//         default:
//             break;
//     }
// }

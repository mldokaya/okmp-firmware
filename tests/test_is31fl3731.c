#include "unity.h"
#include "is31fl3731.h"
#include <string.h>
#include <stdio.h>

static is31fl3731_dev issi;
static uint8_t x;
static uint8_t y;
static uint8_t val;
static bool state;
static uint8_t x_arr[ISSI_LED_COUNT];
static uint8_t y_arr[ISSI_LED_COUNT];
static uint8_t val_arr[ISSI_LED_COUNT];
static bool state_arr[ISSI_LED_COUNT];
static int mock_write_single_calls;
static int mock_write_buf_calls;

int mock_write_single(void *i2c, uint8_t reg, uint8_t byte){
    mock_write_single_calls++;
}

int mock_write_buf(void *i2c, uint8_t reg, uint8_t *buf, uint8_t n_bytes){
    mock_write_buf_calls++;
}

void setUp(){
    mock_write_single_calls = 0;
    mock_write_buf_calls = 0;
    x = 0;
    y = 0;
    val = 0;
    state = true;
    issi.write_single = mock_write_single;
    issi.write_buf = mock_write_buf;
}

void tearDown(){}

void test_issi_init_frame_null_dev(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_init_frame(NULL, ISSI_FRAME1, NULL));
}

void test_issi_init_frame_invalid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_init_frame(&issi, ISSI_FRAMES, NULL)); // Frame value is out of range
}

void test_issi_init_frame_null_state(void){
    memset(issi.buffer, 0xFF, ISSI_FRAMES * ISSI_BYTES_PER_FRAME); // Set all buffer values to 0xFF
    TEST_ASSERT_EQUAL(ISSI_OK, issi_init_frame(&issi, ISSI_FRAME1, NULL)); // Verify success with valid parameters
    TEST_ASSERT_EACH_EQUAL_UINT8(0, issi.buffer, ISSI_BYTES_PER_FRAME); // Verify frame 1 was zeroed

    // Repeat with a different frame
    TEST_ASSERT_EQUAL(ISSI_OK, issi_init_frame(&issi, ISSI_FRAME8, NULL));
    TEST_ASSERT_EACH_EQUAL_UINT8(0, &issi.buffer[ISSI_FRAME8 * ISSI_BYTES_PER_FRAME], ISSI_BYTES_PER_FRAME);
}

void test_issi_init_frame_with_state(void){
    uint8_t frame_state[ISSI_BYTES_PER_FRAME];
    memset(frame_state, 0x01, sizeof(frame_state)); // Generate a non-zero state
    TEST_ASSERT_EQUAL(ISSI_OK, issi_init_frame(&issi, ISSI_FRAME1, frame_state)); // Verify success with non-NULL state
    TEST_ASSERT_EACH_EQUAL_UINT8(0x01, issi.buffer, ISSI_BYTES_PER_FRAME); // Verify state of frame 1 matches given state

    memset(frame_state, 0x02, sizeof(frame_state)); // Generate a different non-zero state

    // Repeat with a different frame and state
    TEST_ASSERT_EQUAL(ISSI_OK, issi_init_frame(&issi, ISSI_FRAME8, frame_state));
    TEST_ASSERT_EACH_EQUAL_UINT8(0x02, &issi.buffer[ISSI_FRAME8 * ISSI_BYTES_PER_FRAME], ISSI_BYTES_PER_FRAME);
}

void test_issi_switch_page_null_dev(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_switch_page(NULL, ISSI_FRAME1));
}

void test_issi_switch_page_invalid_page(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_switch_page(&issi, ISSI_FRAME8+1));
}

void test_issi_switch_page_current_page(void){
    issi.current_page = ISSI_FUNCTION;
    
    TEST_ASSERT_EQUAL(ISSI_OK, issi_switch_page(&issi, ISSI_FUNCTION));
    TEST_ASSERT_EQUAL(0, mock_write_single_calls);
}

void test_issi_switch_page_different_page(void){
    issi.current_page = ISSI_FUNCTION;
    
    TEST_ASSERT_EQUAL(ISSI_OK, issi_switch_page(&issi, ISSI_FRAME1));
    TEST_ASSERT_EQUAL(1, mock_write_single_calls);
}

void test_issi_update_frame_null_dev(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_update_frame(NULL, ISSI_FRAME1));
}

void test_issi_update_frame_invalid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_update_frame(&issi, ISSI_FUNCTION));
}

// TODO: Make this test better or add more
void test_issi_update_frame_valid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_OK, issi_update_frame(&issi, ISSI_FRAME1));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}


void test_issi_update_leds_null_dev(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_update_leds(NULL, ISSI_FRAME1, ISSI_CA1, 1));
}

void test_issi_update_leds_invalid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_update_leds(&issi, ISSI_FUNCTION, ISSI_CA1, 1));
}

void test_issi_update_leds_invalid_reg(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_update_leds(&issi, ISSI_FRAME1, ISSI_CB9+1, 1));
}

void test_issi_update_leds_valid_params(void){
    TEST_ASSERT_EQUAL(ISSI_OK, issi_update_leds(&issi, ISSI_FRAME1, ISSI_CA1, 1));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}

void test_issi_update_functions_null_dev(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_update_functions(NULL, ISSI_FUNC_DISPLAY, 1));
}

void test_issi_update_functions_invalid_func(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_update_functions(&issi, ISSI_FUNC_ADC+1, 1));
}

void test_issi_update_functions_valid_params(void){
    TEST_ASSERT_EQUAL(ISSI_OK, issi_update_functions(&issi, ISSI_FUNC_DISPLAY, 1));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}

void test_issi_set_led_null_dev(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led(NULL, ISSI_FRAME1, 0, 0, true, true));
}

void test_issi_set_led_invalid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led(NULL, ISSI_FUNCTION, 0, 0, true, true));
}

void test_issi_set_led_invalid_pos(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led(NULL, ISSI_FRAME1, ISSI_XMAX+1, ISSI_YMAX+1, true, true));
}

void test_issi_set_led_group_null_param(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_group(NULL, ISSI_FRAME1, &x, &y, &state, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_group(&issi, ISSI_FRAME1, NULL, &y, &state, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_group(&issi, ISSI_FRAME1, &x, NULL, &state, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_group(&issi, ISSI_FRAME1, &x, &y, NULL, true));
}

void test_issi_set_led_group_invalid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_group(&issi, ISSI_FUNCTION, &x, &y, &state, true));
}

void test_issi_set_led_group_invalid_pos(void){
    x = ISSI_XMAX+1;
    y = ISSI_YMAX+1;
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_group(&issi, ISSI_FRAME1, &x, &y, &state, true));
}

void test_issi_set_led_group_on_off(void){
    memset(issi.buffer, 0, sizeof(issi.buffer));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
        state_arr[i] = true;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_led_group(&issi, ISSI_FRAME1, x_arr, y_arr, state_arr, false));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        TEST_ASSERT_TRUE(issi.buffer[ISSI_LED_INDEX(ISSI_FRAME1, x_arr[i], y_arr[i])] & 1 << (x_arr[i] % 8));
        state_arr[i] = false;
    }
    memset(issi.buffer, 0xFF, sizeof(issi.buffer));
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_led_group(&issi, ISSI_FRAME8, x_arr, y_arr, state_arr, false));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        TEST_ASSERT_FALSE(issi.buffer[ISSI_LED_INDEX(ISSI_FRAME8, x_arr[i], y_arr[i])] & 1 << (x_arr[i] % 8));
    }
}

void test_issi_set_led_group_update(void){
    issi.current_page = ISSI_FRAME1;
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
        state_arr[i] = true;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_led_group(&issi, ISSI_FRAME1, x_arr, y_arr, state_arr, true));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_led_group(&issi, ISSI_FRAME1, x_arr, y_arr, state_arr, false));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}

void test_issi_set_led_all_null_param(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_all(NULL, ISSI_FRAME1, x_arr, y_arr, true, false));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_all(&issi, ISSI_FRAME1, NULL, y_arr, true, false));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_all(&issi, ISSI_FRAME1, x_arr, NULL, true, false));
}

void test_issi_set_led_all_invalid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_all(&issi, ISSI_FUNCTION, &x, &y, state, true));
}

void test_issi_set_led_all_invalid_pos(void){
    x = ISSI_XMAX+1;
    y = ISSI_YMAX+1;
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_led_all(&issi, ISSI_FRAME1, &x, &y, state, true));
}

void test_issi_set_led_all_on_off(void){
    memset(issi.buffer, 0, sizeof(issi.buffer));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_led_all(&issi, ISSI_FRAME1, x_arr, y_arr, state, false));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        TEST_ASSERT_TRUE(issi.buffer[ISSI_LED_INDEX(ISSI_FRAME1, x_arr[i], y_arr[i])] & 1 << (x_arr[i] % 8));
    }
    state = false;
    memset(issi.buffer, 0xFF, sizeof(issi.buffer));
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_led_all(&issi, ISSI_FRAME8, x_arr, y_arr, state, false));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        TEST_ASSERT_FALSE(issi.buffer[ISSI_LED_INDEX(ISSI_FRAME8, x_arr[i], y_arr[i])] & 1 << (x_arr[i] % 8));
    }
}

void test_issi_set_led_all_update(void){
    issi.current_page = ISSI_FRAME1;
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_led_all(&issi, ISSI_FRAME1, x_arr, y_arr, state, true));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_led_all(&issi, ISSI_FRAME1, x_arr, y_arr, state, false));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}

void test_issi_set_blink_group_null_param(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_group(NULL, ISSI_FRAME1, &x, &y, &state, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_group(&issi, ISSI_FRAME1, NULL, &y, &state, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_group(&issi, ISSI_FRAME1, &x, NULL, &state, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_group(&issi, ISSI_FRAME1, &x, &y, NULL, true));
}

void test_issi_set_blink_group_invalid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_group(&issi, ISSI_FUNCTION, &x, &y, &state, true));
}

void test_issi_set_blink_group_invalid_pos(void){
    x = ISSI_XMAX+1;
    y = ISSI_YMAX+1;
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_group(&issi, ISSI_FRAME1, &x, &y, &state, true));
}

void test_issi_set_blink_group_on_off(void){
    memset(issi.buffer, 0, sizeof(issi.buffer));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
        state_arr[i] = true;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_blink_group(&issi, ISSI_FRAME1, x_arr, y_arr, state_arr, false));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        TEST_ASSERT_TRUE(issi.buffer[ISSI_BLINK_INDEX(ISSI_FRAME1, x_arr[i], y_arr[i])] & 1 << (x_arr[i] % 8));
        state_arr[i] = false;
    }
    memset(issi.buffer, 0xFF, sizeof(issi.buffer));
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_blink_group(&issi, ISSI_FRAME8, x_arr, y_arr, state_arr, false));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        TEST_ASSERT_FALSE(issi.buffer[ISSI_BLINK_INDEX(ISSI_FRAME8, x_arr[i], y_arr[i])] & 1 << (x_arr[i] % 8));
    }
}

void test_issi_set_blink_group_update(void){
    issi.current_page = ISSI_FRAME1;
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
        state_arr[i] = true;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_blink_group(&issi, ISSI_FRAME1, x_arr, y_arr, state_arr, true));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_blink_group(&issi, ISSI_FRAME1, x_arr, y_arr, state_arr, false));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}

void test_issi_set_blink_all_null_param(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_all(NULL, ISSI_FRAME1, x_arr, y_arr, true, false));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_all(&issi, ISSI_FRAME1, NULL, y_arr, true, false));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_all(&issi, ISSI_FRAME1, x_arr, NULL, true, false));
}

void test_issi_set_blink_all_invalid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_all(&issi, ISSI_FUNCTION, &x, &y, state, true));
}

void test_issi_set_blink_all_invalid_pos(void){
    x = ISSI_XMAX+1;
    y = ISSI_YMAX+1;
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_blink_all(&issi, ISSI_FRAME1, &x, &y, state, true));
}

void test_issi_set_blink_all_on_off(void){
    memset(issi.buffer, 0, sizeof(issi.buffer));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_blink_all(&issi, ISSI_FRAME1, x_arr, y_arr, state, false));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        TEST_ASSERT_TRUE(issi.buffer[ISSI_BLINK_INDEX(ISSI_FRAME1, x_arr[i], y_arr[i])] & 1 << (x_arr[i] % 8));
    }
    state = false;
    memset(issi.buffer, 0xFF, sizeof(issi.buffer));
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_blink_all(&issi, ISSI_FRAME8, x_arr, y_arr, state, false));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        TEST_ASSERT_FALSE(issi.buffer[ISSI_BLINK_INDEX(ISSI_FRAME8, x_arr[i], y_arr[i])] & 1 << (x_arr[i] % 8));
    }
}

void test_issi_set_blink_all_update(void){
    issi.current_page = ISSI_FRAME1;
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_blink_all(&issi, ISSI_FRAME1, x_arr, y_arr, state, true));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_blink_all(&issi, ISSI_FRAME1, x_arr, y_arr, state, false));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}

void test_issi_set_pwm_group_null_param(void){
    val = 0;
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_group(NULL, ISSI_FRAME1, &x, &y, &val, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_group(&issi, ISSI_FRAME1, NULL, &y, &val, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_group(&issi, ISSI_FRAME1, &x, NULL, &val, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_group(&issi, ISSI_FRAME1, &x, &y, NULL, true));
}

void test_issi_set_pwm_group_invalid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_group(&issi, ISSI_FUNCTION, &x, &y, &val, true));
}

void test_issi_set_pwm_group_invalid_pos(void){
    x = ISSI_XMAX+1;
    y = ISSI_YMAX+1;
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_group(&issi, ISSI_FRAME1, &x, &y, &val, true));
}

void test_issi_set_pwm_group_change(void){
    memset(issi.buffer, 0, sizeof(issi.buffer));
    issi.current_page = ISSI_FRAME1;
    issi.write_buf = mock_write_buf;
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
        val_arr[i] = i;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_pwm_group(&issi, ISSI_FRAME1, x_arr, y_arr, val_arr, false));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        TEST_ASSERT_TRUE(issi.buffer[ISSI_PWM_INDEX(ISSI_FRAME1, x_arr[i], y_arr[i])] == val_arr[i]);
    }
}

void test_issi_set_pwm_group_update(void){
    issi.current_page = ISSI_FRAME1;
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
        val_arr[i] = i;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_pwm_group(&issi, ISSI_FRAME1, x_arr, y_arr, val_arr, true));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_pwm_group(&issi, ISSI_FRAME1, x_arr, y_arr, val_arr, false));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}

void test_issi_set_pwm_all_null_param(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_all(NULL, ISSI_FRAME1, &x, &y, val, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_all(&issi, ISSI_FRAME1, NULL, &y, val, true));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_all(&issi, ISSI_FRAME1, &x, NULL, val, true));
}

void test_issi_set_pwm_all_invalid_frame(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_all(&issi, ISSI_FUNCTION, &x, &y, val, true));
}

void test_issi_set_pwm_all_invalid_pos(void){
    x = ISSI_XMAX+1;
    y = ISSI_YMAX+1;
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_pwm_all(&issi, ISSI_FRAME1, &x, &y, val, true));
}

void test_issi_set_pwm_all_change(void){
    memset(issi.buffer, 0, sizeof(issi.buffer));
    val = 0xFF;
    issi.current_page = ISSI_FRAME1;
    issi.write_buf = mock_write_buf;
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_pwm_all(&issi, ISSI_FRAME1, x_arr, y_arr, val, false));
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        TEST_ASSERT_TRUE(issi.buffer[ISSI_PWM_INDEX(ISSI_FRAME1, x_arr[i], y_arr[i])] == val);
    }
}

void test_issi_set_pwm_all_update(void){
    val = 0xFF;
    issi.current_page = ISSI_FRAME1;
    issi.write_buf = mock_write_buf;
    for(int i = 0; i < ISSI_LED_COUNT; i++){
        x_arr[i] = i % ISSI_XMAX;
        y_arr[i] = i % ISSI_YMAX;
    }
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_pwm_all(&issi, ISSI_FRAME1, x_arr, y_arr, val, true));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_pwm_all(&issi, ISSI_FRAME1, x_arr, y_arr, val, false));
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}

void test_issi_set_function_null_dev(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_function(NULL, ISSI_FUNC_DISPLAY, 0, true));
}

void test_issi_set_function_invalid_func(void){
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_function(&issi, ISSI_FUNC_ADC+1, 0, true));
}

void test_issi_set_function_valid_func(void){
    memset(issi.buffer, 0, sizeof(issi.buffer));
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_function(&issi, ISSI_FUNC_DISPLAY, 0xFF, false));
    TEST_ASSERT_EQUAL_UINT8(0xFF, issi.buffer[ISSI_FUNC_OFFSET + ISSI_FUNC_DISPLAY]);
}

void test_issi_set_function_update(void){
    issi.current_page = ISSI_FUNCTION;
    issi.write_buf = mock_write_buf;
    memset(issi.buffer, 0, sizeof(issi.buffer));
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_function(&issi, ISSI_FUNC_DISPLAY, 0xFF, false));
    TEST_ASSERT_EQUAL_UINT8(0xFF, issi.buffer[ISSI_FUNC_OFFSET + ISSI_FUNC_DISPLAY]);
    TEST_ASSERT_EQUAL(0, mock_write_buf_calls);
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_function(&issi, ISSI_FUNC_DISPLAY, 0x00, true));
    TEST_ASSERT_EQUAL_UINT8(0x00, issi.buffer[ISSI_FUNC_OFFSET + ISSI_FUNC_DISPLAY]);
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}

void test_issi_set_functions_null_param(void){
    enum issi_func func;
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_functions(NULL, &func, &val, 1, false));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_functions(&issi, NULL, &val, 1, false));
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_functions(&issi, &func, NULL, 1, false));
}

void test_issi_set_functions_invalid_func(void){
    enum issi_func func = ISSI_FUNC_ADC+1;
    TEST_ASSERT_EQUAL(ISSI_ERROR_INVALID_PARAM, issi_set_functions(&issi, &func, &val, 1, false));
}

void test_issi_set_functions_zero_funcs(void){
    enum issi_func func = ISSI_FUNC_ADC;
    val = 0xFF;
    memset(issi.buffer, 0, sizeof(issi.buffer));
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_functions(&issi, &func, &val, 0, false));
    TEST_ASSERT_EQUAL_UINT8(0, issi.buffer[ISSI_FUNC_OFFSET + ISSI_FUNC_ADC]);
}

void test_issi_set_functions_multiple(void){
    enum issi_func func[4] = {ISSI_FUNC_DISPLAY, ISSI_FUNC_AUTOPLAY1, ISSI_FUNC_AUDIO_SYNC, ISSI_FUNC_ADC};
    uint8_t val[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    memset(issi.buffer, 0, sizeof(issi.buffer));
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_functions(&issi, func, val, 4, false));
    for(int i = 0; i < 4; i++){
        TEST_ASSERT_EQUAL_UINT8(val[i], issi.buffer[ISSI_FUNC_OFFSET + func[i]]);
    }
}

void test_issi_set_functions_update(void){
    enum issi_func func[4] = {ISSI_FUNC_DISPLAY, ISSI_FUNC_AUTOPLAY1, ISSI_FUNC_AUDIO_SYNC, ISSI_FUNC_ADC};
    uint8_t val[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    issi.write_buf = mock_write_buf;
    memset(issi.buffer, 0, sizeof(issi.buffer));
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_functions(&issi, func, val, 4, false));
    TEST_ASSERT_EQUAL(0, mock_write_buf_calls);
    memset(val, 0, 4);
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_functions(&issi, func, val, 4, true));
    for(int i = 0; i < 4; i++){
        TEST_ASSERT_EQUAL_UINT8(val[i], issi.buffer[ISSI_FUNC_OFFSET + func[i]]);
    }
    TEST_ASSERT_EQUAL(1, mock_write_buf_calls);
}

void test_issi_set_functions_all(void){
    enum issi_func funcs[ISSI_FUNCTIONS];
    uint8_t val_arr[ISSI_FUNCTIONS];
    for(int i = 0; i < ISSI_FUNCTIONS; i++){
        funcs[i] = i;
        val_arr[i] = i;
    }
    memset(issi.buffer, 0, sizeof(issi.buffer));
    TEST_ASSERT_EQUAL(ISSI_OK, issi_set_functions(&issi, funcs, val_arr, ISSI_FUNCTIONS, false));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(val_arr, &issi.buffer[ISSI_FUNC_OFFSET], ISSI_FUNCTION);
}

int main(void){
    UNITY_BEGIN();

    // issi_init_frame
    RUN_TEST(test_issi_init_frame_null_dev);
    RUN_TEST(test_issi_init_frame_invalid_frame);
    RUN_TEST(test_issi_init_frame_null_state);
    RUN_TEST(test_issi_init_frame_with_state);

    // issi_switch_page
    RUN_TEST(test_issi_switch_page_null_dev);
    RUN_TEST(test_issi_switch_page_invalid_page);
    RUN_TEST(test_issi_switch_page_current_page);
    RUN_TEST(test_issi_switch_page_different_page);

    // issi_update_frame
    RUN_TEST(test_issi_update_frame_null_dev);
    RUN_TEST(test_issi_update_frame_invalid_frame);
    RUN_TEST(test_issi_update_frame_valid_frame);

    // issi_update_leds
    RUN_TEST(test_issi_update_leds_null_dev);
    RUN_TEST(test_issi_update_leds_invalid_frame);
    RUN_TEST(test_issi_update_leds_invalid_reg);
    RUN_TEST(test_issi_update_leds_valid_params);

    // issi_update_functions
    RUN_TEST(test_issi_update_functions_null_dev);
    RUN_TEST(test_issi_update_functions_invalid_func);
    RUN_TEST(test_issi_update_functions_valid_params);

    // issi_set_led_group
    RUN_TEST(test_issi_set_led_group_null_param);
    RUN_TEST(test_issi_set_led_group_invalid_frame);
    RUN_TEST(test_issi_set_led_group_invalid_pos);
    RUN_TEST(test_issi_set_led_group_on_off);
    RUN_TEST(test_issi_set_led_group_update);

    // issi_set_led_all
    RUN_TEST(test_issi_set_led_all_null_param);
    RUN_TEST(test_issi_set_led_all_invalid_frame);
    RUN_TEST(test_issi_set_led_all_invalid_pos);
    RUN_TEST(test_issi_set_led_all_on_off);
    RUN_TEST(test_issi_set_led_all_update);

    // issi_set_blink_group
    RUN_TEST(test_issi_set_blink_group_null_param);
    RUN_TEST(test_issi_set_blink_group_invalid_frame);
    RUN_TEST(test_issi_set_blink_group_invalid_pos);
    RUN_TEST(test_issi_set_blink_group_on_off);
    RUN_TEST(test_issi_set_blink_group_update);

    // issi_set_blink_all
    RUN_TEST(test_issi_set_blink_all_null_param);
    RUN_TEST(test_issi_set_blink_all_invalid_frame);
    RUN_TEST(test_issi_set_blink_all_invalid_pos);
    RUN_TEST(test_issi_set_blink_all_on_off);
    RUN_TEST(test_issi_set_blink_all_update);

    // issi_set_pwm_group
    RUN_TEST(test_issi_set_pwm_group_null_param);
    RUN_TEST(test_issi_set_pwm_group_invalid_frame);
    RUN_TEST(test_issi_set_pwm_group_invalid_pos);
    RUN_TEST(test_issi_set_pwm_group_change);
    RUN_TEST(test_issi_set_pwm_group_update);

    // issi_set_pwm_all
    RUN_TEST(test_issi_set_pwm_all_null_param);
    RUN_TEST(test_issi_set_pwm_all_invalid_frame);
    RUN_TEST(test_issi_set_pwm_all_invalid_pos);
    RUN_TEST(test_issi_set_pwm_all_change);
    RUN_TEST(test_issi_set_pwm_all_update);

    // issi_set_function
    RUN_TEST(test_issi_set_function_null_dev);
    RUN_TEST(test_issi_set_function_invalid_func);
    RUN_TEST(test_issi_set_function_valid_func);
    RUN_TEST(test_issi_set_function_update);

    // issi_set_functions
    RUN_TEST(test_issi_set_functions_null_param);
    RUN_TEST(test_issi_set_functions_invalid_func);
    RUN_TEST(test_issi_set_functions_zero_funcs);
    RUN_TEST(test_issi_set_functions_multiple);
    RUN_TEST(test_issi_set_functions_update);
    RUN_TEST(test_issi_set_functions_all);
    return UNITY_END();
}
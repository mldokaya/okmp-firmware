#include "unity.h"
#include "sh1106.h"
#include <string.h>

// TODO: Add more tests

static struct sh1106_dev sh1106;
static int mock_reset_calls;
static int mock_write_calls;
static uint8_t data;
static uint8_t cmd;

void mock_reset(){
    mock_reset_calls++;
}

void mock_write(struct sh1106_dev *sh1106, uint8_t *data, const uint8_t n_bytes, bool a0){
    mock_write_calls++;
}

void setUp(){
    mock_reset_calls = 0;
    mock_write_calls = 0;
    data = 0;
}

void tearDown(){

}

void test_sh1106_init_null_dev(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_init(NULL));
}

void test_sh1106_init_valid_dev(void){
    sh1106.reset = mock_reset;
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_init(&sh1106));
    TEST_ASSERT_EQUAL(1, mock_reset_calls);
}

void test_sh1106_send_data_null_params(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_send_data(NULL, &data, 1));
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_send_data(&sh1106, NULL, 1));
}

void test_sh1106_send_data_valid_data(void){
    sh1106.write = mock_write;
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_send_data(&sh1106, &data, 1));
    TEST_ASSERT_EQUAL(1, mock_write_calls);
}

void test_sh1106_send_cmd_null_dev(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_send_cmd(NULL, 0));
}

void test_sh1106_send_cmd_valid_cmd(void){
    sh1106.write = mock_write;
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_send_cmd(&sh1106, SH1106_SET_DISPLAY));
    TEST_ASSERT_EQUAL(1, mock_write_calls);
}

void test_sh1106_send_cmd_list_null_params(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_send_cmd_list(NULL, &cmd, 1));
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_send_cmd_list(&sh1106, NULL, 1));
}

void test_sh1106_send_cmd_list_valid_params(void){
    sh1106.write = mock_write;
    cmd = SH1106_SET_DISPLAY;
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_send_cmd_list(&sh1106, &cmd, 1));
    TEST_ASSERT_EQUAL(1, mock_write_calls);
}

void test_sh1106_set_page_null_dev(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_set_page(NULL, 0));
}

void test_sh1106_set_page_invalid_page(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_set_page(&sh1106, 10));
}

void test_sh1106_set_page_valid_page(void){
    sh1106.write = mock_write;
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_set_page(&sh1106, 0));
    TEST_ASSERT_EQUAL(1, mock_write_calls);
}

void test_sh1106_set_col_null_dev(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_set_col(NULL, 0));
}

void test_sh1106_set_col_invalid_col(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_set_page(&sh1106, 200));
}

void test_sh1106_set_col_valid_col(void){
    sh1106.write = mock_write;
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_set_col(&sh1106, 0));
    TEST_ASSERT_EQUAL(2, mock_write_calls);
}

void test_sh1106_clear_null_dev(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_clear(NULL));
}

void test_sh1106_clear_full_buffer(void){
    memset(sh1106.buffer, 0xFF, SH1106_BYTES);
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_clear(&sh1106));
    TEST_ASSERT_EACH_EQUAL_UINT8(0x00, sh1106.buffer, SH1106_BYTES);
}

void test_sh1106_fill_null_dev(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_fill(NULL));
}

void test_sh1106_fill_empty_buffer(void){
    memset(sh1106.buffer, 0x00, SH1106_BYTES);
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_fill(&sh1106));
    TEST_ASSERT_EACH_EQUAL_UINT8(0xFF, sh1106.buffer, SH1106_BYTES);
}

void test_sh1106_update_display_null_dev(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_update_display(NULL));
}

void test_sh1106_update_display_valid_dev(void){
    sh1106.write = mock_write;
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_update_display(&sh1106));
    TEST_ASSERT_EQUAL(4, mock_write_calls);
}

void test_sh1106_update_region_null_dev(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_update_region(NULL, 0, 0, 1, 1));
}

void test_sh1106_update_region_invalid_pos(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_update_region(&sh1106, 200, 200, 1, 1));
}

void test_sh1106_update_region_single_page(void){
    sh1106.write = mock_write;
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_update_region(&sh1106, 0, 0, 1, 1));
    TEST_ASSERT_EQUAL(1 * 4, mock_write_calls);
}

void test_sh1106_update_region_multiple_pages(void){
    sh1106.write = mock_write;
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_update_region(&sh1106, 0, 0, 1, 20)); // Should be updating 3 pages
    TEST_ASSERT_EQUAL(3 * 4, mock_write_calls);
}

void test_sh1106_clear_region_null_dev(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_clear_region(NULL, 0, 0, 1, 1, false));
}

void test_sh1106_clear_region_invalid_pos(void){
    TEST_ASSERT_EQUAL(SH1106_ERROR_INVALID_PARAM, sh1106_clear_region(&sh1106, 200, 200, 1, 1, false));
}

void test_sh1106_clear_region_single_page_no_update(void){
    sh1106.write = mock_write;
    memset(sh1106.buffer, 0xFF, SH1106_BYTES);
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_clear_region(&sh1106, 0, 0, 1, 3, false));
    TEST_ASSERT_EQUAL(0, mock_write_calls);
    TEST_ASSERT_EQUAL(0x00, sh1106.buffer[0]);
    TEST_ASSERT_EACH_EQUAL_UINT8(0xFF, &sh1106.buffer[1], SH1106_BYTES-1); // Every other bytes should still be 0xFF
}

void test_sh1106_clear_region_multiple_pages_no_update(void){
    sh1106.write = mock_write;
    memset(sh1106.buffer, 0xFF, SH1106_BYTES);
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_clear_region(&sh1106, 0, 0, 1, 9, false));
    TEST_ASSERT_EQUAL(0, mock_write_calls);
    TEST_ASSERT_EQUAL(0x00, sh1106.buffer[0]);
    TEST_ASSERT_EQUAL(0x00, sh1106.buffer[128]);
    TEST_ASSERT_EACH_EQUAL_UINT8(0xFF, &sh1106.buffer[1], 127); // Every other bytes should still be 0xFF
    TEST_ASSERT_EACH_EQUAL_UINT8(0xFF, &sh1106.buffer[129], SH1106_BYTES-129);
}

void test_sh1106_clear_region_multiple_cols_no_update(void){
    sh1106.write = mock_write;
    memset(sh1106.buffer, 0xFF, SH1106_BYTES);
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_clear_region(&sh1106, 0, 0, 2, 9, false));
    TEST_ASSERT_EQUAL(0, mock_write_calls);
    TEST_ASSERT_EQUAL(0x00, sh1106.buffer[0]);
    TEST_ASSERT_EQUAL(0x00, sh1106.buffer[1]);
    TEST_ASSERT_EQUAL(0x00, sh1106.buffer[128]);
    TEST_ASSERT_EQUAL(0x00, sh1106.buffer[129]);
    TEST_ASSERT_EACH_EQUAL_UINT8(0xFF, &sh1106.buffer[2], 126); // Every other bytes should still be 0xFF
    TEST_ASSERT_EACH_EQUAL_UINT8(0xFF, &sh1106.buffer[130], SH1106_BYTES-130);
}

void test_sh1106_clear_region_update(void){
    sh1106.write = mock_write;
    memset(sh1106.buffer, 0xFF, SH1106_BYTES);
    TEST_ASSERT_EQUAL(SH1106_OK, sh1106_clear_region(&sh1106, 0, 0, 1, 3, true));
    TEST_ASSERT_EQUAL(4, mock_write_calls); // With 1 page being updated mock_write should be called 4 times
    TEST_ASSERT_EQUAL(0x00, sh1106.buffer[0]);
    TEST_ASSERT_EACH_EQUAL_UINT8(0xFF, &sh1106.buffer[1], SH1106_BYTES-1); // Every other bytes should still be 0xFF
}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_sh1106_init_null_dev);
    RUN_TEST(test_sh1106_init_valid_dev);

    RUN_TEST(test_sh1106_send_data_null_params);
    RUN_TEST(test_sh1106_send_data_valid_data);
    RUN_TEST(test_sh1106_send_cmd_null_dev);

    RUN_TEST(test_sh1106_send_cmd_valid_cmd);
    RUN_TEST(test_sh1106_send_cmd_list_null_params);
    RUN_TEST(test_sh1106_send_cmd_list_valid_params);

    RUN_TEST(test_sh1106_set_page_null_dev);
    RUN_TEST(test_sh1106_set_page_invalid_page);
    RUN_TEST(test_sh1106_set_page_valid_page);

    RUN_TEST(test_sh1106_set_col_null_dev);
    RUN_TEST(test_sh1106_set_col_invalid_col);
    RUN_TEST(test_sh1106_set_col_valid_col);

    RUN_TEST(test_sh1106_clear_null_dev);
    RUN_TEST(test_sh1106_clear_full_buffer);

    RUN_TEST(test_sh1106_fill_null_dev);
    RUN_TEST(test_sh1106_fill_empty_buffer);

    RUN_TEST(test_sh1106_update_display_null_dev);
    RUN_TEST(test_sh1106_update_display_valid_dev);
    RUN_TEST(test_sh1106_update_region_null_dev);
    RUN_TEST(test_sh1106_update_region_invalid_pos);
    RUN_TEST(test_sh1106_update_region_single_page);
    RUN_TEST(test_sh1106_update_region_multiple_pages);

    RUN_TEST(test_sh1106_clear_region_null_dev);
    RUN_TEST(test_sh1106_clear_region_invalid_pos);
    RUN_TEST(test_sh1106_clear_region_single_page_no_update);
    RUN_TEST(test_sh1106_clear_region_multiple_pages_no_update);
    RUN_TEST(test_sh1106_clear_region_multiple_cols_no_update);
    RUN_TEST(test_sh1106_clear_region_update);

    return UNITY_END();
}
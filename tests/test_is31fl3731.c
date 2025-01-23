#include "unity.h"
#include "is31fl3731.h"

issi_ctx *issi;

void setUp(){}
void tearDown(){}

void test_unit_test(){
    TEST_ASSERT_EQUAL(1, 1);
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_unit_test);
    return UNITY_END();
}
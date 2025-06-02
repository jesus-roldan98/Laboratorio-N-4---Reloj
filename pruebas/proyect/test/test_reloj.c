#include "unity.h"

union {
struct {
    uint8_t seconds [2];
    uint8_t minutes [2];
    uint8_t hours [2];
} time 
uint8_t bcd [6]
}
void test_set_up_with_invalid_time (void) {
    
    clock_time_t current_time;

    clock_t clok = ClockCreate();
    TEST_ASSERT_FALSE (ClockTimeIsValid (Clock,  &current_time));
    TEST_ASSERT_EACH_EQUAL_UINT8 (0, current_time.bcd, 0);
}
/*********************************************************************************************************************
Copyright (c) 2025, Roldan JesusAlejandro <kechuroldanjesus@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*********************************************************************************************************************/

/** @file test_clock.c
 ** @brief Plantilla para la creación del test del reloj
 **/

/* === Headers files inclusions =============================================================== */

#include "unity.h"
#include "clock.h"

/* === Macros definitions ====================================================================== */

#define CLOCK_TICKS_PER_SECOND 5 // Define el número de ticks del reloj para simular un segundo

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

void SimulatedSeconds(clock_t clock, uint8_t seconds);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

void SimulatedSeconds(clock_t clock, uint8_t seconds) {
    // Simula el avance del reloj en segundos
    for (uint8_t i = 0; i <CLOCK_TICKS_PER_SECOND * seconds; i++) {
        ClockNewTick(clock);
    }

}

/* === Public function implementation ========================================================= */


// Al inicialzar el reloj esta en 00:00 y con hora invalida
void test_set_up_with_invalid_time (void) {
    
    clock_time_t current_time = {
        .bcd = {1, 2, 3, 4, 5, 6},

    };

    clock_t clock = ClockCreate(CLOCK_TICKS_PER_SECOND);
    TEST_ASSERT_FALSE (ClockGetTime(clock,  &current_time));
    TEST_ASSERT_EACH_EQUAL_UINT8 (0, current_time.bcd, 6);
}

// -Al ajustar la hora el reloj con valores correctos, queda en hora y es valida

void test_set_up_with_valid_time (void) {
   static const clock_time_t new_time = {
        .time = {
            .seconds = {4, 5},
            .minutes = {3, 2},
            .hours   = {1, 4}
        }
    };
    
    clock_time_t current_time = {0};

    clock_t clock = ClockCreate(CLOCK_TICKS_PER_SECOND);
    
    TEST_ASSERT_TRUE(ClockSetTime(clock, &new_time));
    TEST_ASSERT_TRUE(ClockGetTime(clock,  &current_time));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(new_time.bcd, current_time.bcd, 6);
}

// Despues de n ciclos el reloj avanza un segundo

void test_clock_advance_one_second (void) {

    clock_time_t current_time = {0};
    static const clock_time_t espected_value = {
        .time = {
            .seconds = {1, 0},
            .minutes = {0, 0},
            .hours   = {0, 0}
        }
    };
    clock_t clock = ClockCreate(CLOCK_TICKS_PER_SECOND);
    
    ClockSetTime(clock, &(clock_time_t){0});
    SimulatedSeconds(clock, 1);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(espected_value.bcd, current_time.bcd, 6);
    TEST_ASSERT_EQUAL_MEMORY(&espected_value, &current_time, sizeof(clock_time_t));

}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */

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
#define TEST_ASSERT_TIME(seconds_units, second_tens, minutes_units, minutes_tens, hours_units, hours_tens, current_time) \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(seconds_units, current_time.bcd[0], "Diference in unit second"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(second_tens, current_time.bcd[1], "Diference in tens second"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(minutes_units, current_time.bcd[2], "Diference in unit minutes"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(minutes_tens, current_time.bcd[3], "Diference in tens minutes"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(hours_units, current_time.bcd[4], "Diference in unit hours"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(hours_tens, current_time.bcd[5], "Diference in tens hours")

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

clock_t clock; // Variable para almacenar el reloj simulado

/* === Private function declarations =========================================================== */

static void SimulatedSeconds(clock_t clock, uint32_t seconds);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

static void SimulatedSeconds(clock_t clock, uint32_t seconds) {
    // Simula el avance del reloj en segundos
    for (uint32_t i = 0; i <CLOCK_TICKS_PER_SECOND * seconds; i++) {
        ClockNewTick(clock);
    }

}

/* === Public function implementation ========================================================= */


void setUp(void) {
 
    clock = ClockCreate(CLOCK_TICKS_PER_SECOND);

}

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
    
    TEST_ASSERT_TRUE(ClockSetTime(clock, &new_time));
    TEST_ASSERT_TRUE(ClockGetTime(clock,  &current_time));
    TEST_ASSERT_TIME (4, 5, 3, 2, 1, 4, current_time);
}

// Despues de n ciclos el reloj avanza un segundo

void test_clock_advance_one_second (void) {

    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulatedSeconds(clock, 1);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(1, 0, 0, 0, 0, 0, current_time);
}

void test_clock_advance_ten_seconds (void) {

    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulatedSeconds(clock, 10);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 1, 0, 0, 0, 0, current_time);
}

void test_clock_advance_one_minut (void) {

    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulatedSeconds(clock, 60);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 0, 1, 0, 0, 0, current_time);
}

void test_clock_advance_ten_minuts (void) {

    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulatedSeconds(clock, 600);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 0, 0, 1, 0, 0, current_time);
}

void test_clock_advance_one_hour (void) {

    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulatedSeconds(clock, 3600);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 0, 0, 0, 1, 0, current_time);
}

void test_clock_advance_ten_hours (void) {

    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulatedSeconds(clock, 36000);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 0, 0, 0, 0, 1, current_time);
}

void test_clock_retourn_time (void) {

    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulatedSeconds(clock, 86400);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 0, 0, 0, 0, 0, current_time);
}

void test_clock_set_alarm_time (void) {

    clock_time_t alarm_time = {
        .time = {
            .seconds = {5, 0},
            .minutes = {3, 0},
            .hours   = {1, 0}
        }
    };

    clock_time_t read_alarm_time = {0};

    TEST_ASSERT_TRUE(ClockSetAlarm(clock, &alarm_time));
    TEST_ASSERT_TRUE(ClockGetAlarm(clock, &read_alarm_time));
    TEST_ASSERT_EQUAL_MEMORY(&alarm_time, &read_alarm_time, sizeof(clock_time_t));
}

//avanza un segundo y si la hora coincide devuelve true
void test_clock_alarm_ringing_at_exact_time(void) {
    clock_time_t alarm_time = {
        .time = {
            .seconds = {0, 0},
            .minutes = {1, 0},
            .hours   = {0, 0}
        }
    };

    ClockSetTime(clock, &(clock_time_t){
        .time = {
            .seconds = {9, 5},
            .minutes = {0, 0},
            .hours   = {0, 0}
        }
    });

    ClockSetAlarm(clock, &alarm_time);

    ClockEnableAlarm(clock);

    SimulatedSeconds(clock, 1); // Avanza un segundo → 00:01:00

    TEST_ASSERT_TRUE(ClockAlarmMatchTheTime(clock));
}

void test_clock_alarm_not_sound_if_disabled(void) {
    clock_time_t alarm_time = {
        .time = {
            .seconds = {0, 0},
            .minutes = {1, 0},
            .hours   = {0, 0}
        }
    };

    // Ajustamos la hora a 00:00:59 (se reiniciará a 01:00 tras 1 segundo)
    ClockSetTime(clock, &(clock_time_t){
        .time = {
            .seconds = {9, 5},
            .minutes = {0, 0},
            .hours   = {0, 0}
        }
    });

    ClockSetAlarm(clock, &alarm_time);

    ClockDisableAlarm(clock); // Desactivamos la alarma

    SimulatedSeconds(clock, 1); // Avanzamos 1 segundo → 00:01:00

    TEST_ASSERT_FALSE(ClockAlarmMatchTheTime(clock)); // No debe sonar
}

void test_clock_alarm_enable_disable_check(void) {
    // Al inicio está deshabilitada
    TEST_ASSERT_FALSE(ClockIsAlarmEnabled(clock));

    // Habilitamos la alarma
    ClockEnableAlarm(clock);
    TEST_ASSERT_TRUE(ClockIsAlarmEnabled(clock));

    // Deshabilitamos la alarma
    ClockDisableAlarm(clock);
    TEST_ASSERT_FALSE(ClockIsAlarmEnabled(clock));
}

void test_clock_postpone_alarm_by_minutes(void) {
    clock_time_t alarm_time = {
        .time = {
            .seconds = {0, 0},
            .minutes = {5, 3},  // 35 minutos
            .hours   = {2, 1}   // 12 horas
        }
    };

    ClockSetAlarm(clock, &alarm_time);
    ClockPostponeAlarm(clock, 30);  // +30 min => 13:05

    clock_time_t result = {0};
    ClockGetAlarm(clock, &result);

    TEST_ASSERT_TIME(0, 0, 5, 3, 2, 1, result);
}


/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */

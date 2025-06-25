/*********************************************************************************************************************
Copyright (c) 2025, Roldan Jesus Alejandro kechuroldanjesus@gmail.com

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

#ifndef CLOCK_H_
#define CLOCK_H_

/** @file defines.h
 ** @brief Plantilla para la creación de archivos de de cabeceras en lenguaje C
 **/

/* === Headers files inclusions ==================================================================================== */

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* === Public data type declarations =============================================================================== */

typedef union {
    struct {
        uint8_t seconds [2];
        uint8_t minutes [2];
        uint8_t hours [2];
    } time;
    uint8_t bcd [6];
} clock_time_t;

typedef struct clock_s * clock_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

clock_t ClockCreate (uint16_t ticks_per_second);

bool ClockGetTime(clock_t clock, clock_time_t * result);

bool ClockSetTime(clock_t clock, const clock_time_t * new_time);

void ClockNewTick(clock_t clock);

bool ClockSetAlarm(clock_t self, const clock_time_t * alarm_time);

bool ClockGetAlarm(clock_t self, clock_time_t * alarm_time);

bool ClockAlarmMatchTheTime(clock_t self);

void ClockEnableAlarm(clock_t self);

void ClockDisableAlarm(clock_t self);

bool ClockIsAlarmEnabled(clock_t self);

void ClockPostponeAlarm(clock_t self, uint8_t minutes);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H_ */

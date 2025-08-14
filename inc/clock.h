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

/** @file clock.h
 ** @brief Esta biblioteca permite simular el funcionamiento de un reloj digital que mantiene la hora
 * con precisión de segundos. También incluye funcionalidades para establecer y controlar
 * una alarma configurable, utilizando el paradigma de TDD (Test Driven Development).
 *
 * La hora y la alarma son representadas en formato BCD en un arreglo de 6 bytes:
 * - bcd[0] y bcd[1]: unidades y decenas de segundos.
 * - bcd[2] y bcd[3]: unidades y decenas de minutos.
 * - bcd[4] y bcd[5]: unidades y decenas de horas.
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

/**
 * @brief Estructura para representar una hora en formato BCD.
 *
 * Este tipo de dato permite acceder a la hora como un arreglo de 6 bytes (`bcd`)
 * o bien de forma estructurada como segundos, minutos y horas en BCD.
 */

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

/**
 * @brief Crea una nueva instancia del reloj.
 *
 * @param ticks_per_second Número de ticks requeridos para que avance un segundo.
 * @return clock_t Puntero al nuevo reloj creado.
 */

clock_t ClockCreate (uint16_t ticks_per_second);

/**
 * @brief Obtiene la hora actual del reloj.
 *
 * @param clock Instancia del reloj.
 * @param result Puntero donde se almacenará la hora actual.
 * @return true si la hora es válida, false en caso contrario.
 */

bool ClockGetTime(clock_t clock, clock_time_t * result);

/**
 * @brief Establece una nueva hora para el reloj.
 *
 * @param clock Instancia del reloj.
 * @param new_time Puntero a la nueva hora en formato BCD.
 * @return true si se configuró correctamente.
 */

bool ClockSetTime(clock_t clock, const clock_time_t * new_time);

/**
 * @brief Indica al reloj que ha pasado un tick de reloj.
 *
 * Llama a esta función periódicamente (según el valor de ticks_per_second) para actualizar la hora.
 *
 * @param clock Instancia del reloj.
 */

void ClockNewTick(clock_t clock);

/**
 * @brief Establece una nueva hora de alarma.
 *
 * @param self Instancia del reloj.
 * @param alarm_time Puntero a la hora deseada de la alarma.
 * @return true si se configuró correctamente.
 */

bool ClockSetAlarm(clock_t self, const clock_time_t * alarm_time);

/**
 * @brief Obtiene la hora actualmente configurada como alarma.
 *
 * @param self Instancia del reloj.
 * @param alarm_time Puntero donde se almacenará la hora de la alarma.
 * @return true si se obtuvo correctamente.
 */

bool ClockGetAlarm(clock_t self, clock_time_t * alarm_time);


/**
 * @brief Verifica si la hora actual coincide con la hora de alarma.
 *
 * @param self Instancia del reloj.
 * @return true si la hora actual coincide y la alarma está habilitada.
 */

bool ClockAlarmMatchTheTime(clock_t self);

/**
 * @brief Habilita la alarma.
 *
 * @param self Instancia del reloj.
 */

void ClockEnableAlarm(clock_t self);

/**
 * @brief Deshabilita la alarma.
 *
 * @param self Instancia del reloj.
 */

void ClockDisableAlarm(clock_t self);

/**
 * @brief Consulta si la alarma está habilitada.
 *
 * @param self Instancia del reloj.
 * @return true si la alarma está activa.
 */

bool ClockIsAlarmEnabled(clock_t self);

/**
 * @brief Pospone la alarma una cantidad arbitraria de minutos.
 *
 * Si la alarma está habilitada, esta función suma la cantidad de minutos indicada
 * y actualiza la hora de la alarma de forma adecuada.
 *
 * @param self Instancia del reloj.
 * @param minutes Minutos que se desean añadir a la hora de alarma.
 */

void ClockPostponeAlarm(clock_t self, uint8_t minutes);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H_ */

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

#ifndef CLOCK_TASK_H_
#define CLOCK_TASK_H_

/** @file cloc_task.h
 ** @brief Declaraciones para la tarea que maneja el reloj digital
 **/

/* === Headers files inclusions ==================================================================================== */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "clock.h"

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

/**
 * @enum clock_state_t
 * @brief Estados posibles de la máquina de estados del reloj.
 */

typedef enum {
    STATE_CLOCK_INIT,
    STATE_NORMAL,
    STATE_SET_HOURS,
    STATE_SET_MINUTES,
    STATE_SET_ALARM_HOURS,
    STATE_SET_ALARM_MINUTES
} clock_state_t;



/* === Public variable declarations ================================================================================ */

/**
 * @brief Obtiene el número de ticks del reloj del sistema.
 * @return Número de ticks desde el inicio del sistema.
 */

uint32_t ClockGetTicks(void);

/** @brief Mutex para proteger el acceso a la variable de estado compartida */

extern SemaphoreHandle_t xStateMutex;

/** @brief Instancia del reloj principal del sistema */

extern clock_t clock;

/** @brief Estructura con la hora y minutos actuales del reloj */

extern clock_time_t time_clock;

/** @brief Estado actual de la máquina de estados del reloj */

extern clock_state_t state;  // Variable global compartida

/* === Public function declarations ================================================================================ */

/**
 * @brief Tarea de FreeRTOS que gestiona el reloj digital.
 *
 * Esta función implementa la máquina de estados del reloj, manejando los estados de inicialización,
 * operación normal y ajuste de hora/alarma. La tarea actualiza la hora, gestiona la alarma y
 * protege los accesos compartidos mediante semáforos.
 *
 * @param pvParameters Puntero a parámetros de la tarea (generalmente NULL)
 */

void vClockTask(void *pvParameters);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_TASK_H_ */

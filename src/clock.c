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

/** @file clock.c
 ** @brief Implementación de una biblioteca de reloj digital con alarma basada en TDD.
 *
 * Este módulo implementa un reloj digital que mantiene la hora en formato BCD y gestiona una alarma configurable.
 * La hora avanza en función de "ticks" de reloj simulados. La biblioteca está diseñada para ser utilizada
 * en entornos embebidos y es testeable con frameworks como Unity.
 *
 * Las funcionalidades principales incluyen:
 * - Mantenimiento de hora actual con precisión de segundos.
 * - Ajuste y lectura de la hora actual.
 * - Ajuste, lectura, habilitación, deshabilitación y comparación de la hora de alarma.
 * - Posposición de la alarma una cantidad arbitraria de minutos.
 **/

/* === Headers files inclusions ==================================================================================== */
#include "clock.h"
#include <stddef.h>
#include <string.h>
/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

struct clock_s {
    uint16_t clock_ticks;     // Ticks por segundo (constante, lo pasa ClockCreate)
    uint16_t tick_counter;    // Contador interno de ticks acumulados
    clock_time_t current_time;
    bool is_valid;

    clock_time_t alarm_time;
    bool alarm_active;
};


/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function implementation ============================================================================== */

/**
 * @brief Crea una instancia de reloj con los ticks por segundo especificados.
 *
 * @param ticks_per_second Número de ticks necesarios para que pase un segundo.
 * @return clock_t Instancia del reloj inicializado (hora inválida y alarma deshabilitada).
 */

clock_t ClockCreate(uint16_t ticks_per_second) {
    static struct clock_s self[1];
    memset(self, 0, sizeof(struct clock_s));
    self->is_valid = false;
    self->alarm_active = false;
    self->clock_ticks = ticks_per_second;  
    self->tick_counter = 0;                
   return self;
}


/**
 * @brief Obtiene la hora actual del reloj.
 *
 * @param self Instancia del reloj.
 * @param result Puntero donde se almacena la hora en formato BCD.
 * @return true si la hora es válida, false si aún no ha sido configurada.
 */

bool ClockGetTime(clock_t self, clock_time_t * result) {
    memcpy(result, &self->current_time, sizeof(clock_time_t));
    return self->is_valid; 
}

/**
 * @brief Establece una nueva hora para el reloj.
 *
 * @param self Instancia del reloj.
 * @param new_time Hora en formato BCD a configurar.
 * @return true si se configuró correctamente.
 */

bool ClockSetTime(clock_t self, const clock_time_t * new_time) {
    self->is_valid = true; // Marca la hora como válida
    memcpy(&self->current_time, new_time, sizeof(clock_time_t));
    return self->is_valid;
}

/**
 * @brief Notifica al reloj que ha transcurrido un tick.
 *
 * Cuando se alcanza la cantidad de ticks correspondiente a un segundo, la hora se incrementa.
 * Esta función debe ser llamada periódicamente por el sistema.
 *
 * @param self Instancia del reloj.
 */

void ClockNewTick(clock_t self) {
    self->tick_counter++;  

    if (self->tick_counter >= self->clock_ticks) {
        self->tick_counter = 0;  

        // Aumentar 1 segundo en formato BCD
        self->current_time.time.seconds[0]++;
        if (self->current_time.time.seconds[0] == 10) {
            self->current_time.time.seconds[0] = 0;
            self->current_time.time.seconds[1]++;
        }
        if (self->current_time.time.seconds[1] == 6) {
            self->current_time.time.seconds[1] = 0;
            self->current_time.time.minutes[0]++;
        }
        if (self->current_time.time.minutes[0] == 10) {
            self->current_time.time.minutes[0] = 0;
            self->current_time.time.minutes[1]++;
        }
        if (self->current_time.time.minutes[1] == 6) {
            self->current_time.time.minutes[1] = 0;
            self->current_time.time.hours[0]++;
        }
        if (self->current_time.time.hours[0] == 10) {
            self->current_time.time.hours[0] = 0;
            self->current_time.time.hours[1]++;
        }
        if (self->current_time.time.hours[1] == 2 && self->current_time.time.hours[0] == 4) {
            self->current_time.time.hours[0] = 0;
            self->current_time.time.hours[1] = 0;
        }
    }
}


/**
 * @brief Establece la hora deseada para la alarma.
 *
 * @param self Instancia del reloj.
 * @param alarm_time Hora de la alarma en formato BCD.
 * @return true si se estableció correctamente.
 */

bool ClockSetAlarm(clock_t self, const clock_time_t * alarm_time) {
    memcpy(&self->alarm_time, alarm_time, sizeof(clock_time_t));
    return true;
}

/**
 * @brief Obtiene la hora actualmente configurada como alarma.
 *
 * @param self Instancia del reloj.
 * @param alarm_time Puntero donde se almacenará la hora de la alarma.
 * @return true si se recuperó correctamente.
 */

bool ClockGetAlarm(clock_t self, clock_time_t * alarm_time) {
    memcpy(alarm_time, &self->alarm_time, sizeof(clock_time_t));
    return true;
}

/**
 * @brief Verifica si la hora actual coincide con la de la alarma.
 *
 * @param self Instancia del reloj.
 * @return true si la hora actual coincide con la alarma y esta está habilitada.
 */

bool ClockAlarmMatchTheTime(clock_t self) {
    if (!self->alarm_active) {
        return false; // La alarma no está activa, no hay coincidencia
    }
    return memcmp(&self->current_time, &self->alarm_time, sizeof(clock_time_t)) == 0;
}

/**
 * @brief Habilita la alarma del reloj.
 *
 * @param self Instancia del reloj.
 */

void ClockEnableAlarm(clock_t self) {
    self->alarm_active = true;
}

/**
 * @brief Deshabilita la alarma del reloj.
 *
 * @param self Instancia del reloj.
 */

void ClockDisableAlarm(clock_t self) {
    self->alarm_active = false;
}

/**
 * @brief Consulta si la alarma del reloj está habilitada.
 *
 * @param self Instancia del reloj.
 * @return true si la alarma está habilitada, false en caso contrario.
 */

bool ClockIsAlarmEnabled(clock_t self) {
    return self->alarm_active;
}

/**
 * @brief Pospone la alarma una cantidad arbitraria de minutos.
 *
 * Si la alarma está habilitada, esta función le suma los minutos indicados y la ajusta correctamente
 * manteniendo el formato BCD y límites de 24 horas.
 *
 * @param self Instancia del reloj.
 * @param minutes Minutos a sumar a la hora de la alarma.
 */

void ClockPostponeAlarm(clock_t self, uint8_t minutes) {
    if (!self->alarm_active || minutes == 0) {
        return; // No hace nada si la alarma no está activa o no se desea posponer
    }

    // Convertir BCD a binario para los minutos
    uint8_t min_bcd = self->alarm_time.time.minutes[0] + self->alarm_time.time.minutes[1] * 10;
    uint8_t hour_bcd = self->alarm_time.time.hours[0] + self->alarm_time.time.hours[1] * 10;

    // Sumar minutos
    min_bcd += minutes;

    // Ajustar horas si se excede de 59 minutos
    hour_bcd += min_bcd / 60;
    min_bcd %= 60;

    // Ajustar horas para que no supere 23
    hour_bcd %= 24;

    // Volver a convertir a BCD
    self->alarm_time.time.minutes[1] = min_bcd / 10;
    self->alarm_time.time.minutes[0] = min_bcd % 10;
    self->alarm_time.time.hours[1] = hour_bcd / 10;
    self->alarm_time.time.hours[0] = hour_bcd % 10;
}

/* === End of documentation ======================================================================================== */

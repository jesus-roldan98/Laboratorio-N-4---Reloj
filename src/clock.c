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

/** @file plantilla.c
 ** @brief Plantilla para la creación de archivos de código fuente en lenguaje C
 **/

/* === Headers files inclusions ==================================================================================== */
#include "clock.h"
#include <stddef.h>
#include <string.h>
/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

struct clock_s {
    uint16_t clock_ticks; // Número de ticks por segundo
    clock_time_t current_time;
    bool is_valid; // Indica si la hora es válida

    clock_time_t alarm_time; // Hora de la alarma
    bool alarm_active;
};

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


/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function implementation ============================================================================== */

clock_t ClockCreate(uint16_t ticks_per_second) {
    (void)ticks_per_second; 
    static struct clock_s self[1];
    memset(self, 0, sizeof(struct clock_s)); // Inicializar la estructura a cero
    self->is_valid = false; // Inicializar el reloj con hora inválida
    self->alarm_active = false; // Inicializar la alarma como inactiva
    return self; // Implementación pendiente
}

bool ClockGetTime(clock_t self, clock_time_t * result) {
    memcpy(result, &self->current_time, sizeof(clock_time_t));
    return self->is_valid; 
}

bool ClockSetTime(clock_t self, const clock_time_t * new_time) {
    self->is_valid = true; // Marca la hora como válida
    memcpy(&self->current_time, new_time, sizeof(clock_time_t));
    return self->is_valid;
}

void ClockNewTick(clock_t self) {

    self->clock_ticks++;
    if (self->clock_ticks == 5) {
        self->clock_ticks = 0; // Reinicia el contador de ticks
        self->current_time.time.seconds[0] ++; // Incrementa los segundos
        if (self->current_time.time.seconds[0] == 10) {
            self->current_time.time.seconds[0] = 0; // Reinicia los segundos a 0
            self->current_time.time.seconds[1] ++; // Incrementa el dígito de los segundos
        }
        if (self->current_time.time.seconds[1] == 6) {
            self->current_time.time.seconds[1] = 0; // Incrementa los minutos
            self->current_time.time.minutes[0] ++; // Incrementa los minutos
        }

        if (self->current_time.time.minutes[0] == 10) {
            self->current_time.time.minutes[0] = 0; // Reinicia los minutos a 0
            self->current_time.time.minutes[1] ++; // Incrementa el dígito de los minutos
        }

        if (self->current_time.time.minutes[1] == 6) {
            self->current_time.time.minutes[1] = 0; // Incrementa las horas
            self->current_time.time.hours[0] ++; // Incrementa las horas
        }

        if (self->current_time.time.hours[0] == 10) {
            self->current_time.time.hours[0] = 0; // Reinicia las horas a 0
            self->current_time.time.hours[1] ++; // Incrementa el dígito de las horas
        }

        if (self->current_time.time.hours[1] == 2 && self->current_time.time.hours[0] == 4) {
            // Reset a 00:00:00
            self->current_time.time.hours[0] = 0;
            self->current_time.time.hours[1] = 0;
        }
    }
    
}

bool ClockSetAlarm(clock_t self, const clock_time_t * alarm_time) {
    memcpy(&self->alarm_time, alarm_time, sizeof(clock_time_t));
    return true;
}

bool ClockGetAlarm(clock_t self, clock_time_t * alarm_time) {
    memcpy(alarm_time, &self->alarm_time, sizeof(clock_time_t));
    return true;
}

bool ClockAlarmMatchTheTime(clock_t self) {
    if (!self->alarm_active) {
        return false; // La alarma no está activa, no hay coincidencia
    }
    return memcmp(&self->current_time, &self->alarm_time, sizeof(clock_time_t)) == 0;
}

void ClockEnableAlarm(clock_t self) {
    self->alarm_active = true;
}

void ClockDisableAlarm(clock_t self) {
    self->alarm_active = false;
}

bool ClockIsAlarmEnabled(clock_t self) {
    return self->alarm_active;
}


/* === End of documentation ======================================================================================== */

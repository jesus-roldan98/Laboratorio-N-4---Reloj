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

/** @file main.c
 ** @brief Plantilla para la creación del archivo main
 **/

/* === Headers files inclusions =============================================================== */

#include "bsp.h"
#include <stdbool.h>
#include "screen.h"
#include "clock.h"

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

typedef enum {
    STATE_CLOCK_INIT,        // Estado inicial: inicialización del reloj
    STATE_NORMAL,            // Estado principal: mostrar hora actual
    STATE_SET_HOURS,         // Estado para configurar horas
    STATE_SET_MINUTES,       // Estado para configurar minutos
    STATE_SET_ALARM_HOURS,   // Configurar alarma - horas
    STATE_SET_ALARM_MINUTES, // Configurar alarma - minutos
    
} clock_state_t;

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

static BoardT board;
static clock_t clock; // Variable para almacenar el reloj simulado
static clock_state_t state;
static uint32_t blink_counter = 0;  // Contador para controlar el parpadeo
static bool alarm_active = false; // Variable para controlar el estado de la alarma

/* === Private function implementation ========================================================= */

void ClockStates(clock_state_t mode) {

    state = mode;

    switch (state) {
    case STATE_CLOCK_INIT:

        DisplayFlashDigits(board->screen, 0, 3, 100);
        DisplayFlashPoints(board->screen, 1, 1, 100);

        break;

    case STATE_NORMAL:

        DisplayFlashDigits(board->screen, 0, 0, 0);
        DisplayFlashPoints(board->screen, 0, 0, 0);
        break;

    case STATE_SET_HOURS:

        DisplayFlashDigits(board->screen, 0, 1, 50);
        DisplayFlashPoints(board->screen, 0, 3, 0);
        break;

    case STATE_SET_MINUTES:

        DisplayFlashDigits(board->screen, 2, 3, 50);
        DisplayFlashPoints(board->screen, 0, 3, 0);
        break;

    case STATE_SET_ALARM_HOURS:
        DisplayFlashDigits(board->screen, 0, 3, 50);
        DisplayFlashPoints(board->screen, 0, 3, 0);
        break;

    case STATE_SET_ALARM_MINUTES:

        DisplayFlashDigits(board->screen, 0, 3, 50);
        DisplayFlashPoints(board->screen, 0, 3, 0);

        break;
    }
}

void UpBCDAdjusted(uint8_t numero[2], bool is_hours) {
    uint8_t temp[2] = {numero[1], numero[0]}; // Invertimos para trabajar
    
    // Incrementamos como decimal normal
    uint16_t value = temp[0] * 10 + temp[1];
    value = (value + 1) % (is_hours ? 24 : 60);
    
    // Convertimos de vuelta a BCD invertido
    temp[0] = value / 10; // Decenas
    temp[1] = value % 10; // Unidades
    
    numero[0] = temp[1]; // Restauramos orden original
    numero[1] = temp[0];
}

// Función para decremento ajustado
void DownBCDAdjusted(uint8_t numero[2], bool is_hours) {
    uint8_t temp[2] = {numero[1], numero[0]}; // Invertimos
    
    uint16_t value = temp[0] * 10 + temp[1];
    value = (value == 0) ? (is_hours ? 23 : 59) : value - 1;
    
    temp[0] = value / 10;
    temp[1] = value % 10;
    
    numero[0] = temp[1];
    numero[1] = temp[0];
}

void HandleAlarm(void) {
    // Verificar si debemos activar la alarma
    if (!alarm_active && ClockIsAlarmEnabled(clock) && ClockAlarmMatchTheTime(clock) && (state == STATE_NORMAL)) {
        alarm_active = true;
        DigitalOutputActivate(board->led_green);  // Encender LED de alarma
    }
    
    // La alarma permanecerá activa hasta que el usuario la cancele
}

void CancelAlarm(void) {
    if (alarm_active) {
        alarm_active = false;
        DigitalOutputDeactivate(board->led_green);  // Apagar LED de alarma
        
    }
}


/* === Public function implementation ========================================================= */

int main(void) {

    clock_time_t time_clock;
    clock_time_t time_alarm;

    board = BoardCreate();     // Crear la estructura de la placa y asignar los pines a los leds y botones
    clock = ClockCreate(1000); // Crear el objeto reloj
    int divisor = 0;

    
    
    ClockGetTime(clock, &time_clock); // Obtener la hora actual del reloj
    ClockGetAlarm(clock,&time_alarm);
    ClockDisableAlarm(clock); 

    SysTickInit(1000);
    ClockStates(STATE_CLOCK_INIT);

    while (true) {

        if (DigitalInputHasActivate(board->cancel)){
            if (state == STATE_NORMAL){
                if (alarm_active) {
                    ClockDisableAlarm(clock);
                    CancelAlarm(); // Cancelar la alarma si está activa
                }
            }
            else if (state == STATE_SET_HOURS || state == STATE_SET_MINUTES) {
                if (ClockGetTime(clock, &time_clock)) {
                    ClockStates(STATE_NORMAL);
                } else {
                    ClockStates(STATE_CLOCK_INIT); 
                }
            } else if (state == STATE_SET_ALARM_HOURS || state == STATE_SET_ALARM_MINUTES) {

                ClockStates(STATE_NORMAL);
            }
        }

        if (DigitalInputHasActivate(board->set_time)) {

            ClockStates(STATE_SET_MINUTES); // Cambiar al estado de configuración de horas
            ClockGetTime(clock, &time_clock); // Obtener la hora actual del reloj
        }

        if (DigitalInputHasActivate(board->set_alarm)) {

            ClockStates(STATE_SET_ALARM_MINUTES); // Cambiar al estado de configuración de alarma
            ClockGetAlarm(clock, &time_alarm);

        }

        if (DigitalInputHasActivate(board->accept)) {

            if (state == STATE_SET_MINUTES) {

                ClockStates(STATE_SET_HOURS);
            } 
            
            else if (state == STATE_SET_HOURS) {
                time_clock.time.seconds[0] = 0;
                time_clock.time.seconds[1] = 0;
                ClockSetTime(clock, &time_clock);
                ClockStates(STATE_NORMAL); // Cambiar al estado normal
            }

            else if (state == STATE_SET_ALARM_MINUTES) {

                ClockStates(STATE_SET_ALARM_HOURS);
            }

            else if (state == STATE_SET_ALARM_HOURS) {

                time_alarm.time.seconds[0] = 0;
                time_alarm.time.seconds[1] = 0;
                ClockSetAlarm(clock, &time_alarm);
                ClockStates(STATE_NORMAL);
                ClockEnableAlarm(clock);
            }

            else if (state == STATE_NORMAL) {

                if (alarm_active) {

                    ClockPostponeAlarm(clock, 5);
                    CancelAlarm();
                }

    
            }

        }

        if (DigitalInputHasActivate(board->increment)) {

            if (state == STATE_SET_HOURS) {

                UpBCDAdjusted(time_clock.time.hours, true);
                ScreenWriteBCD(board->screen, &time_clock, false, (uint8_t[]){0, 0, 0, 0});
            } else if (state == STATE_SET_MINUTES) {

                UpBCDAdjusted(time_clock.time.minutes, false);
                ScreenWriteBCD(board->screen, &time_clock, false, (uint8_t[]){0, 0, 0, 0});
            } else if (state == STATE_SET_ALARM_HOURS) {

                UpBCDAdjusted(time_alarm.time.hours, true);
                ScreenWriteBCD(board->screen, &time_alarm, false, (uint8_t[]){1, 1, 1, 1});
            } else if (state == STATE_SET_ALARM_MINUTES) {

                UpBCDAdjusted(time_alarm.time.minutes, false);
                ScreenWriteBCD(board->screen, &time_alarm, false, (uint8_t[]){1, 1, 1, 1});
            }
        }

        if (DigitalInputHasActivate(board->decrement)) {
            
            if (state == STATE_SET_HOURS) {
                DownBCDAdjusted(time_clock.time.hours, true);
                ScreenWriteBCD(board->screen, &time_clock, false, (uint8_t[]){0, 0, 0, 0});
            } else if (state == STATE_SET_MINUTES) {

                DownBCDAdjusted(time_clock.time.minutes, false);
                ScreenWriteBCD(board->screen, &time_clock, false, (uint8_t[]){0, 0, 0, 0});
            } else if (state == STATE_SET_ALARM_HOURS) {

                UpBCDAdjusted(time_alarm.time.hours, true);
                ScreenWriteBCD(board->screen, &time_alarm, false, (uint8_t[]){1, 1, 1, 1});
            } else if (state == STATE_SET_ALARM_MINUTES) {

                UpBCDAdjusted(time_alarm.time.minutes, false);
                ScreenWriteBCD(board->screen, &time_alarm, false, (uint8_t[]){1, 1, 1, 1});
            }

        }

        divisor++;

        if (divisor == 5) {
            divisor = 0;
        }

        for (int delay = 0; delay < 25000; delay++) { __asm("NOP"); }
    }
}

void SysTick_Handler(void) {
    clock_time_t current_time;
    static bool show_dot = true;
    uint8_t decimal_points[4] = {0, 0, 0, 0};

    blink_counter++;
    if (blink_counter >= 1000) {
        blink_counter = 0;
        show_dot = !show_dot;
        
        // Verificar coincidencia de alarma solo una vez por segundo
        if (!alarm_active && (state == STATE_NORMAL)) {
            HandleAlarm();
        }
    }

    ScreenRefresh(board->screen);
    ClockNewTick(clock);
    ClockGetTime(clock, &current_time);
    HandleAlarm();

    if (state == STATE_NORMAL || state == STATE_CLOCK_INIT) {
        if (state == STATE_NORMAL) {
            decimal_points[1] = show_dot ? 1 : 0;
        }else {
            decimal_points[1] = 1; // No mostrar punto decimal en el estado de inicialización
        }
        decimal_points[3] = ClockIsAlarmEnabled(clock) ? 1 : 0;
        ScreenWriteBCD(board->screen, &current_time, false, decimal_points);
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */

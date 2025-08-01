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

/* === Private function implementation ========================================================= */

void ClockStates(clock_state_t mode) {

    state = mode;

    switch (state) {
    case STATE_CLOCK_INIT:

        DisplayFlashDigits(board->screen, 0, 3, 100);
        DisplayFlashPoints(board->screen, 0, 3, 0);

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
        DisplayFlashDigits(board->screen, 0, 1, 50);
        break;

    case STATE_SET_ALARM_MINUTES:

        DisplayFlashDigits(board->screen, 2, 3, 50);

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



/* === Public function implementation ========================================================= */

int main(void) {

    clock_time_t time_clock;

    board = BoardCreate();     // Crear la estructura de la placa y asignar los pines a los leds y botones
    clock = ClockCreate(1000); // Crear el objeto reloj
    int divisor = 0;
    
    ClockGetTime(clock, &time_clock); // Obtener la hora actual del reloj

    SysTickInit(1000);
    ClockStates(STATE_CLOCK_INIT);

    while (true) {

        if (DigitalInputHasActivate(board->set_time)) {

            ClockStates(STATE_SET_MINUTES); // Cambiar al estado de configuración de horas
            ClockGetTime(clock, &time_clock); // Obtener la hora actual del reloj
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
        }

        if (DigitalInputHasActivate(board->increment)) {

            if (state == STATE_SET_HOURS) {

                UpBCDAdjusted(time_clock.time.hours, true);
                ScreenWriteBCD(board->screen, &time_clock, false, (uint8_t[]){0, 0, 0, 0});
            } else if (state == STATE_SET_MINUTES) {

                UpBCDAdjusted(time_clock.time.minutes, false);
                ScreenWriteBCD(board->screen, &time_clock, false, (uint8_t[]){0, 0, 0, 0});
            }
        }

        if (DigitalInputHasActivate(board->decrement)) {
            
            if (state == STATE_SET_HOURS) {
                
                DownBCDAdjusted(time_clock.time.hours, true);
                ScreenWriteBCD(board->screen, &time_clock, false, (uint8_t[]){0, 0, 0, 0});
            } else if (state == STATE_SET_MINUTES) {

                DownBCDAdjusted(time_clock.time.minutes, false);
                ScreenWriteBCD(board->screen, &time_clock, false, (uint8_t[]){0, 0, 0, 0});
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
    static bool show_dot = true;  // Estado del punto decimal
    uint8_t decimal_points[4] = {0, 0, 0, 0};

    blink_counter++;
    // Cambiar estado del punto cada segundo (1000 ticks)
    if (blink_counter >= 1000) {
        blink_counter = 0;
        show_dot = !show_dot;  // Alternar estado del punto
    }

    ScreenRefresh(board->screen); // Llamar a la función de actualización de pantalla en cada interrupción de SysTick
    ClockNewTick(clock);          // Llamar a la función de actualización del reloj en cada interrupción de SysTick
    ClockGetTime(clock, &current_time); // Obtener la hora actual del reloj
    
    if (state == STATE_NORMAL || state == STATE_CLOCK_INIT) {

        if (state == STATE_NORMAL) {
            decimal_points[1] = show_dot ? 1 : 0;
        
        }
        ScreenWriteBCD(board->screen, &current_time, false, decimal_points);
    }

}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */

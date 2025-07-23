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
    STATE_NORMAL,         // Estado principal: mostrar hora actual
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


/* === Private function implementation ========================================================= */

void ClockStates(clock_state_t mode) {
    
    state = mode;
   
        switch (state) {
        case STATE_CLOCK_INIT: 

            DisplayFlashDigits(board->screen, 0, 3, 50);
            DisplayFlashPoints(board->screen, 0, 3, 0);

            break;
        

        case STATE_NORMAL:
        
            DisplayFlashDigits(board->screen, 0, 0, 0);
            DisplayFlashPoints(board->screen, 1, 1, 50);
            break; 
        
        case STATE_SET_HOURS:
            
            DisplayFlashDigits(board->screen, 0, 1, 50);

            break;

        case STATE_SET_MINUTES:
            
            DisplayFlashDigits(board->screen, 2, 3, 50);
            
            break;

        case STATE_SET_ALARM_HOURS:
            DisplayFlashDigits(board->screen, 0, 1, 50);
            break;

        case STATE_SET_ALARM_MINUTES:
            
            DisplayFlashDigits(board->screen, 2, 3, 50);

            break;
    }
    
    
}

void UpBCD (uint8_t numero [2], const uint8_t limite [2]) {

    numero [1]++;
    if (numero[1] > 9) {
        numero[1] = 0;
        numero[0]++;
    }
    if ((numero [0] == limite [0]) && (numero [1] == limite [1])) {
        numero[0] = 0;
        numero[1] = 0;
    }
}

void DownBCD(uint8_t numero[2], const uint8_t limite[2]) {

    if (numero[1] == 0) {
        numero[1] = 9;

        if (numero[0] == 0) {
            numero[0] = 9;
        } else {
            numero[0]--;
        }
    } else {
        numero[1]--;
    }

    if ((numero[0] == limite[0]) && (numero[1] == limite[1])) {
        numero[0] = 0;
        numero[1] = 0;
    }
}


/* === Public function implementation ========================================================= */

int main(void) {

    board = BoardCreate(); // Crear la estructura de la placa y asignar los pines a los leds y botones
    clock = ClockCreate(1000); // Crear el objeto reloj
    int divisor = 0;
    
   

    SysTickInit(1000); // Inicializar SysTick con 1000 ticks por segundo

    ClockStates(STATE_CLOCK_INIT); // Cambiar al estado de inicialización del reloj


    while (true) {

       //if(DigitalInputHasActivate(board->accept)){
       //    if (state == STATE_NORMAL) {
       //     if ()
       //    }
//
       //}

      


        divisor++;

        if (divisor == 5) {
            divisor = 0;
        }

        
        for (int delay = 0; delay < 25000; delay++) {
             __asm("NOP"); 
        }
    }


}

void SysTick_Handler(void) {
    //static uint16_t counter = 0;
   // uint8_t hour{4};


    ScreenRefresh(board->screen); // Llamar a la función de actualización de pantalla en cada interrupción de SysTick
    ClockNewTick(clock); // Llamar a la función de actualización del reloj en cada interrupción de SysTick
    clock_time_t init_time = {
    .time.hours = {0, 0},
    .time.minutes = {0, 0},
    .time.seconds = {0, 0}
};
    if (state == STATE_CLOCK_INIT) {
        
        static uint8_t decimal_points[4] = {0, 0, 0, 0};

        // Actualizar el display en estado INIT (esto lo hace parpadear)
        ScreenWriteBCD(board->screen, &init_time, false, decimal_points);
    }

}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */

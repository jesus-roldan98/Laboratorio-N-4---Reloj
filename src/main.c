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

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

static BoardT board;
static clock_t clock; // Variable para almacenar el reloj simulado

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */

int main(void) {

    board = BoardCreate(); // Crear la estructura de la placa y asignar los pines a los leds y botones
    clock = ClockCreate(1000); // Crear el objeto reloj

    int divisor = 0;
    uint8_t value[4] = {0, 0, 0, 0};
    uint8_t value_decimal_points[4] = {1, 1, 1, 1}; // Ejemplo de puntos decimales

   

    SysTickInit(1000); // Inicializar SysTick con 1000 ticks por segundo

    ScreenWriteBCD(board->screen, value, sizeof(value), value_decimal_points); // Escribir valores en la pantalla

    DisplayFlashDigits(board->screen, 0, 3, 0); // configurar desde donde hasta donde parpadean los displays, ejemplo de 0 a 3 todos los
                           
    DisplayFlashPoints(board->screen, 0, 3, 50); // displays el ultimo numero indica la velocidad de parpadeo

    
    while (true) {

       

       if(DigitalInputHasActivate(board->cancel)) {
           DigitalOutputToggle(board->led_red);
       }

       //if(DigitalInputHasActivate(board->cancel)){
       //    DigitalOutputToggle(board->led_green);
       //}

       // if(DigitalInputHasActivate(board->set_alarm)){
       //     DigitalOutputActivate(board->led_blue);
       // }else if (DigitalInputHasActivate(board->accept)){
       //     DigitalOutputDeactivate(board->led_blue);
       // }


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
   ScreenRefresh(board->screen); // Llamar a la función de actualización de pantalla en cada interrupción de SysTick
   ClockNewTick(clock); // Llamar a la función de actualización del reloj en cada interrupción de SysTick
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */

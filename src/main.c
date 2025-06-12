/* Copyright 2022, Laboratorio de Microprocesadores
 * Facultad de Ciencias Exactas y Tecnología
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2025, Roldan Jesus Alejandro <kechuroldanjesus@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Simple sample of use LPC HAL gpio functions
 **
 ** \addtogroup samples Sample projects
 ** \brief Sample projects to use as a starting point
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "bsp.h"
#include <stdbool.h>
#include "screen.h"

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */

int main(void) {

    int divisor = 0;
    uint8_t value[4] = {1, 5, 1, 2};
    uint8_t value_decimal_points[4] = {0, 0, 1, 1}; // Ejemplo de puntos decimales

    BoardT board = BoardCreate();

    ScreenWriteBCD(board->screen, value, sizeof(value), value_decimal_points); // Escribir valores en la pantalla

    DisplayFlashDigits(board->screen, 0, 2, 50); // configurar desde donde hasta donde parpadean los displays, ejemplo de 0 a 3 todos los
                           
    DisplayFlashPoints(board->screen, 2, 3, 50); // displays el ultimo numero indica la velocidad de parpadeo

    
    while (true) {

       if(DigitalInputHasActivate(board->decrement)) {
           DigitalOutputToggle(board->led_red);
       }

       if(DigitalInputHasActivate(board->cancel)){
           DigitalOutputToggle(board->led_green);
       }

        if(DigitalInputHasActivate(board->set_alarm)){
            DigitalOutputActivate(board->led_blue);
        }else if (DigitalInputHasActivate(board->accept)){
            DigitalOutputDeactivate(board->led_blue);
        }


        divisor++;

        if (divisor == 5) {
            divisor = 0;
        }

        ScreenRefresh(board->screen);
        for (int delay = 0; delay < 25000; delay++) {
             __asm("NOP"); 
        }
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */

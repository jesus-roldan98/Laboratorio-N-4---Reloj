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

#ifndef BSP_H_
#define BSP_H_

/** @file bsp.h
 ** @brief Plantilla para la creación de archivos de de cabeceras en lenguaje C
 **/

/* === Headers files inclusions ==================================================================================== */

#include "digital.h"
#include "chip.h"
#include <stdbool.h>
#include "defines.h"
#include <stdlib.h>

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */
/* @brief Estructura con los parametros de la placa
    * @param LedRed indica el led rojo esta encendido del led rgb
    * @param LedGreen indica el led verde esta encendido del led rgb
    * @param LedBlue indica el led azul esta encendido del led rgb
    * @param Led1  led 1
    * @param Led2  led 2
    * @param Led3  led 3
    * @param Input1 Pin de la placa para el input 1
    * @param Input2 Pin de la placa para el input 2
    * @param Input3 Pin de la placa para el input 3
    * @param Input4 Pin de la placa para el input 4
    */ 
   
typedef struct BoardS {
    DigitalOutputT LedRed;
    DigitalOutputT LedGreen;
    DigitalOutputT LedBlue;
    DigitalOutputT Led1;
    DigitalOutputT Led2;
    DigitalOutputT Led3;

    DigitalInputT Input1;
    DigitalInputT Input2;
    DigitalInputT Input3;
    DigitalInputT Input4;

} const * BoardT;

/* === Public data type declarations =============================================================================== */

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

BoardT BoardCreate(void); //! <-- Crea la estructura de la placa y asigna los pines a los leds y botones

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* BSP_H_ */

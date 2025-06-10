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
 ** @brief Declaraciones para la capa de soporte de hardware
 **/

/* === Headers files inclusions ==================================================================================== */

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

#include "digital.h"
#include "screen.h"
/* === Public data type declarations =============================================================================== */


/**
 * @struct BoardS
 * @brief Estructura que representa todos los periféricos disponibles en la placa.
 *
 * Contiene referencias a salidas digitales (LEDs, buzzer), entradas digitales (botones)
 * y una pantalla multiplexada.
 */

typedef struct BoardS {
    DigitalOutputT buzzer;      /**< Salida digital para el zumbador */
    DigitalOutputT led_red;     /**< Salida digital para LED rojo */
    DigitalOutputT led_green;   /**< Salida digital para LED verde */
    DigitalOutputT led_blue;    /**< Salida digital para LED azul */
    DigitalInputT set_time;     /**< Entrada digital para botón de configurar hora */
    DigitalInputT set_alarm;    /**< Entrada digital para botón de configurar alarma */
    DigitalInputT decrement;    /**< Entrada digital para botón de disminuir valor */
    DigitalInputT increment;    /**< Entrada digital para botón de aumentar valor */
    DigitalInputT accept;       /**< Entrada digital para botón de aceptar */ 
    DigitalInputT cancel;       /**< Entrada digital para botón de cancelar */
    ScreenT screen;             /**< Pantalla de 7 segmentos multiplexada */

} const * BoardT;
/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */


/**
 * @brief Inicializa los recursos de hardware y devuelve un descriptor de la placa.
 *
 * Configura las entradas y salidas digitales, así como la pantalla de 7 segmentos.
 *
 * @return BoardT Puntero constante a la estructura con los periféricos inicializados.
 */

BoardT BoardCreate(void); //! <-- Crea la estructura de la placa y asigna los pines a los leds y botones

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* BSP_H_ */

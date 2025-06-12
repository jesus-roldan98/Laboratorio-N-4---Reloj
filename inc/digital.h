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

#ifndef DIGITAL_H_
#define DIGITAL_H_

/**
 * @file digital.h
 * @brief Declaraciones para el manejo de entradas y salidas digitales.
 */
/* === Headers files inclusions ==================================================================================== */
#include <stdbool.h>
/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

typedef struct DigitalOutputS * DigitalOutputT;

typedef struct DigitalInputS * DigitalInputT;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */


/**
 * @brief Crea un objeto de salida digital.
 *
 * @param port Número del puerto GPIO.
 * @param pin Número del pin dentro del puerto.
 * @return DigitalOutputT Puntero al objeto de salida digital creado.
 */

DigitalOutputT DigitalOutputCreate(int gpio, int bit, bool state); //! <- crea un objeto salida

/**
 * @brief Activa (pone en nivel alto) la salida digital.
 *
 * @param self Objeto de salida digital.
 */

void DigitalOutputActivate(DigitalOutputT self);


/**
 * @brief Desactiva (pone en nivel bajo) la salida digital.
 *
 * @param self Objeto de salida digital.
 */

void DigitalOutputDeactivate(DigitalOutputT self);

/**
 * @brief Invierte el estado actual de la salida digital.
 *
 * @param self Objeto de salida digital.
 */

void DigitalOutputToggle(DigitalOutputT self);

/**
 * @brief Crea un objeto de entrada digital.
 *
 * @param port Número del puerto GPIO.
 * @param pin Número del pin dentro del puerto.
 * @param inverted true si la entrada es activa en bajo, false si es activa en alto.
 * @return DigitalInputT Puntero al objeto de entrada digital creado.
 */

DigitalInputT DigitalInputCreate(int port, int pin, bool inverted); //! <- crea un objeto entrada

/**
 * @brief Obtiene el estado actual de la entrada digital.
 *
 * @param self Objeto de entrada digital.
 * @return true si la entrada está activa, false en caso contrario.
 */

bool DigitalInputGetState(DigitalInputT self);


/**
 * @brief Indica si el estado de la entrada ha cambiado desde la última lectura.
 *
 * @param self Objeto de entrada digital.
 * @return 1 si hubo cambio, 0 si no hubo, -1 si es inválido.
 */

int DigitalInputHasChanged(DigitalInputT self);

/**
 * @brief Indica si se produjo un flanco de subida en la entrada.
 *
 * @param self Objeto de entrada digital.
 * @return true si hubo flanco de subida, false en caso contrario.
 */

bool DigitalInputHasActivate(DigitalInputT self);

/**
 * @brief Indica si se produjo un flanco de bajada en la entrada.
 *
 * @param self Objeto de entrada digital.
 * @return true si hubo flanco de bajada, false en caso contrario.
 */

bool DigitalInputHasDeactivate(DigitalInputT self);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* DIGITAL_H_ */

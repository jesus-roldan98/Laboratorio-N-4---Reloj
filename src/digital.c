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

#include "digital.h"
#include "chip.h"
#include <stdbool.h>
#include "defines.h"
#include <stdlib.h>

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/**
 * @brief Estructura para representar una salida digital
 */

typedef struct DigitalOutputS {

    int gpio;       /**< Número de GPIO */
    int bit;        /**< Número de bit dentro del GPIO */
    int state;      /**< Estado actual de la salida */

} DigitalOutputS;

/**
 * @brief Estructura para representar una entrada digital
 */

typedef struct DigitalInputS {

    int port;           /**< Número de puerto */
    int pin;            /**< Número de pin dentro del puerto */
    bool inverted;      /**< Indica si la lógica está invertida */
    bool last_state;    /**< Último estado leído */

} DigitalInputS;

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function implementation ============================================================================== */

DigitalOutputT DigitalOutputCreate (int gpio, int bit, bool state) {
    DigitalOutputT self = malloc(sizeof(struct DigitalInputS));
    if (self != NULL) {
        self->gpio = gpio;
        self->bit = bit;
        self->state = state;
        
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->gpio, self->bit, !self->state);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->gpio, self->bit, true);
    }
    return self;
}
/**
 * @brief Activa (pone en alto) la salida digital.
 * 
 * @param self Objeto de salida digital.
 */

void DigitalOutputActivate(DigitalOutputT self) {

    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->gpio, self->bit, true);
}

/**
 * @brief Desactiva (pone en bajo) la salida digital.
 * 
 * @param self Objeto de salida digital.
 */

void DigitalOutputDeactivate(DigitalOutputT self) {

    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->gpio, self->bit, false);
}

/**
 * @brief Cambia el estado actual de la salida digital (toggle).
 * 
 * @param self Objeto de salida digital.
 */

void DigitalOutputToggle(DigitalOutputT self) {

    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, self->gpio, self->bit);
}

/**
 * @brief Crea y configura una entrada digital.
 * 
 * @param port Puerto donde está conectada la entrada.
 * @param pin Número de pin dentro del puerto.
 * @param inverted true si la entrada es activa en bajo, false si es activa en alto.
 * @return DigitalInputT Puntero al objeto creado, o NULL si falla la reserva de memoria.
 */

DigitalInputT DigitalInputCreate(int port, int pin, bool inverted) {
    DigitalInputT self = malloc(sizeof(struct DigitalInputS));
    if (self != NULL) {
        self->port = port;
        self->pin = pin;
        self->inverted = inverted;
        
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->port, self->pin, false);
        self->last_state = DigitalInputGetState (self);

    }
    return self;
}

/**
 * @brief Lee el estado actual de la entrada digital.
 * 
 * @param self Objeto de entrada digital.
 * @return true si está activo, false si está inactivo.
 */

bool DigitalInputGetState(DigitalInputT self) {
    bool state = Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, self->port, self->pin);

    if (self->inverted) {
        state = !state;
    }
    return state;
}

/**
 * @brief Verifica si la entrada cambió de estado desde la última lectura.
 * 
 * @param self Objeto de entrada digital.
 * @return 1 si hubo flanco ascendente, -1 si hubo flanco descendente, 0 si no hubo cambios.
 */

int DigitalInputHasChanged(DigitalInputT self) {
    bool current_state = DigitalInputGetState(self);
    int result = 0;

    if (current_state && !self->last_state) {
        result = 1;
    } else if (!current_state && self->last_state) {
        result = -1;
    }

    self->last_state = current_state;

    return result;
}

/**
 * @brief Indica si se detectó un flanco ascendente (activación).
 * 
 * @param self Objeto de entrada digital.
 * @return true si hubo flanco ascendente.
 */

bool DigitalInputHasActivate(DigitalInputT self) {
    return DigitalInputHasChanged(self) == 1;
}

/**
 * @brief Indica si se detectó un flanco descendente (desactivación).
 * 
 * @param self Objeto de entrada digital.
 * @return true si hubo flanco descendente.
 */

bool DigitalInputHasDeactivate(DigitalInputT self) {
    return DigitalInputHasChanged(self) == -1;
}

/* === End of documentation ======================================================================================== */

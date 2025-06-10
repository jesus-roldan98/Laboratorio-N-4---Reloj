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

typedef struct DigitalOutputS {

    int port;
    int pin;

} DigitalOutputS;

typedef struct DigitalInputS {

    int port;
    int pin;
    bool inverted;
    bool last_state;

} DigitalInputS;

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */
// salida
DigitalOutputT DigitalOutputCreate(int port, int pin) {
    DigitalOutputT self = malloc(sizeof(struct DigitalOutputS));
    if (self != NULL) {
        self->port = port;
        self->pin = pin;

        Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->port, self->pin, true);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->port, self->pin, true);
    }

    return self;
}

void DigitalOutputActivate(DigitalOutputT self) {

    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->port, self->pin, true);
}

void DigitalOutputDeactivate(DigitalOutputT self) {

    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->port, self->pin, false);
}

void DigitalOutputToggle(DigitalOutputT self) {

    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, self->port, self->pin);
}
// Etrada
DigitalInputT DigitalInputCreate(int port, int pin, bool inverted) {
    DigitalInputT self = malloc(sizeof(struct DigitalInputS));
    if (self != NULL) {
        self->port = port;
        self->pin = pin;
        self->inverted = inverted;
        self->last_state = false;

        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->port, self->pin, false);
    }
    return self;
}

bool DigitalInputGetState(DigitalInputT self) {
    bool state = Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, self->port, self->pin);

    if (self->inverted) {
        state = !state;
    }
    return state;
}

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

bool DigitalInputHasActivate(DigitalInputT self) {
    return DigitalInputHasChanged(self) == 1;
}

bool DigitalInputHasDeactivate(DigitalInputT self) {
    return DigitalInputHasChanged(self) == -1;
}

/* === Public function implementation ============================================================================== */

/* === End of documentation ======================================================================================== */

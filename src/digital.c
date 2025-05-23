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

typedef struct DigtalOutputS {
    
    int port;
    int pin;
    
};

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

DigitalOutputT DigitalOutputCreate(int port, int pin) {
    DigitalOutputT self = malloc(sizeof(struct DigtalOutputS));
    if (self != NULL) {
    self->port = port;
    self->pin = pin;
    
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self -> port, self -> pin, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self -> port, self -> pin, true);

    } 
    return self;

}

void DigitalOutputActivate (DigitalOutputT self) {

    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self -> port, self -> pin, true);

}

void DigitalOutputDeactivate (DigitalOutputT self) {

    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self -> port, self -> pin, false);

}

void DigitalOutputToggle (DigitalOutputT self) {

Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, self -> port, self -> pin);

}

/* === Public function implementation ============================================================================== */

/* === End of documentation ======================================================================================== */

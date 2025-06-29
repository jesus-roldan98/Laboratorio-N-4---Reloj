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

/** @file screen.c
 ** @brief implemntacion del modulo para la gestion de una pantalla multplexada de 7 segmentos
 **/

/* === Headers files inclusions ==================================================================================== */

#include "screen.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* === Macros definitions ========================================================================================== */

#ifndef SCREEN_MAX_DIGITS
#define SCREEN_MAX_DIGITS 8
#endif

/* === Private data type declarations ============================================================================== */

struct ScreenS {
    uint8_t digits;      //! <- cantidad de digitos de la pantalla
    uint8_t flashing_to; //! <- segmentos de la pantalla
    uint8_t current_digit;
    uint8_t flashing_from;
    uint8_t flashing_count;
    uint16_t flashing_frecuency;
    // flashing punto decimal
    uint8_t decimal_points_count;
    uint8_t decimal_points_from;
    uint8_t decimal_points_to;
    uint16_t decimal_points_frecuency;
    screen_driver_t driver;                          //! <- driver de la pantalla
    uint8_t value[SCREEN_MAX_DIGITS];                //! <- valores a mostrar en la pantalla
    uint8_t value_decimal_points[SCREEN_MAX_DIGITS]; //! <- puntos decimales para cada dígito
};

static const uint8_t IMAGES[10] = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,             // 0
    SEGMENT_B | SEGMENT_C,                                                             // 1
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,                         // 2
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,                         // 3
    SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,                                     // 4
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,                         // 5
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,             // 6
    SEGMENT_A | SEGMENT_B | SEGMENT_C,                                                 // 7
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G, // 8
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G              // 9
};

/* === Private function declarations =============================================================================== */

static uint8_t DisplayFlickerDigits(ScreenT screen);
static uint8_t DisplayFlickerPoints(ScreenT screen);

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

static uint8_t DisplayFlickerDigits(ScreenT screen) {
    uint8_t flick;
    flick = screen->value[screen->current_digit]; // Obtener los segmentos del digito actual
    if (screen->flashing_frecuency != 0) {
        if (screen->current_digit == 0) {

            screen->flashing_count = (screen->flashing_count + 1) % (screen->flashing_frecuency);
        }
        if (screen->flashing_count < (screen->flashing_frecuency / 2)) {
            if (screen->current_digit >= screen->flashing_from && screen->current_digit <= screen->flashing_to) {
                flick = 0; // Apagar los segmentos si estamos en la mitad del parpadeo
            }
        }
    }
    return flick;
}

static uint8_t DisplayFlickerPoints(ScreenT screen) {
    uint8_t flick;
    flick = screen->value_decimal_points[screen->current_digit]; // Obtener los segmentos del digito actual
    if (screen->decimal_points_frecuency != 0) {
        if (screen->current_digit == 0) {

            screen->decimal_points_count = (screen->decimal_points_count + 1) % (screen->decimal_points_frecuency);
        }
        if (screen->decimal_points_count < (screen->decimal_points_frecuency / 2)) {
            if (screen->current_digit >= screen->decimal_points_from &&
                screen->current_digit <= screen->decimal_points_to) {
                flick = 0; // Apagar los segmentos si estamos en la mitad del parpadeo
            }
        }
    }
    return flick;
}

/* === Public function implementation ============================================================================== */

ScreenT ScreenCreate(uint8_t digits, screen_driver_t driver) {
    ScreenT self = malloc(sizeof(struct ScreenS));
    if (digits > SCREEN_MAX_DIGITS) {
        digits = SCREEN_MAX_DIGITS; // Limitar a la cantidad maxima de digitos
    }
    if (self != NULL) {
        self->digits = digits;
        self->driver = driver;
        self->current_digit = 0; // Inicializar el digito actual
        self->flashing_frecuency = 0;
        self->flashing_count = 0;
        self->decimal_points_frecuency = 0;
        self->decimal_points_count = 0;
    }
    return self;
}

void ScreenWriteBCD(ScreenT self, uint8_t value[], uint8_t size, uint8_t value_decimal_points[]) {
    memset(self->value, 0, sizeof(self->value));                               // Limpiar el buffer de la pantalla
    memset(self->value_decimal_points, 0, sizeof(self->value_decimal_points)); // Limpiar los puntos decimales
    if (size > self->digits) {
        size = self->digits; // Limitar al tamaño de la pantalla
    }
    for (uint8_t i = 0; i < size; i++) {
    uint8_t pos = self->digits - 1 - i;  // invertir el orden
    self->value[pos] = IMAGES[value[i]];
    self->value_decimal_points[i] = value_decimal_points[i] ? SEGMENT_P : 0;
}

}

void ScreenRefresh(ScreenT screen) {
    uint8_t segments = screen->value[screen->current_digit];
    uint8_t points = screen->value_decimal_points[screen->current_digit];

    screen->driver->DigitsTurnOff();
    screen->current_digit = (screen->current_digit + 1) % screen->digits; // Incrementar el digito actual
    segments = DisplayFlickerDigits(screen);                              // Obtener los segmentos del digito actual
    points = DisplayFlickerPoints(screen);                                // Obtener los puntos del digito actual
    screen->driver->SegmentsUpdate(segments, points);                     // Actualizar los segmentos de la pantalla
    screen->driver->DigitsTurnOn(screen->current_digit);                  // Apagar el digito actual
}

int DisplayFlashDigits(ScreenT display, uint8_t from, uint8_t to, uint16_t divisor) {

    int result = 0;
    if ((from > to) || (from >= SCREEN_MAX_DIGITS) || (to >= SCREEN_MAX_DIGITS)) {
        result = -1;
    } else if (!display) {
        result = -1;
    } else {
        display->flashing_from = from;
        display->flashing_to = to;
        display->flashing_frecuency = 2 * divisor;
        display->flashing_count = 0;
    }
    return result;
}

int DisplayFlashPoints(ScreenT display, uint8_t from, uint8_t to, uint16_t divisor) {

    int result = 0;
    if ((from > to) || (from >= SCREEN_MAX_DIGITS) || (to >= SCREEN_MAX_DIGITS)) {
        result = -1;
    } else if (!display) {
        result = -1;
    } else {
        display->decimal_points_from = from;
        display->decimal_points_to = to;
        display->decimal_points_frecuency = 2 * divisor;
        display->decimal_points_count = 0;
    }
    return result;
}

/* === End of documentation ======================================================================================== */

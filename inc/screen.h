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

#ifndef SCREEN_H_
#define SCREEN_H_

/** @file screen.h
 ** @brief Declaraciones del modulo para la gestion de una pantalla multplexada de 7 segmentos
 **/

/* === Headers files inclusions ==================================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

#define SEGMENT_A (1 << 0)
#define SEGMENT_B (1 << 1)
#define SEGMENT_C (1 << 2)
#define SEGMENT_D (1 << 3)
#define SEGMENT_E (1 << 4)
#define SEGMENT_F (1 << 5)
#define SEGMENT_G (1 << 6)
#define SEGMENT_P (1 << 7)

/* === Public data type declarations =============================================================================== */

typedef struct ScreenS * ScreenT; //! <- tipo de dato para la pantalla

typedef void (*digits_turn_off_t)(void); // Tipo de funcion para apagar los digitos

typedef void (*segments_update_t)(uint8_t, uint8_t); // Tipo de funcion para actualizar los segmentos

typedef void (*digits_turn_on_t)(uint8_t); // Tipo de funcion para prender los digitos

typedef struct screen_driver_s {
    digits_turn_off_t DigitsTurnOff;  // Funcion para apagar los digitos
    segments_update_t SegmentsUpdate; // Funcion para actualizar los segmentos
    digits_turn_on_t DigitsTurnOn;    // Funcion para prender los digitos
} const * screen_driver_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

ScreenT ScreenCreate(uint8_t digits, screen_driver_t driver);

void ScreenWriteBCD(ScreenT screen, uint8_t value[], uint8_t size, uint8_t value_decimal_points[]);

void ScreenRefresh(ScreenT screen);

/*@brief Funcion que indica cuando pintar y cuando no pintar la pantalla
 * @param display puntero al descriptor de la pantalla
 * @param digit numero del digito a encender (0-3)
 */

int DisplayFlashDigits(ScreenT display, uint8_t from, uint8_t to, uint16_t divisor);
int DisplayFlashPoints(ScreenT display, uint8_t from, uint8_t to, uint16_t divisor);

/**
 * @brief Establece el estado de encendido del punto de un dígito específico.
 *
 * @param digit Índice del dígito [0–3].
 * @param on true para encender el punto, false para apagarlo.
 */
void DisplaySetPoint(uint8_t digit, bool on);

/**
 * @brief Configura un punto para que parpadee.
 *
 * @param digit Índice del dígito [0–3].
 * @param flash true para parpadear, false para mostrar estado estático.
 */

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* SCREEN_H_ */

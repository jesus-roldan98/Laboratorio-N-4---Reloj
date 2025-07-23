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
#include "clock.h"

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/**
 * @name Máscaras de bits para los segmentos del display de 7 segmentos
 * @{
 */

#define SEGMENT_A (1 << 0) //!< Segmento A
#define SEGMENT_B (1 << 1) //!< Segmento B
#define SEGMENT_C (1 << 2) //!< Segmento C
#define SEGMENT_D (1 << 3) //!< Segmento D
#define SEGMENT_E (1 << 4) //!< Segmento E
#define SEGMENT_F (1 << 5) //!< Segmento F
#define SEGMENT_G (1 << 6) //!< Segmento G
#define SEGMENT_P (1 << 7) //!< Punto decimal

/* === Public data type declarations =============================================================================== */

/**
 * @brief Tipo opaco que representa una pantalla multiplexada
 */

typedef struct ScreenS * ScreenT; 

/**
 * @brief Puntero a función que apaga todos los dígitos del display
 */

typedef void (*digits_turn_off_t)(void); 

/**
 * @brief Puntero a función que actualiza los segmentos de un dígito
 * @param digit Índice del dígito a actualizar
 * @param segments Máscara de segmentos a encender
 */

typedef void (*segments_update_t)(uint8_t, uint8_t); 

/**
 * @brief Puntero a función que enciende un dígito específico
 * @param digit Índice del dígito a encender
 */

typedef void (*digits_turn_on_t)(uint8_t); 

/**
 * @brief Estructura que define el controlador de hardware de la pantalla
 */

typedef struct screen_driver_s {
    digits_turn_off_t DigitsTurnOff;   /**< Función para apagar todos los dígitos */
    segments_update_t SegmentsUpdate; /**< Función para actualizar los segmentos de un dígito */
    digits_turn_on_t DigitsTurnOn;    /**< Función para encender un dígito específico */
} const * screen_driver_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */


/**
 * @brief Crea una pantalla multiplexada con la cantidad de dígitos indicada
 *
 * @param digits Cantidad de dígitos de la pantalla
 * @param driver Controlador de hardware (funciones para manejar los dígitos y segmentos)
 * @return Objeto pantalla creado
 */

ScreenT ScreenCreate(uint8_t digits, screen_driver_t driver);

/**
 * @brief Escribe valores BCD en los dígitos de la pantalla
 *
 * @param screen Pantalla a actualizar
 * @param value Vector de valores BCD a mostrar
 * @param size Tamaño del vector de valores
 * @param value_decimal_points Vector con información de puntos decimales encendidos (1) o apagados (0)
 */

void ScreenWriteBCD(ScreenT self, const clock_time_t * time, bool show_seconds, uint8_t decimal_points[]);

/**
 * @brief Actualiza la pantalla. Debe llamarse periódicamente (ej. en interrupción de SysTick)
 *
 * @param screen Pantalla a refrescar
 */

void ScreenRefresh(ScreenT screen);

/**
 * @brief Controla el parpadeo de los dígitos dentro de un rango
 *
 * @param display Pantalla sobre la que se actúa
 * @param from Índice inicial del rango
 * @param to Índice final del rango
 * @param divisor Factor de tiempo que define la velocidad del parpadeo
 * @return Estado del parpadeo (1: mostrar, 0: ocultar)
 */

int DisplayFlashDigits(ScreenT display, uint8_t from, uint8_t to, uint16_t divisor);

/**
 * @brief Controla el parpadeo de los puntos decimales dentro de un rango
 *
 * @param display Pantalla sobre la que se actúa
 * @param from Índice inicial del rango
 * @param to Índice final del rango
 * @param divisor Factor de tiempo que define la velocidad del parpadeo
 * @return Estado del parpadeo (1: mostrar, 0: ocultar)
 */

int DisplayFlashPoints(ScreenT display, uint8_t from, uint8_t to, uint16_t divisor);

/**
 * @brief Configura un punto decimal para que parpadee
 *
 * @param digit Índice del dígito (0 a 3)
 * @param flash true para parpadeo, false para estado estático
 */

void DisplaySetPoint(uint8_t digit, bool on);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* SCREEN_H_ */

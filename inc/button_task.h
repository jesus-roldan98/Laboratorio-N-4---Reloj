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

#ifndef BUTTON_TASK_H_
#define BUTTON_TASK_H_

/** @file button_task.h
 ** @brief Declaraciones para la capa de soporte de hardware
 **/

/* === Headers files inclusions ==================================================================================== */

#include "board.h"      // <-- para BoardT
#include "bsp.h"
#include "FreeRTOS.h"
#include "queue.h"

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

/**
 * @enum event_t
 * @brief Tipos de eventos que puede generar la interfaz de usuario mediante los botones.
 */

typedef enum {
    EV_SET_TIME,
    EV_SET_ALARM,
    EV_ACCEPT,
    EV_CANCEL,
    EV_INCREMENT,
    EV_DECREMENT
} event_t;

/**
 * @struct app_event_t
 * @brief Estructura que encapsula un evento generado por los botones.
 */

typedef struct {
    event_t type;
} app_event_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Inicializa la tarea de botones.
 *
 * Esta función configura la tarea de FreeRTOS que se encarga de leer los botones del sistema y enviar
 * eventos al queue especificado.
 *
 * @param board Instancia de la placa a la que pertenecen los botones.
 * @param xEvtQueue Handle de la cola de eventos donde se publicarán los eventos detectados.
 */

void ButtonTaskInit(BoardT board, QueueHandle_t xEvtQueue);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* BUTTON_TASK_H_ */

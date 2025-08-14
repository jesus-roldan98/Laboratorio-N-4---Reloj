/*********************************************************************************************************************
Copyright (c) 2025, TuNombre <tucorreo@example.com>

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

#ifndef DISPLAY_TASK_H_
#define DISPLAY_TASK_H_

/* === Headers files inclusions ==================================================================================== */

#include "screen.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/* === Public data type declarations =============================================================================== */

typedef struct display_task_args_s {
    ScreenT screen;
    QueueHandle_t time_queue;
    QueueHandle_t state_queue;
    QueueHandle_t alarm_queue;
    SemaphoreHandle_t mutex;
} display_task_args_t;

/* === Public function declarations ================================================================================ */

/**
 * @brief Inicializa las tareas relacionadas con la visualización en pantalla
 * @param args Argumentos para las tareas (cola de tiempo, cola de estado, mutex, etc.)
 */
void DisplayTaskInit(display_task_args_t * args);

#endif /* DISPLAY_TASK_H_ */
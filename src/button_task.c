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

#include "button_task.h"
#include "digital.h"
#include "FreeRTOS.h"
#include "task.h"


/* === Macros definitions ========================================================================================== */



/* === Private data type declarations ============================================================================== */


/* === Private function declarations =============================================================================== */

static BoardT s_board;
static QueueHandle_t s_evtQueue;

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

static void vButtonTask(void *pvParameters);

/* === Public function implementation ============================================================================== */

void ButtonTaskInit(BoardT board, QueueHandle_t xEvtQueue) {
    s_board = board;
    s_evtQueue = xEvtQueue;

    // Crear la tarea de FreeRTOS
    xTaskCreate(vButtonTask, "Buttons", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
}

static void vButtonTask(void *pvParameters) {
    const TickType_t debounce = pdMS_TO_TICKS(30);
    app_event_t ev;
    TickType_t last_input_tick __attribute__((unused)) = 0;

    for (;;) {
        if (DigitalInputHasActivate(s_board->set_time)) {
            ev.type = EV_SET_TIME; xQueueSend(s_evtQueue, &ev, 0);
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        } else if (DigitalInputHasActivate(s_board->set_alarm)) {
            ev.type = EV_SET_ALARM; xQueueSend(s_evtQueue, &ev, 0);
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        } else if (DigitalInputHasActivate(s_board->accept)) {
            ev.type = EV_ACCEPT; xQueueSend(s_evtQueue, &ev, 0);
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        } else if (DigitalInputHasActivate(s_board->cancel)) {
            ev.type = EV_CANCEL; xQueueSend(s_evtQueue, &ev, 0);
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        } else if (DigitalInputHasActivate(s_board->increment)) {
            ev.type = EV_INCREMENT; xQueueSend(s_evtQueue, &ev, 0);
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        } else if (DigitalInputHasActivate(s_board->decrement)) {
            ev.type = EV_DECREMENT; xQueueSend(s_evtQueue, &ev, 0);
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        }

        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

/* === End of documentation ======================================================================================== */

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

#include "clock_task.h"

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function implementation ============================================================================== */

void vClockTask(void *pvParameters) {
    for (;;) {
        ClockNewTick(clock);  // el RTC simulado sigue corriendo

        if (xSemaphoreTake(xStateMutex, portMAX_DELAY)) {
            // SOLO actualizar time_clock desde el reloj en estados que no son de edición
            if (state == STATE_NORMAL || state == STATE_CLOCK_INIT) {
                ClockGetTime(clock, &time_clock);
            }
            xSemaphoreGive(xStateMutex);
        }

        HandleAlarm();  // función de la alarma (después la moveremos a su archivo)

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

uint32_t ClockGetTicks(void) {
    return xTaskGetTickCount();
}

/* === End of documentation ======================================================================================== */

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

/** @file display_task.c
 ** @brief Implementación de tareas para la actualización de la pantalla del reloj digital
 **/

/* === Headers files inclusions ==================================================================================== */

#include "display_task.h"
#include "screen.h"
#include "clock.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* === Macros definitions ========================================================================================== */

#define DISPLAY_REFRESH_RATE_MS    1   // Tiempo entre refrescos de pantalla
#define TIME_UPDATE_RATE_MS        50  // Tiempo entre actualizaciones de tiempo

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

static void RefreshScreenTask(void * params);
static void UpdateTimeTask(void * params);
static void UpdateStateTask(void * params);
static void UpdateAlarmTask(void * params);

/* === Public function implementation ============================================================================== */

void DisplayTaskInit(display_task_args_t * args) {
    // Crear tarea de refresco de pantalla
    xTaskCreate(RefreshScreenTask, "Refresh", configMINIMAL_STACK_SIZE, 
               args, tskIDLE_PRIORITY + 1, NULL);

    // Crear tarea de actualización de tiempo
    xTaskCreate(UpdateTimeTask, "TimeUpdate", configMINIMAL_STACK_SIZE, 
               args, tskIDLE_PRIORITY + 1, NULL);

    // Crear tarea de actualización de estado
    xTaskCreate(UpdateStateTask, "StateUpdate", configMINIMAL_STACK_SIZE, 
               args, tskIDLE_PRIORITY + 1, NULL);

    // Crear tarea de actualización de alarma (opcional)
    xTaskCreate(UpdateAlarmTask, "AlarmUpdate", configMINIMAL_STACK_SIZE, 
               args, tskIDLE_PRIORITY + 1, NULL);
}

/* === Private function implementation ============================================================================= */

static void RefreshScreenTask(void * params) {
    display_task_args_t * args = (display_task_args_t *)params;
    TickType_t last_wake_time = xTaskGetTickCount();

    while(1) {
        // Refrescar la pantalla periódicamente
        ScreenRefresh(args->screen);
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(DISPLAY_REFRESH_RATE_MS));
    }
}

static void UpdateTimeTask(void * params) {
    display_task_args_t * args = (display_task_args_t *)params;
    clock_time_t current_time;
    uint8_t decimal_points[4] = {0, 0, 0, 0}; // Puntos decimales apagados por defecto

    while(1) {
        // Recibir actualización de tiempo desde la cola
        if(xQueueReceive(args->time_queue, &current_time, portMAX_DELAY) == pdTRUE) {
            xSemaphoreTake(args->mutex, portMAX_DELAY);
            
            // Mostrar horas y minutos (sin segundos)
            ScreenWriteBCD(args->screen, &current_time, false, decimal_points);
            
            xSemaphoreGive(args->mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(TIME_UPDATE_RATE_MS));
    }
}

static void UpdateStateTask(void * params) {
    display_task_args_t * args = (display_task_args_t *)params;
    char state;
    uint8_t decimal_points[4] = {0, 0, 1, 0}; // Puntos decimales para indicar estado
    TickType_t last_toggle = xTaskGetTickCount();
    bool toggle_state = false;

    while(1) {
        if(xQueueReceive(args->state_queue, &state, 0) == pdTRUE) {
            // Procesar nuevo estado si se recibió
            if(state == 'R') { // Running
                // Parpadear los dos puntos centrales
                toggle_state = !toggle_state;
                decimal_points[2] = toggle_state ? 1 : 0;
                
                xSemaphoreTake(args->mutex, portMAX_DELAY);
                DisplayFlashPoints(args->screen, 2, 2, 500); // Parpadeo cada 500ms
                xSemaphoreGive(args->mutex);
            } else if(state == 'S') { // Stopped
                // Apagar los puntos
                decimal_points[2] = 0;
                
                xSemaphoreTake(args->mutex, portMAX_DELAY);
                DisplayFlashPoints(args->screen, 2, 2, 0); // Sin parpadeo
                xSemaphoreGive(args->mutex);
            }
        }
        
        vTaskDelayUntil(&last_toggle, pdMS_TO_TICKS(500));
    }
}

static void UpdateAlarmTask(void * params) {
    display_task_args_t * args = (display_task_args_t *)params;
    bool alarm_active;
    uint8_t decimal_points[4] = {0, 0, 0, 1}; // Punto decimal derecho para alarma

    while(1) {
        if(xQueueReceive(args->alarm_queue, &alarm_active, 0) == pdTRUE) {
            xSemaphoreTake(args->mutex, portMAX_DELAY);
            
            if(alarm_active) {
                // Parpadear todo el display cuando la alarma está activa
                DisplayFlashDigits(args->screen, 0, 3, 250); // Parpadeo rápido
                decimal_points[3] = 1;
            } else {
                DisplayFlashDigits(args->screen, 0, 3, 0); // Sin parpadeo
                decimal_points[3] = 0;
            }
            
            xSemaphoreGive(args->mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/* === End of documentation ======================================================================================== */
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

/** @file main.c
 ** @brief Implementación principal del reloj digital con alarma
 ** @author Roldan JesusAlejandro
 ** @date Agosto 2025
 **/

/* === Headers files inclusions =============================================================== */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "bsp.h"
#include "clock.h"
#include "screen.h"
#include "digital.h"

/* === Macros definitions ====================================================================== */
#define configTICK_RATE_HZ        ((TickType_t)1000)   // 1ms tick rate
#define configMINIMAL_STACK_SIZE  ((uint16_t)128)
#define configTOTAL_HEAP_SIZE     ((size_t)(16 * 1024)) // 16KB heap

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */
static void vTaskClock(void *pvParameters);
static void vTaskScreen(void *pvParameters);
static void vTaskKeys(void *pvParameters);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/**
 * @brief Tarea encargada de actualizar el reloj
 */
static void vTaskClock(void *pvParameters) {
    clock_t clock = (clock_t) pvParameters;

    for (;;) {
        ClockNewTick(clock);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 segundo
    }
}

/**
 * @brief Tarea encargada de refrescar la pantalla
 */
static void vTaskScreen(void *pvParameters) {
    BoardT board = (BoardT) pvParameters;
    const TickType_t xFrequency = pdMS_TO_TICKS(1);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        ScreenRefresh(board->screen);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/**
 * @brief Tarea encargada de leer y procesar teclas
 */
static void vTaskKeys(void *pvParameters) {
    BoardT board = (BoardT) pvParameters;

    for (;;) {
        // Aquí puedes implementar la lógica para leer los botones
        // Ejemplo:
        if (DigitalInputHasActivate(board->set_time)) {
            // Lógica para manejar el botón set_time
        }
        // ... manejo de otros botones
        
        vTaskDelay(pdMS_TO_TICKS(50)); // cada 50 ms
    }
}

/* === Public function implementation ========================================================= */

/**
 * @brief Función principal del programa
 * @return int Siempre retorna 0 (no utilizado en sistemas embebidos)
 */
int main(void) {

    clock_time_t time_clock;
    //clock_time_t time_alarm;
    // Inicializar hardware
    BoardT board = BoardCreate();

    // Crear reloj con 1000 ticks por segundo
    clock_t clock = ClockCreate(1000);

    // Configurar hora inicial
    ClockGetTime(clock, &time_clock);

    // Crear tareas de FreeRTOS
    xTaskCreate(vTaskClock, "Clock", configMINIMAL_STACK_SIZE, (void *)clock, 1, NULL);
    xTaskCreate(vTaskScreen, "Screen", configMINIMAL_STACK_SIZE, (void *)board, 1, NULL);
    xTaskCreate(vTaskKeys, "Keys", configMINIMAL_STACK_SIZE, (void *)board, 1, NULL);

    // Iniciar el planificador
    vTaskStartScheduler();
    
    while(1);
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
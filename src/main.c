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
//hasta aqui el reloj funciona bien
/** @file main.c
 ** @brief Implementación principal del reloj digital con alarma
 ** @author Roldan JesusAlejandro
 ** @date Agosto 2025
 **/

/* === Headers files inclusions =============================================================== */

#include "bsp.h"
#include "screen.h"
#include "clock.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* === Macros definitions ====================================================================== */

#define TASK_CLOCK_LOGIC_PRIORITY        (tskIDLE_PRIORITY + 1)
#define TASK_REFRESH_DISPLAY_PRIORITY    (tskIDLE_PRIORITY + 2)
#define TASK_KEYPAD_PRIORITY             (tskIDLE_PRIORITY + 3)

/* === Private data type declarations ========================================================== */

/**
 * @enum clock_state_t
 * @brief Estados de la máquina de estados del reloj
 */

typedef enum {
    STATE_CLOCK_INIT,        // Estado inicial: inicialización del reloj
    STATE_NORMAL,            // Estado principal: mostrar hora actual
    STATE_SET_HOURS,         // Estado para configurar horas
    STATE_SET_MINUTES,       // Estado para configurar minutos
    STATE_SET_ALARM_HOURS,   // Configurar alarma - horas
    STATE_SET_ALARM_MINUTES, // Configurar alarma - minutos
    
} clock_state_t;

typedef enum {
    EVENT_NONE,
    EVENT_SET_TIME,
    EVENT_SET_ALARM,
    EVENT_INCREMENT,
    EVENT_DECREMENT,
    EVENT_ACCEPT,
    EVENT_CANCEL,
    EVENT_TICK
} clock_event_t;

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

static void vDisplayTask(void *pvParameters);
static void vClockTask(void *pvParameters);
//static void ProcessButtonEvents(void);
static void ClockStates(clock_state_t mode);
static void UpBCDAdjusted(uint8_t numero[2], bool is_hours);
//static void DownBCDAdjusted(uint8_t numero[2], bool is_hours);
//static void CancelAlarm(void);
//static void HandleAlarm(void);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

static BoardT board;                     ///< Estructura de la placa
static clock_t clock;                    ///< Variable para almacenar el reloj simulado
static clock_state_t state;              ///< Estado actual del reloj
//static uint32_t blink_counter = 0;       ///< Contador para controlar el parpadeo
//static bool alarm_active = false;        ///< Estado de la alarma
static bool show_dot = true;             ///< Control para mostrar/ocultar puntos
static clock_time_t time_clock = {
    .time = {
        .seconds = {0, 0},
        .minutes = {0, 0},
        .hours = {0, 0}
    }
};

static TaskHandle_t xDisplayTaskHandle = NULL;
static TaskHandle_t xClockTaskHandle = NULL;
static QueueHandle_t xEventQueue = NULL;
static TaskHandle_t xKeypadTaskHandle = NULL;

/* === Private function implementation ========================================================= */

static void vDisplayTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1) {
        ScreenRefresh(board->screen); // Solo refresca, no reescribe todo
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5));
    }
}

static void vClockTask(void *pvParameters) {
    clock_event_t event;
    while(1) {
        if(xQueueReceive(xEventQueue, &event, portMAX_DELAY) == pdPASS) {
            switch(event) {
                case EVENT_SET_TIME:
                    if(state == STATE_NORMAL) {
                        ClockGetTime(clock, &time_clock);
                        ClockStates(STATE_SET_MINUTES);
                    }
                    break;
                case EVENT_INCREMENT:
                    if(state == STATE_SET_MINUTES)
                        UpBCDAdjusted(time_clock.time.minutes,false);
                    else if(state == STATE_SET_HOURS)
                        UpBCDAdjusted(time_clock.time.hours,true);
                    break;
                case EVENT_ACCEPT:
                    if(state == STATE_SET_MINUTES)
                        ClockStates(STATE_SET_HOURS);
                    else if(state == STATE_SET_HOURS) {
                        ClockSetTime(clock, &time_clock);
                        ClockStates(STATE_NORMAL);
                    }
                    break;
                
                default:
                    break;
            }
        }
    }
}

/**
 * @brief Tarea para el manejo del teclado
 * @param pvParameters Parámetros de la tarea (no usados)
 */
static void vKeypadTask(void *pvParameters) {
    const TickType_t xDebounceDelay = pdMS_TO_TICKS(20); // Retardo antirebote
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    while(1) {
        clock_event_t event = EVENT_NONE;
        
        // Leer botones con antirebote
        if(DigitalInputHasActivate(board->set_time)) {
            event = EVENT_SET_TIME;
        }
        else if(DigitalInputHasActivate(board->set_alarm)) {
            event = EVENT_SET_ALARM;
        }
        else if(DigitalInputHasActivate(board->increment)) {
            event = EVENT_INCREMENT;
        }
        else if(DigitalInputHasActivate(board->decrement)) {
            event = EVENT_DECREMENT;
        }
        else if(DigitalInputHasActivate(board->accept)) {
            event = EVENT_ACCEPT;
        }
        else if(DigitalInputHasActivate(board->cancel)) {
            event = EVENT_CANCEL;
        }
        
        // Enviar evento si se presionó algún botón
        if(event != EVENT_NONE) {
            xQueueSend(xEventQueue, &event, 0);
            vTaskDelay(xDebounceDelay); // Esperar para evitar rebotes
        }
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10)); // Ciclo cada 10ms
    }
}

/**
 * @brief Maneja los diferentes estados del reloj
 * @param mode Estado actual del reloj
 */

static void ClockStates(clock_state_t mode) {

    state = mode;

    uint8_t decimal_points[4] = {0, 0, 0, 0};

    switch (state) {
    case STATE_CLOCK_INIT:

        DisplayFlashDigits(board->screen, 0, 3, 50);
        DisplayFlashPoints(board->screen, 1, 1, 100);

        break;

    case STATE_NORMAL:

        ClockGetTime(clock, &time_clock);
        decimal_points[1] = show_dot ? 1 : 0;
        decimal_points[3] = ClockIsAlarmEnabled(clock) ? 1 : 0;
        ScreenWriteBCD(board->screen, &time_clock, false, decimal_points);
        break;

    case STATE_SET_HOURS:

        DisplayFlashDigits(board->screen, 0, 1, 50);
        DisplayFlashPoints(board->screen, 0, 3, 0);
        break;

    case STATE_SET_MINUTES:

        DisplayFlashDigits(board->screen, 2, 3, 50);
        DisplayFlashPoints(board->screen, 0, 3, 0);
        break;

    case STATE_SET_ALARM_HOURS:
        DisplayFlashDigits(board->screen, 0, 3, 50);
        DisplayFlashPoints(board->screen, 0, 3, 0);
        break;

    case STATE_SET_ALARM_MINUTES:

        DisplayFlashDigits(board->screen, 0, 3, 50);
        DisplayFlashPoints(board->screen, 0, 3, 0);

        break;
    }
}

/**
 * @brief Incrementa un número BCD con ajuste para horas o minutos
 * @param numero Array de 2 elementos con el número BCD (unidades, decenas)
 * @param is_hours Indica si es un valor de horas (ajusta a 23) o minutos (ajusta a 59)
 */

void UpBCDAdjusted(uint8_t numero[2], bool is_hours) {
    uint8_t temp[2] = {numero[1], numero[0]}; // Invertimos para trabajar
    
    // Incrementamos como decimal normal
    uint16_t value = temp[0] * 10 + temp[1];
    value = (value + 1) % (is_hours ? 24 : 60);
    
    // Convertimos de vuelta a BCD invertido
    temp[0] = value / 10; // Decenas
    temp[1] = value % 10; // Unidades
    
    numero[0] = temp[1]; // Restauramos orden original
    numero[1] = temp[0];
}

/**
 * @brief Decrementa un número BCD con ajuste para horas o minutos
 * @param numero Array de 2 elementos con el número BCD (unidades, decenas)
 * @param is_hours Indica si es un valor de horas (ajusta a 23) o minutos (ajusta a 59)
 */

// void DownBCDAdjusted(uint8_t numero[2], bool is_hours) {
//     uint8_t temp[2] = {numero[1], numero[0]}; // Invertimos
    
//     uint16_t value = temp[0] * 10 + temp[1];
//     value = (value == 0) ? (is_hours ? 23 : 59) : value - 1;
    
//     temp[0] = value / 10;
//     temp[1] = value % 10;
    
//     numero[0] = temp[1];
//     numero[1] = temp[0];
// }

/**
 * @brief Maneja la activación de la alarma
 */

// void HandleAlarm(void) {
//     // Verificar si debemos activar la alarma
//     if (!alarm_active && ClockIsAlarmEnabled(clock) && ClockAlarmMatchTheTime(clock) && (state == STATE_NORMAL)) {
//         alarm_active = true;
//         DigitalOutputActivate(board->led_green);  // Encender LED de alarma
//     }
    
//     // La alarma permanecerá activa hasta que el usuario la cancele
// }

/**
 * @brief Cancela la alarma activa
 */

//void CancelAlarm(void) {
//    if (alarm_active) {
//        alarm_active = false;
//        DigitalOutputDeactivate(board->led_green);  // Apagar LED de alarma
//        
//    }
//}


/* === Public function implementation ========================================================= */

/**
 * @brief Función principal del programa
 * @return int Siempre retorna 0 (no utilizado en sistemas embebidos)
 */

int main(void) {

    
    clock_time_t time_alarm;

    board = BoardCreate();     // Crear la estructura de la placa y asignar los pines a los leds y botones
    clock = ClockCreate(1000); // Crear el objeto reloj

    

    show_dot = false;

    // Forzar que el display arranque en 0000 con puntos apagados
    ScreenWriteBCD(board->screen, &time_clock, false, (uint8_t[4]){0,0,0,0});

    
    
    ClockGetTime(clock, &time_clock); // Obtener la hora actual del reloj
    ClockGetAlarm(clock,&time_alarm);
    ClockDisableAlarm(clock); 

    SysTickInit(1000);
    //ClockStates(STATE_CLOCK_INIT);

    // Crear cola para eventos
    xEventQueue = xQueueCreate(10, sizeof(clock_event_t));

    // Crear tareas
    xTaskCreate(vClockTask, "Clock", configMINIMAL_STACK_SIZE * 2, NULL, TASK_CLOCK_LOGIC_PRIORITY, &xClockTaskHandle);
    xTaskCreate(vDisplayTask, "Display", configMINIMAL_STACK_SIZE * 2, NULL, TASK_REFRESH_DISPLAY_PRIORITY, &xDisplayTaskHandle);
    xTaskCreate(vKeypadTask, "Keypad", configMINIMAL_STACK_SIZE * 2, NULL, TASK_KEYPAD_PRIORITY, &xKeypadTaskHandle);

    // Iniciar el planificador
    vTaskStartScheduler();

    while(1);
    
}


/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
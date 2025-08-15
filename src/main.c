/***************************************
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
***************************************/

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
    EV_SET_TIME,
    EV_SET_ALARM,
    EV_ACCEPT,
    EV_CANCEL,
    EV_INCREMENT,
    EV_DECREMENT
} event_t;

typedef struct {
    event_t type;
} app_event_t;

typedef enum {
    ALARM_CHECK,
    ALARM_ACTIVATE,
    ALARM_DEACTIVATE
} alarm_event_t;

/* === Private variable declarations =========================================================== */

static BoardT board;                                        ///< Estructura de la placa
static uint8_t decimal_points [4] = {0, 0, 0, 0};           ///< Puntos decimales
static clock_time_t time_clock;                             ///< Hora actual del reloj
static clock_time_t time_alarm;                             ///< Hora de la alarma
static clock_t clock;                                       ///< Variable para almacenar el reloj simulado
static clock_state_t state = STATE_CLOCK_INIT;              ///< Estado actual del reloj

static bool show_dot = true;                                ///< Control para mostrar/ocultar puntos
static bool alarm_enabled = false;
static bool alarm_triggered = false;


static QueueHandle_t xEvtQ;
static SemaphoreHandle_t xStateMutex;
static TaskHandle_t xAlarmTaskHandle = NULL;
static QueueHandle_t xAlarmQueue = NULL;
static TickType_t last_input_tick = 0;                      ///< Último tick que hubo interacción

/* === Private function declarations =========================================================== */

void UpBCDAdjusted(uint8_t numero[2], bool is_hours);
void DownBCDAdjusted(uint8_t numero[2], bool is_hours);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

static void vAlarmTask(void *pvParameters) {
    alarm_event_t event;
    
    while(1) {
        if(xQueueReceive(xAlarmQueue, &event, pdMS_TO_TICKS(1000)) == pdPASS) {
            switch(event) {
                case ALARM_CHECK:
                    if(!alarm_triggered && alarm_enabled && 
                       ClockAlarmMatchTheTime(clock) && (state == STATE_NORMAL)) {
                        alarm_triggered = true;
                        DigitalOutputActivate(board->led_green);
                    }
                    break;
                    
                case ALARM_ACTIVATE:
                    alarm_triggered = true;
                    DigitalOutputActivate(board->led_green);
                    break;
                    
                case ALARM_DEACTIVATE:
                    alarm_triggered = false;
                    DigitalOutputDeactivate(board->led_green);
                    break;
            }
        }
    }
}

void HandleAlarm(void) {
    alarm_event_t event = ALARM_CHECK;
    xQueueSend(xAlarmQueue, &event, 0);
}

void CancelAlarm(void) {
    alarm_event_t event = ALARM_DEACTIVATE;
    xQueueSend(xAlarmQueue, &event, 0);
}



/**
 * @brief Maneja los diferentes estados del reloj
 * @param mode Estado actual del reloj
 */

void ClockStates(clock_state_t mode) {

    state = mode;

    switch (state) {
    case STATE_CLOCK_INIT:

        DisplayFlashDigits(board->screen, 0, 3, 100);
        DisplayFlashPoints(board->screen, 1, 1, 100);

        break;

    case STATE_NORMAL:

        DisplayFlashDigits(board->screen, 0, 0, 0);
        DisplayFlashPoints(board->screen, 0, 0, 0);
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
        DisplayFlashDigits(board->screen, 0, 1, 50);
        DisplayFlashPoints(board->screen, 0, 3, 0);
        break;

    case STATE_SET_ALARM_MINUTES:

        DisplayFlashDigits(board->screen, 2, 3, 50);
        DisplayFlashPoints(board->screen, 0, 3, 0);

        break;
    }
}

static void vStateMachineTask(void *pvParameters) {
    app_event_t ev;

    ClockStates(STATE_CLOCK_INIT);
    if (xSemaphoreTake(xStateMutex, portMAX_DELAY)) {
        ClockGetTime(clock, &time_clock);
        ClockGetAlarm(clock, &time_alarm);
        xSemaphoreGive(xStateMutex);
    }

    for (;;) {
        if (xQueueReceive(xEvtQ, &ev, portMAX_DELAY)) {

            if (xSemaphoreTake(xStateMutex, portMAX_DELAY)) {

                switch (ev.type) {
                case EV_SET_TIME:
                    if (state == STATE_NORMAL || state == STATE_CLOCK_INIT) {
                        ClockGetTime(clock, &time_clock);
                        ClockStates(STATE_SET_MINUTES);
                    }
                    
                    break;

                case EV_SET_ALARM:
                    if (state == STATE_NORMAL) {
                        ClockGetAlarm(clock, &time_alarm);
                        ClockStates(STATE_SET_ALARM_MINUTES);
                    }
                    break;

                case EV_ACCEPT:
                    if (state == STATE_SET_MINUTES) {
                        ClockStates(STATE_SET_HOURS);
                    } else if (state == STATE_SET_HOURS) {
                        time_clock.time.seconds[0] = 0; time_clock.time.seconds[1] = 0;
                        ClockSetTime(clock, &time_clock);
                        ClockStates(STATE_NORMAL);
                    } else if (state == STATE_SET_ALARM_MINUTES) {
                        ClockStates(STATE_SET_ALARM_HOURS);
                    } else if (state == STATE_SET_ALARM_HOURS) {
                        time_alarm.time.seconds[0] = 0; time_alarm.time.seconds[1] = 0;
                        ClockSetAlarm(clock, &time_alarm);
                        ClockEnableAlarm(clock);
                        alarm_enabled = true;
                        ClockStates(STATE_NORMAL);
                    } else if (state == STATE_NORMAL && alarm_triggered) {
                        ClockPostponeAlarm(clock, 5);
                        alarm_triggered = false;
                        DigitalOutputDeactivate(board->led_green);
                    } else if (state == STATE_NORMAL && !alarm_triggered) {
                        ClockEnableAlarm(clock);
                        alarm_enabled = true;
                        decimal_points[3] = 1;
                    }
                    break;

                case EV_CANCEL:
                    if (state == STATE_NORMAL && alarm_triggered) {
                        ClockCancelAlarmToday(clock);
                        alarm_triggered = false;
                        decimal_points[3] = 1;
                        DigitalOutputDeactivate(board->led_green);
                    } else if (state == STATE_NORMAL && !alarm_triggered) {
                        ClockDisableAlarm(clock);
                        alarm_enabled = false;
                        decimal_points[3] = 0;
                    } else if (state == STATE_SET_ALARM_HOURS || state == STATE_SET_ALARM_MINUTES) {
                        ClockStates(STATE_NORMAL);
                    } else if (state == STATE_SET_HOURS || state == STATE_SET_MINUTES){

                        if (!ClockCancelSetTime(clock)) {
                           
                            ClockStates(STATE_NORMAL);
                            
                        }else {
                            ClockStates(STATE_CLOCK_INIT);
                        }
                        
                    }
                    break;

                case EV_INCREMENT:
                    if (state == STATE_SET_HOURS) {
                        UpBCDAdjusted(time_clock.time.hours, true);
                    } else if (state == STATE_SET_MINUTES) {
                        UpBCDAdjusted(time_clock.time.minutes, false);
                    } else if (state == STATE_SET_ALARM_HOURS) {
                        UpBCDAdjusted(time_alarm.time.hours, true);
                    } else if (state == STATE_SET_ALARM_MINUTES) {
                        UpBCDAdjusted(time_alarm.time.minutes, false);
                    }
                    break;

                case EV_DECREMENT:
                    if (state == STATE_SET_HOURS) {
                        DownBCDAdjusted(time_clock.time.hours, true);
                    } else if (state == STATE_SET_MINUTES) {
                        DownBCDAdjusted(time_clock.time.minutes, false);
                    } else if (state == STATE_SET_ALARM_HOURS) {
                        DownBCDAdjusted(time_alarm.time.hours, true);
                    } else if (state == STATE_SET_ALARM_MINUTES) {
                        DownBCDAdjusted(time_alarm.time.minutes, false);
                    }
                    break;
                }

                xSemaphoreGive(xStateMutex);
            }

            // manejo de alarma (sin bloquear mutex mucho tiempo)
            if (!alarm_triggered && alarm_enabled && ClockAlarmMatchTheTime(clock) && state == STATE_NORMAL) {
                alarm_triggered = true;
                DigitalOutputActivate(board->led_green);
            }
        }
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

void DownBCDAdjusted(uint8_t numero[2], bool is_hours) {
    uint8_t temp[2] = {numero[1], numero[0]}; // Invertimos
    
    uint16_t value = temp[0] * 10 + temp[1];
    value = (value == 0) ? (is_hours ? 23 : 59) : value - 1;
    
    temp[0] = value / 10;
    temp[1] = value % 10;
    
    numero[0] = temp[1];
    numero[1] = temp[0];
}

static void vRefreshScreenTask(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xStateMutex, portMAX_DELAY)) {

            ScreenRefresh(board->screen);

            if (state == STATE_SET_HOURS || state == STATE_SET_MINUTES) {
                ScreenWriteBCD(board->screen, &time_clock, false, (uint8_t[]){0, 0, 0, 0});
            } else if (state == STATE_SET_ALARM_HOURS || state == STATE_SET_ALARM_MINUTES) {
                ScreenWriteBCD(board->screen, &time_alarm, false, (uint8_t[]){1, 1, 1, 1});
            } else {
                // STATE_NORMAL / INIT
                decimal_points[1] = (state == STATE_NORMAL) ? (show_dot ? 1 : 0) : 1;
                decimal_points[3] = ClockIsAlarmEnabled(clock) ? 1 : 0;
                ScreenWriteBCD(board->screen, &time_clock, false, decimal_points);
            }

            xSemaphoreGive(xStateMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(3));
    }
}

static void vDotTask(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xStateMutex, portMAX_DELAY)) {
            if (state == STATE_NORMAL) {
                show_dot = !show_dot;
            } else {
                show_dot = true; // o como prefieras en INIT
            }
            xSemaphoreGive(xStateMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vButtonTask(void *pvParameters) {
    const TickType_t debounce = pdMS_TO_TICKS(30);
    app_event_t ev;

    for (;;) {
        if (DigitalInputHasActivate(board->set_time)) {
            ev.type = EV_SET_TIME;  xQueueSend(xEvtQ, &ev, 0);
            last_input_tick = xTaskGetTickCount(); 
            vTaskDelay(debounce);
        } else if (DigitalInputHasActivate(board->set_alarm)) {
            ev.type = EV_SET_ALARM; xQueueSend(xEvtQ, &ev, 0); 
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        } else if (DigitalInputHasActivate(board->accept)) {
            ev.type = EV_ACCEPT;    xQueueSend(xEvtQ, &ev, 0); 
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        } else if (DigitalInputHasActivate(board->cancel)) {
            ev.type = EV_CANCEL;    xQueueSend(xEvtQ, &ev, 0); 
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        } else if (DigitalInputHasActivate(board->increment)) {
            ev.type = EV_INCREMENT; xQueueSend(xEvtQ, &ev, 0); 
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        } else if (DigitalInputHasActivate(board->decrement)) {
            ev.type = EV_DECREMENT; xQueueSend(xEvtQ, &ev, 0); 
            last_input_tick = xTaskGetTickCount();
            vTaskDelay(debounce);
        }

        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

uint32_t ClockGetTicks(void) {
    return xTaskGetTickCount();
}

static void vClockTask(void *pvParameters) {
    for (;;) {
        ClockNewTick(clock);  // el RTC simulado sigue corriendo

        if (xSemaphoreTake(xStateMutex, portMAX_DELAY)) {
            // SOLO actualizar time_clock desde el reloj en estados que no son de edición
            if (state == STATE_NORMAL || state == STATE_CLOCK_INIT) {
                ClockGetTime(clock, &time_clock);
            }
            xSemaphoreGive(xStateMutex);
        }

        HandleAlarm();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
    
static void vInactivityTask(void *pvParameters) {
    for (;;) {
        TickType_t now = xTaskGetTickCount();
        
        // Solo considerar estados de edición
        if ((state == STATE_SET_MINUTES || state == STATE_SET_HOURS ||
             state == STATE_SET_ALARM_MINUTES || state == STATE_SET_ALARM_HOURS) &&
            (now - last_input_tick >= pdMS_TO_TICKS(INACTIVITY_TIMEOUT_MS))) {
            
            // Enviar evento CANCEL solo si la cola no está llena
            app_event_t ev = { .type = EV_CANCEL };
            xQueueSend(xEvtQ, &ev, 0);
            
            // Actualizamos last_input_tick para evitar enviar continuamente
            last_input_tick = now;
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // chequear cada 100 ms
    }
}

/* === Public function implementation ========================================================= */

/**
 * @brief Función principal del programa
 * @return int Siempre retorna 0 (no utilizado en sistemas embebidos)
 */
int main(void) {

    
    //clock_time_t time_alarm;
    // Inicializar hardware
    board = BoardCreate();


    // Crear reloj con 1000 ticks por segundo
    clock = ClockCreate(1000);
    SysTickInit(1000); // Configurar SysTick para 1ms
    // Configurar hora inicial
    ClockGetTime(clock, &time_clock);
    ClockGetAlarm(clock,&time_alarm);
    ClockDisableAlarm(clock);

    xEvtQ = xQueueCreate(10, sizeof(app_event_t));
    xStateMutex = xSemaphoreCreateMutex();
    xAlarmQueue = xQueueCreate(5, sizeof(alarm_event_t));

    // Tareas y prioridades
    xTaskCreate(vAlarmTask,         "Alarm",    configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &xAlarmTaskHandle);
    xTaskCreate(vRefreshScreenTask, "Refresh",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, NULL);
    xTaskCreate(vButtonTask,        "Buttons",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
    xTaskCreate(vClockTask,         "Clock",    configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
    xTaskCreate(vStateMachineTask,  "FSM",      configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(vDotTask,           "Dot",      configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vInactivityTask, "Inactivity",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();
    
    while(1);
}



/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
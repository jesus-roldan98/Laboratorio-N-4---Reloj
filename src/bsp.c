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

/** @file bsp.c
 ** @brief Plantilla para la creación de archivos de código fuente en lenguaje C
 **/

/* === Headers files inclusions ==================================================================================== */

#include "bsp.h"
#include "chip.h"
#include "screen.h"
#include "poncho.h"
#include <stdlib.h>
#include <stddef.h>

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

void DigitsTurnOff(void);

void SegmentsUpdate(uint8_t value, uint8_t value_decimal_points);
 
void DigitsTurnOn(uint8_t digit);

DigitalOutputT LedRGBInit(uint8_t color);

/* === Private variable definitions ================================================================================ */

static const struct screen_driver_s screen_driver = {
    .DigitsTurnOff = DigitsTurnOff,
    .SegmentsUpdate = SegmentsUpdate,
    .DigitsTurnOn = DigitsTurnOn
};


/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

void DigitsInit(void) {
   Chip_SCU_PinMuxSet(DIGIT_1_PORT, DIGIT_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_1_FUNC);
   DigitalOutputCreate(DIGIT_1_GPIO, DIGIT_1_BIT, false);
   
   Chip_SCU_PinMuxSet(DIGIT_2_PORT, DIGIT_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_2_FUNC);
   DigitalOutputCreate(DIGIT_2_GPIO, DIGIT_2_BIT, false);
   
   Chip_SCU_PinMuxSet(DIGIT_3_PORT, DIGIT_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_3_FUNC);
   DigitalOutputCreate(DIGIT_3_GPIO, DIGIT_3_BIT, false);
   
   Chip_SCU_PinMuxSet(DIGIT_4_PORT, DIGIT_4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_4_FUNC);
   DigitalOutputCreate(DIGIT_4_GPIO, DIGIT_4_BIT, false);
}
void SegmentsInit(void) {
    
   Chip_SCU_PinMuxSet(SEGMENT_A_PORT, SEGMENT_A_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_A_FUNC);
   DigitalOutputCreate(SEGMENT_A_GPIO, SEGMENT_A_BIT, false);
   
   Chip_SCU_PinMuxSet(SEGMENT_B_PORT, SEGMENT_B_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_B_FUNC);
   DigitalOutputCreate(SEGMENT_B_GPIO, SEGMENT_B_BIT, false);
   
   Chip_SCU_PinMuxSet(SEGMENT_C_PORT, SEGMENT_C_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_C_FUNC);
   DigitalOutputCreate(SEGMENT_C_GPIO, SEGMENT_C_BIT, false);
   
   Chip_SCU_PinMuxSet(SEGMENT_D_PORT, SEGMENT_D_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_D_FUNC);
   DigitalOutputCreate(SEGMENT_D_GPIO, SEGMENT_D_BIT, false);
   
   Chip_SCU_PinMuxSet(SEGMENT_E_PORT, SEGMENT_E_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_E_FUNC);
   DigitalOutputCreate(SEGMENT_E_GPIO, SEGMENT_E_BIT, false);
   
   Chip_SCU_PinMuxSet(SEGMENT_F_PORT, SEGMENT_F_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_F_FUNC);
   DigitalOutputCreate(SEGMENT_F_GPIO, SEGMENT_F_BIT, false);
   
   Chip_SCU_PinMuxSet(SEGMENT_G_PORT, SEGMENT_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_G_FUNC);
   DigitalOutputCreate(SEGMENT_G_GPIO, SEGMENT_G_BIT, false);
   
   Chip_SCU_PinMuxSet(SEGMENT_P_PORT, SEGMENT_P_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_P_FUNC);
   DigitalOutputCreate(SEGMENT_P_GPIO, SEGMENT_P_BIT, false);
}

void DigitsTurnOff(void) {

   Chip_GPIO_ClearValue(LPC_GPIO_PORT, DIGITS_GPIO, DIGITS_MASK);
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, SEGMENTS_GPIO, SEGMENTS_MASK);
   Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, false);

}

void SegmentsUpdate(uint8_t value, uint8_t value_decimal_points) {

 Chip_GPIO_SetValue(LPC_GPIO_PORT, SEGMENTS_GPIO, (value & SEGMENTS_MASK)); // Actualizar los segmentos
 Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, (value_decimal_points & SEGMENT_P)); // Actualizar el punto decimal

}

void DigitsTurnOn(uint8_t digit) {

   Chip_GPIO_SetValue(LPC_GPIO_PORT, DIGITS_GPIO, (1 << (3 - digit)) & DIGITS_MASK); // Prender el digito correspondiente
    
}


DigitalOutputT LedRGBInit(uint8_t color) {

   DigitalOutputT result;

   if (color == 1) {
   Chip_SCU_PinMuxSet(PONCHO_RGB_RED_PORT, PONCHO_RGB_RED_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | PONCHO_RGB_RED_FUNC);
   result = DigitalOutputCreate(PONCHO_RGB_RED_GPIO, PONCHO_RGB_RED_BIT, false);
   }
   
   if (color == 2) {
   Chip_SCU_PinMuxSet(PONCHO_RGB_GREEN_PORT, PONCHO_RGB_GREEN_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | PONCHO_RGB_GREEN_FUNC);
   result = DigitalOutputCreate(PONCHO_RGB_GREEN_GPIO, PONCHO_RGB_GREEN_BIT, false);
   }

   if (color == 3) {
   Chip_SCU_PinMuxSet(PONCHO_RGB_BLUE_PORT, PONCHO_RGB_BLUE_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | PONCHO_RGB_BLUE_FUNC);
   result = DigitalOutputCreate(PONCHO_RGB_BLUE_GPIO, PONCHO_RGB_BLUE_BIT, false);
   }

   return result;

}

DigitalInputT DigitalInit(uint8_t Key) {
   DigitalInputT result;

   if (Key == 1) {

      Chip_SCU_PinMuxSet(KEY_F1_PORT, KEY_F1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_F1_FUNC);
      result = DigitalInputCreate(KEY_F1_GPIO, KEY_F1_BIT, false); // Crear el objeto de entrada para el boton F1

   }

   if (Key == 2) {

      Chip_SCU_PinMuxSet(KEY_F2_PORT, KEY_F2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_F2_FUNC);
      result = DigitalInputCreate(KEY_F2_GPIO, KEY_F2_BIT, false); // Crear el objeto de entrada para el boton F2

   }

   if (Key == 3) {

      Chip_SCU_PinMuxSet(KEY_F3_PORT, KEY_F3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_F3_FUNC);
      result = DigitalInputCreate(KEY_F3_GPIO, KEY_F3_BIT, false); // Crear el objeto de entrada para el boton F3

   }

   if (Key == 4) {

      Chip_SCU_PinMuxSet(KEY_F4_PORT, KEY_F4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_F4_FUNC);
      result = DigitalInputCreate(KEY_F4_GPIO, KEY_F4_BIT, false); // Crear el objeto de entrada para el boton F4

   }

   if (Key == 5) {

      Chip_SCU_PinMuxSet(KEY_ACCEPT_PORT, KEY_ACCEPT_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_ACCEPT_FUNC);
      result = DigitalInputCreate(KEY_ACCEPT_GPIO, KEY_ACCEPT_BIT, false); // Crear el objeto de entrada para el boton Aceptar

   }

   if (Key == 6) {

      Chip_SCU_PinMuxSet(KEY_CANCEL_PORT, KEY_CANCEL_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | KEY_CANCEL_FUNC);
      result = DigitalInputCreate(KEY_CANCEL_GPIO, KEY_CANCEL_BIT, false); // Crear el objeto de entrada para el boton Cancelar

   }

   return result;

}


/* === Public function implementation ============================================================================== */

/*@brief implementacion de una board
 * @param self puntero a la estructura de la placa
 * @return puntero a la estructura de la placa
 * @details Crea un objeto de tipo BoardT y lo inicializa con los pines correspondientes a los leds y botones.
 * @note Se utiliza la libreria Chip_SCU_PinMuxSet para configurar los pines.
 * @note Se utiliza la libreria DigitalOutputCreate para crear los objetos de salida.
 * @note Se utiliza la libreria DigitalInputCreate para crear los objetos de entrada.
*/

BoardT BoardCreate(void) {

struct BoardS * self = malloc(sizeof(struct BoardS));
   if (self != NULL) {
      DigitsInit(); // Inicializar los pines de los digitos
      SegmentsInit(); // Inicializar los pines de los segmentos
      self->screen = ScreenCreate(4, &screen_driver);
      self->led_red = LedRGBInit(1); // Inicializar el led rojo
      self->led_green = LedRGBInit(2); // Inicializar el led verde
      self->led_blue = LedRGBInit(3); // Inicializar el led azul

      self->accept = DigitalInit(6); // Inicializar el boton Aceptar
      self->cancel = DigitalInit(5); // Inicializar el boton Cancelar
      self->set_alarm = DigitalInit(4); // Inicializar el boton Set Alarm
      self->set_time = DigitalInit(3); // Inicializar el boton Set Time
      self->increment = DigitalInit(2); // Inicializar el boton Increment
      self->decrement = DigitalInit(1); // Inicializar el boton Decrement 
      }

      return self;
}

/* === End of documentation ======================================================================================== */

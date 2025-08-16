# Electronica IV - RELOJ TERMINADO

##   Reloj funcional con sistema operativo freeRTOS

### Ejercicio a Desarrollar

Se deberá crear un repositorio git con acceso público que tenga el código fuente de un proyecto
en C que a, partir del ejemplo proporcionado que utiliza las teclas y botones de la EDU-CIAA-NXP,
implemente una versión del mismo proyecto utilizando dos capas de abstracción de hardware:

• Toda la gestión de las entradas y salidas digitales se deberá encapsular en un tipo abstracto de datos que deberá estar implementado en un archivo fuente separado del programa
principal.

• Toda la configuración de la placa de soporte del proyecto, como también la creación de los
recursos de entradas y salidas digitales, deberá estar encapsulada en una abstracción que
también deberá ser implementada en un archivo fuente separado.

• La abstracción para la gestión de la placa será a medida del proyecto, asignando nombres
funcionales a los recursos creados para facilitar la programación de la aplicación.

• El programa principal solo deberá utilizar las abstracciones creadas evitando interactuar directamente con las funciones del fabricante.

• Capa de abstracción de hardware HAL (Hardware Abstraction Layer)

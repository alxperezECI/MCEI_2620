1. Prueba Básica de Hilos (hola.c)

Descripción: Un programa sencillo para comprobar que OpenMP funciona y puede crear varios "hilos" o trabajadores virtuales.

Salida en Consola:

Hola desde el hilo 6
Hola desde el hilo 7
Hola desde el hilo 3
Hola desde el hilo 2
Hola desde el hilo 1
Hola desde el hilo 4
Hola desde el hilo 0
Hola desde el hilo 5
... 
Análisis:
Como se puede ver en la salida, los mensajes no aparecen en orden (0, 1, 2, 3...). Esto pasa porque todos los hilos se están ejecutando exactamente al mismo tiempo. Es como tener a varias personas haciendo una tarea a la vez: es imposible predecir quién va a terminar de hablar primero. Esto nos demuestra que el programa realmente está trabajando en paralelo.

2. Integración Numérica: Método de Simpson (simpson.c)Descripción: Programa para calcular el área bajo la curva de la integral $\int_{0}^{\pi} \sin(x) dx$ usando el método de Simpson, pero repartiendo el trabajo del ciclo for entre varios hilos.
Salida en Consola:
Resultado (paralelo): 2.0000000000
Tiempo: 0.05207 segundos

Análisis:
El programa logró encontrar la respuesta matemáticamente correcta (que es exactamente $2$). Al usar OpenMP, el trabajo largo y pesado de hacer los cálculos se repartió entre los distintos núcleos del computador. Al final, el programa sumó todo de forma segura para darnos el resultado total sin mezclar la información.

3. Integración Numérica: Regla del Trapecio (trapecio.c)Descripción: Programa para calcular la misma integral $\int_{0}^{\pi} \sin(x) dx$, esta vez sumando el área de millones de trapecios de forma simultánea.

Resultado (paralelo): 2.0000000000
Tiempo: 0.04294 segundos

Análisis:
Al igual que en el ejercicio anterior, llegamos al resultado correcto de forma muy rápida. Dividir una tarea tan larga y repetitiva entre varios hilos demuestra lo útil que es OpenMP para hacer que nuestros programas matemáticos no se queden pensando por horas, sino que aprovechen toda la potencia de la computadora para terminar en fracciones de segundo.
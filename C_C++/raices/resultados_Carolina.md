---------------------------------------------
Resultados 
---------------------------------------------
   Método      | raíz    | iteraciones | Observaciones |
---------------------------------------------
bisección      |0.20164  |  29         | Dependiendo de en donde se ajusten los limites van a dar raices diferentes en un limite de [0,1] dio el que puse pero en un limite de [1,3] da  2.12842 con 27 iteraciones lo que nos muestra que solo se pueden tomar intervalos pequeños para analizar si hay solo 1 raíz
---------------------------------------------
Falsa posición |0.20164  |  9          |Es un método mucho mas rápido que el anterior reduciendo su numero de iteraciones hasta un poco mas de 3 veces con diferentes intervalos de tiempo,tampoco requiere un paso de codigo adicional ademas de solo cambiar la linea que define mi root_fsolver
Falsa posición | 2.12842 |  10         |
----------------------------------------------
  Brent        | 0.20164 |  6          | Con este metodo aumento la rapidez para encontrar las raices a comparacion de los otros dos en la deteccion de los dos intervalos que he estado evaluando
  Brent        | 2.12842 |  7          |
----------------------------------------------
  Newton       | 0.20164 |  4          | Este metodo redujo el numero de iteraciones pero su implementacion requirio cambiar muchos detalles del codigo base tanto añadir cosas a lo que ya etaba como añadir la funcion derivada por lo que es mas rapido pero no considero que sea el mas eficiente si lo comparamos por ejemplo con el anterior.
  Newton       | 2.12842 |  5          |
----------------------------------------------
  Secante      | 0.20164 |  5          | Es el primer método que hace la misma cantidad de iteraciones para la raiz que está por debajo de 0 y la que está por encima de 2 en rápidez sigue siendo uno de los mas rapidos pero mantiene los mismos ajustes que los de Newton por lo que es tambien más robusto.
  Secante      | 2.12842 |  5          |
----------------------------------------------
  steffenson   | 0.20164 |  4          | Este tiene el mismo comportamiento que el de Newton para el caso del codigo implementado con ayuda de una ia (gemini) por lo que no sabria definir entre este y el de Newton cual seria el mejor.   steffenson   | 0.20164 |  4          |
  steffenson   | 2.12842 |  5          |
----------------------------------------------
Segunda función
----------------------------------------------
Resultados 
---------------------------------------------
   Método      | raíz    | iteraciones
---------------------------------------------
Método	Tipo	Iteraciones	Raíz calculada
Bisección	     |0.56714328	| 28
Posición Falsa |0.567143	  |  7	
Brent	         |0.567143    |	 6
Newton	       |0.567143    |  5	
Secante	       |0.567143    | 6
Steffenson	   |0.567143    |	5	
--------------------------------------------
Observaciones y comparaciones:
--------------------------------------------
Número de iteraciones. La bisección fue por mucho la más lenta. otra vez, con 28 iteraciones.Observando el como se comporta este método su lentitud se debe a que no usa nada de información sobre la forma de la función: solo parte el intervalo a la mitad una y otra vez. De hecho, desde la iteración 21 el valor mostrado ya no cambiaba, pero el programa siguió trabajando porque el intervalo todavía no era menor a la tolerancia de 1e-8. En el otro extremo, Newton y Steffenson solo necesitaron 5 iteraciones, y ya en la segunda iteración iban en 0.5663. La posición falsa siguio siendo una de las mejores (7 iteraciones), porque en este intervalo la función es casi una recta y por eso la aproximación lineal que usa el método cae muy cerca de la raíz desde el primer paso.

Sensibilidad al valor inicial. En este caso como en la función la derivada de e⁻ˣ − x es −e⁻ˣ − 1,  siempre es negativa, y nunca vale cero. Por eso, sin importar desde dónde se arranque, los métodos abiertos siempre encuentran hacia dónde moverse y llegan a la raíz. Además, esta función tiene una sola raíz, así que tampoco existe el riesgo de que el método converja a una raíz distinta de la que uno buscaba.

Robustez. Los métodos cerrados nunca fallan mientras el intervalo inicial encierre la raíz, y aquí se cumple porque f(0) = 1 es positivo y f(1) ≈ −0.632 es negativo. La bisección es la más confiable de todas, aunque sea la más lenta: uno sabe con seguridad que va a llegar. Brent es el mejor de los tres cerrados, porque combina la seguridad de la bisección con la rapidez de los métodos que usan la forma de la función, y por eso terminó en 6 iteraciones sin perder la garantía de convergencia. Los métodos abiertos son más rápidos pero, en general, no ofrecen esa garantía; en este caso particular funcionaron bien por lo bien portada que es la función.

Precisión final. Los seis métodos llegaron al mismo resultado, 0.567143, con la tolerancia de 1e-8 que fijé en el criterio de parada. lo que se puede traducir a que la precisión final no depende del método, sino de la tolerancia que uno pida; lo que sí cambia entre métodos es cuánto trabajo cuesta llegar a esa precisión. En este caso la bisección necesitó casi seis veces más iteraciones que Newton para dar exactamente la misma respuesta.

Conclusión. Si uno tiene la derivada a la mano y sabe que la función se porta bien, Newton es la mejor opción por lo rápido que converge. Si no se tiene la derivada, la secante da prácticamente el mismo rendimiento. 
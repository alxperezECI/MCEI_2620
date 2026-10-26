---------------------------------------------
TABLA 1. Resultados primera función con intervalo de [0, 1] y valor inicial de '0'
---------------------------------------------
Método      | raíz | iteraciones | Observaciones |
Bisección   | 0.20164 | 29 iteraciones | Con aproximadamente 16 iteraciones ya se empezaba a ver una convergencia en 0.20164, empleando el intervalo [0, 1] | 

False Position   | 0.20164 | 8 iteraciones | Requirió muchas menos iteraciones que el método de la bisección para encontrar la raíz. | 

Brent   | 0.20164 | 6 iteraciones | Se emplearon muchas menos iteraciones que el método de la bisección para encontrar la misma raíz. Este método es la combinación de otros métodos como la bisección, la secante y la interpolación cuadrática, lo que permite que si un método falla o es muy lento, se puede utilizar otro de los métodos que la componen para asegurar que se encuentre la raíz | 

Newton   | 0.20164 | 4 iteraciones | Encuentra la raíz en menos iteraciones que un método cerrado, pero aumenta la complejidad del algoritmo porque requiere hacer derivadas para aproximarse en cada iteración a la raíz. | 

Secante   | 0.20164 | 5 iteraciones | Encuentra la raíz en menos iteraciones que un método cerrado. Requirió una iteración más que el método de Newton-Raphson | 

Steffenson   | 0.20164 | 4 iteraciones | Encuentra la raíz en menos iteraciones que un método cerrado. Tuvo la misma cantidad de iteraciones que newton-Raphson para hallar la raíz | 

---------------------------------------------


---------------------------------------------
TABLA 2. Resultados segunda función con intervalo de [0, 1] y valor inicial de '0'
---------------------------------------------
Método      | raíz | iteraciones | Observaciones |
Bisección   | 0.567143 | 28 iteraciones | Con aproximadamente 19 iteraciones ya se empezaba a ver una convergencia en 0.567143, empleano el intervalo [0, 1] que contiene la raíz deseada | 

False Position   | 0.567143 | 7 iteraciones | Requirió de solo 7 iteraciones para encontrar la raíz, 21 iteraciones menos que el método de la bisección. | 

Brent   | 0.567143 | 6 iteraciones | Requirió de solo 6 iteraciones para encontrar la raíz, 22 iteraciones menos que el método de la bisección, y una menos que el de falsa posición y empata con el método de la secante. |

Newton   | 0.567143 |  5 iteraciones | Le tomó 23 iteraciones menos que el método de la bisección, empleando a '0' como punto inicial.  | 


Secante   | 0.567143 |  6 iteraciones | Emplea la misma cantidad de iteraciones que el método de Brent, lo que puede deberse a que Brent requiere usar el método de la secante. | 


Steffenson   | 0.567143 | 5 iteraciones | Iguala en cantidad de iteraciones al métodod de Newton utilizando el mismo punto inicial '0'. Ambos métodos son los que tienen menos iteraciones. | 

---------------------------------------------


TABLA 3. Resultados segunda función con intervalo de [-2, 3] y valor inicial de '-1'
---------------------------------------------
Método      | raíz | iteraciones | Observaciones |
Bisección   | 0.567143 | 30 iteraciones | Al utilizarse un intervalo más grande a [0, 1], la cantidad de iteraciones aumentó, y sigue siendo funcional porque el intervalo contiene a la raíz. | 

False Position   | 0.567143 | 8 iteraciones | Requirió de 8 iteraciones para encontrar la raíz, 22 iteraciones menos que el método de la bisección. Evidentemente, al usar un intervalo más grande que [0, 1] hubieron más iteraciones.| 

Brent   | 0.567143 | 6 iteraciones | Requirió de solo 6 iteraciones para encontrar la raíz, 24 iteraciones menos que el método de la bisección, y dos menos que el de falsa posición.|

Newton   | 0.567143 |  6 iteraciones | Le tomó 24 iteraciones menos que el método de la bisección, empleando a '-1' como punto inicial.  | 


Secante   | 0.567143 |  7 iteraciones | Emplea En este caso, su cantidad de itreraciones fue diferente al método de Brent. | 


Steffenson   | 0.567143 | 5 iteraciones | Iguala en cantidad de iteraciones al métodod de Newton utilizando el mismo punto inicial '-1'. Ambos métodos son los que tienen menos iteraciones, junto con el método de Brent en esta prueba. | 
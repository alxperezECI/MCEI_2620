-----------------------------------------------------------------------------------------------
Resultados 
-----------------------------------------------------------------------------------------------
Método         | raíz    | iteraciones | Observaciones |
-----------------------------------------------------------------------------------------------
Falsa Posición | 0.20164 | 8           | Converge en 8 iteraciones dentro del intervalo [0, 1].
Bisección      | 0.20164 | 29          | Converge en 29 iteraciones por dividir el intervalo por la mitad linealmente
Brent          | 0.20164 | 6           | El más rápido de los métodos cerrados, combinando bisección e interpolación.
Newton         | 0.20164 | 3           | Convergencia cuadrática muy rápida al usar la derivada.
Secante        | 0.20164 | 4           | Converge en 4 iter, aproxima la derivada mediante diferencias finitas.
Steffenson     | 0.20164 | 3           | Método logra convergencia cuadrática acelerada similar a Newton.

FUNCIÓN: e^-x -x

Bisección      |0.567143 | 17          | Convergencia lineal lenta en el intervalo [0, 1].
Falsa Posición |0.567143 | 7           | Reduce rápidamente el intervalo respecto a Bisección.
Brent          |0.567143 | 6           | El más eficiente de los métodos cerrados.
Newton         |0.567143 | 4           | Muy rápido gracias al uso directo de la derivada $f'(x) = -e^{-x} - 1$.
Secante        |0.567143 | 5           | Convergencia superlineal rápida sin evaluar la derivada directamente.
Steffenson     |0.567143 | 5           | Excelente velocidad de convergencia equivalente a la Secante.

6. Preguntas de Exploración

1. ¿Cómo identificar visualmente la existencia de una raíz?
Visualmente, una raíz real de una función corresponde al punto exacto donde la gráfica de la función cruza o toca el 
eje horizontal. En ese punto, la coordenada vertical es cero. Además, si la función cruza el eje, se observa un cambio 
de signo en los valores de y (pasa de positivo a negativo o viceversa).

2. ¿Qué significa que una raíz esté acotada?
Significa que se conoce un intervalo cerrado [a, b] tal que la raíz se encuentra atrapada estrictamente dentro de sus 
límites. 

3. ¿Por qué algunos métodos requieren derivadas?
Porque utilizan la información de la pendiente de la curva en el punto actual para estimar la dirección y distancia 
hacia donde se encuentra la raíz. En lugar de simplemente recortar un intervalo a ciegas, trazan la recta tangente a 
la curva en el punto evaluado para calcular de forma directa la siguiente aproximación (como hace el método de 
Newton-Raphson), lo cual acelera drásticamente la velocidad de solución.

4. ¿Qué ventajas tiene un método abierto frente a uno cerrado?
Los métodos abiertos (Newton, Secante, Steffenson) suelen tener convergencia cuadrática o superlineal, alcanzando la 
precisión deseada en muchísimas menos iteraciones que los métodos cerrados. Así como no requieren encontrar un 
intervalo [a, b] que encierre la raíz con cambio de signo; basta con proporcionar uno o dos puntos iniciales cercanos 
a la raíz estimada.

5. ¿Existe garantía de convergencia en todos los casos?
No.Los métodos cerrados sí garantizan convergencia siempre y cuando la función sea continua en el intervalo [a, b] y 
exista un cambio de signo inicial. Los métodos abiertos no garantizan convergencia universal. Si el punto inicial se 
elige lejos de la raíz, o si durante el proceso se pasa por un punto donde la derivada es cero o muy cercana a cero, el 
algoritmo puede divergir, entrar en bucles infinitos o alejarse completamente de la solución.




14. Preguntas de Análisis

1. ¿Qué método converge en menos iteraciones?
Newton-Raphson y Steffenson son los métodos que convergen en la menor cantidad de iteraciones (3 iteraciones para la 
función polinómica y 4 para la trascendente). Esto se debe a su convergencia cuadrática, donde el número de dígitos 
significativos correctos aproximadamente se duplica en cada paso.

2. ¿Qué método presenta mayor robustez?
Bisección y Brent. El método de Bisección ofrece la máxima robustez matemática porque garantiza la convergencia bajo 
cualquier condición continua si se cumple el Teorema de Bolzano. Por su parte, Brent combina esa misma garantía de 
seguridad con una velocidad muy alta gracias a la interpolación cuadrática.

3. ¿Qué ocurre cuando el valor inicial está lejos de la raíz?
En métodos abiertos (Newton, Secante, Steffenson): Si el valor inicial x_0 está lejano, la tangente puede proyectar 
aproximaciones a regiones planas donde la derivada es casi cero, provocando divergencia, oscilaciones infinidad de 
pasos o saltos hacia raíces no deseadas.
En métodos cerrados: Mientras el valor inicial defina un intervalo válido [a, b] que encierre la raíz, el método 
siempre convergerá, aunque requerirá más iteraciones para achicar el intervalo.

4. ¿Cuál método recomendaría para problemas de ingeniería?
Se recomienda el método de Brent. En problemas reales de ingeniería, no siempre se dispone de la derivada analítica de 
la función o se desconoce si la superficie es suave. Brent ofrece el equilibrio óptimo: es tan seguro y robusto como la 
Bisección (nunca diverge si se acota la raíz) y casi tan rápido como Newton.

5. ¿Existe una relación entre costo computacional y velocidad de convergencia?
Sí, existe un compromiso (trade-off) claro: Velocidad vs. Evaluación de funciones: 
Métodos de convergencia rápida como Newton requieren evaluar tanto la función f(x) como su derivada f'(x) en cada paso 
(o calcular aproximaciones compuestas como Steffenson), aumentando el costo por iteración.
Complejidad algorítmica: Métodos sencillos como Bisección hacen operaciones aritméticas insignificantes por iteración 
pero requieren muchísimas iteraciones (29 iteraciones). Métodos como Brent ejecutan lógica de decisión más compleja 
(verificación de límites, interpolaciones), pero reducen drásticamente el tiempo total al requerir solo 6 iteraciones.

---------------------------------------------------------------
Resultados: F(X) = x*x*x - 5*x + 1  
---------------------------------------------------------------
Método     |       raíz     | iteraciones |   Observaciones |
Biseccion  | 0.201639675535 |     29      |                 |
Falsepos   | 0.201639675723 |     08      |                 |
Brent      | 0.201639675723 |     06      |                 |
Newton     | 0.201639675723 |     03      |                 |
Secante    | 0.201639675723 |     03      |                 |
Steffeson  | 0.201639675723 |     03      |                 |
---------------------------------------------------------------
================================================================
---------------------------------------------------------------
Resultados: f(x) = e^-x - x
---------------------------------------------------------------
Método     |       raíz     | iteraciones |   Observaciones |
Biseccion  | 0.567143289372 |     28      |                 |
Falsepos   | 0.56714329041  |     07      |                 |
Brent      | 0.56714329041  |     06      |                 |
Newton     | 0.56714329041  |     05      |                 |
Secante    | 0.56714329041  |     09      |                 |
Steffeson  | 0.56714329041  |     09      |                 |
----------------------------------------------------------------
Número de iteraciones
Newton fue el método más eficiente con 5 iteraciones, seguido por Brent (6) y False Position (7). Bisección fue el más lento con 28 iteraciones.

Sensibilidad al valor inicial
Newton, Secante y Steffensen son más sensibles al valor inicial, ya que una mala aproximación puede aumentar las iteraciones o impedir la convergencia. Bisección y Brent son menos sensibles mientras la raíz esté dentro del intervalo.

Robustez
Bisección es el método más robusto porque garantiza convergencia si existe un cambio de signo en el intervalo. Brent también mostró gran robustez, pero con una convergencia mucho más rápida.

Precisión final
Todos los métodos alcanzaron prácticamente la misma raíz, 0.56714329041, por lo que la precisión final fue equivalente para la tolerancia utilizada.
----------------------------------------------------------------
================================================================
================================================================
PREGUNTAS DE EXPLORACIÓN:
================================================================

1. ¿Cómo identificar visualmente la existencia de una raíz?
Una raíz se identifica observando la gráfica de la función. Existe una raíz en los puntos donde la curva intersecta o
cruza el eje X, es decir, donde f(x) = 0. Además, si la función cambia de signo entre dos puntos consecutivos, existe al menos una raíz dentro de ese intervalo.

2. ¿Qué significa que una raíz esté acotada?
Una raíz está acotada cuando se sabe que se encuentra dentro de un intervalo [a,b]. Generalmente se verifica que:
    f(a) * f(b) < 0
lo que indica un cambio de signo y garantiza la existencia de al menos una raíz en dicho intervalo para funciones continuas.

3. ¿Por qué algunos métodos requieren derivadas?
Métodos como Newton utilizan la derivada para conocer la pendiente de la función en un punto. Esta información permite estimar rápidamente la ubicación de la raíz mediante una
aproximación lineal, logrando una convergencia más rápida que otros métodos.

4. ¿Qué ventajas tiene un método abierto frente a uno cerrado?
- Generalmente requiere menos iteraciones.
- Alcanza alta precisión más rápido.
- No necesita un intervalo que encierre la raíz.
Sin embargo, puede divergir si se escoge un mal valor inicial. Los métodos cerrados suelen ser más robustos.

5. ¿Existe garantía de convergencia en todos los casos?
No. Ningún método garantiza convergencia en todos los problemas.

================================================================
================================================================
PREGUNTAS DE DISCUSIÓN:
================================================================
1. ¿Qué método converge en menos iteraciones?
Los métodos Newton, Secante y Steffensen convergieron en 3 iteraciones, siendo los más rápidos para este problema. Esto indica que la aproximación inicial utilizada fue adecuada y que la función presenta un comportamiento favorable para estos métodos.

2. ¿Qué método presenta mayor robustez?
El método de Bisección es el más robusto. Siempre que exista un cambio de signo en el intervalo inicial, garantiza la convergencia hacia una raíz. Aunque requiere más iteraciones, es menos sensible a las condiciones iniciales que otros métodos.

3. ¿Qué ocurre cuando el valor inicial está lejos de la raíz?
Cuando el valor inicial está muy alejado de la raíz, algunos métodos como Newton, Secante y Steffensen pueden tardar más en converger o incluso no encontrar la solución. En cambio, los métodos basados en intervalos, como Bisección y Brent, suelen mantener una convergencia más estable y segura.

4. ¿Cuál método recomendaría para problemas de ingeniería?
Para problemas de ingeniería recomendaría el método de Brent, ya que combina rapidez y robustez. No requiere derivadas, converge en pocas iteraciones y mantiene una alta confiabilidad incluso cuando las aproximaciones iniciales no son las mejores.

5. ¿Existe una relación entre costo computacional y velocidad de convergencia?
Sí. Los métodos más rápidos suelen realizar más cálculos por iteración, mientras que los más simples requieren más iteraciones para encontrar la raíz.
================================================================
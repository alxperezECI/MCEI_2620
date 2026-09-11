# Taller de búsqueda de raíces con GNU Scientific Library

**Estudiante:** Jimmy Alexander Calderon Trujillo

## Función de trabajo

La función analizada es:

f(x) = x^3 - 5x + 1

La función presenta múltiples raíces reales.

Para comparar los diferentes algoritmos se estudia inicialmente
la raíz ubicada en el intervalo [0,1].

## Métodos implementados

Se implementaron los siguientes métodos disponibles en GSL:

- Bisección
- False Position
- Brent
- Newton
- Secante
- Steffenson

Los métodos de Bisección, False Position y Brent utilizan un
intervalo que contiene la raíz.

Newton, Secante y Steffenson corresponden a métodos abiertos y
trabajan a partir de un valor inicial.

## Resultados

## Resultados

| Método | Raíz | Iteraciones | Observaciones |
|---|---:|---:|---|
| Bisección | 0.201639675535 | 29 | Convergencia estable y robusta dentro del intervalo [0,1] |
| False Position | 0.201639675723 | 8 | Convergencia más rápida que Bisección mediante interpolación lineal |
| Brent | 0.201639675723 | 6 | Método cerrado con convergencia rápida y alta robustez |
| Newton | 0.201639675723 | 4 | Método abierto; presentó la menor cantidad de iteraciones para x0 = 0.5 |
| Secante | 0.201639675723 | 5 | Método abierto con convergencia rápida |
| Steffenson | 0.201639675723 | 5 | Método abierto con convergencia rápida y acelerada |

## Comparación inicial

Para la función f(x) = x^3 - 5x + 1 se estudió la raíz ubicada
en el intervalo [0,1]. Los métodos cerrados utilizaron como intervalo
inicial [0,1], mientras que los métodos abiertos utilizaron como valor
inicial x0 = 0.5.

Todos los métodos convergieron hacia una raíz aproximada de
x = 0.2016396757.

El método de Newton presentó la menor cantidad de iteraciones, con
4 iteraciones. Le siguieron Secante y Steffenson con 5 iteraciones,
Brent con 6, False Position con 8 y Bisección con 29.

Estos resultados muestran que una menor cantidad de iteraciones no
implica necesariamente que un método sea mejor en todas las
situaciones. Los métodos abiertos pueden presentar una convergencia
rápida, pero su comportamiento puede depender significativamente del
valor inicial. Los métodos cerrados, por su parte, requieren un
intervalo que contenga una raíz y generalmente presentan una mayor
robustez.

## Sensibilidad al valor inicial

Para evaluar la sensibilidad de los métodos abiertos se modificó el
valor inicial utilizado por Newton, Secante y Steffenson.

Con un valor inicial x0 = 0.5 los tres métodos convergieron hacia:

x = 0.2016396757

Posteriormente se utilizó x0 = 2.0 y se obtuvieron los siguientes
resultados:

| Método | Valor inicial | Raíz | Iteraciones |
|---|---:|---:|---:|
| Newton | 2.0 | 2.128419063845 | 4 |
| Secante | 2.0 | 2.128419063844 | 5 |
| Steffenson | 2.0 | 2.128419063845 | 5 |

Al modificar únicamente el valor inicial, los métodos abiertos
convergieron hacia una raíz diferente de la misma función. Esto
demuestra que estos métodos presentan sensibilidad frente a la
selección de la aproximación inicial, especialmente cuando la función
posee múltiples raíces.

En este experimento los tres métodos mantuvieron una convergencia
rápida. Newton necesitó 4 iteraciones, mientras que Secante y
Steffenson necesitaron 5.

## Preguntas de exploración

### ¿Cómo identificar visualmente la existencia de una raíz?

Una raíz puede identificarse gráficamente observando los puntos donde
la curva de la función cruza o toca el eje horizontal. Para una función
continua, un cambio de signo entre dos puntos también indica que existe
al menos una raíz dentro del intervalo.

### ¿Qué significa que una raíz esté acotada?

Significa que se conocen dos valores que forman un intervalo dentro del
cual se encuentra la raíz. Para los métodos de intervalo generalmente
se busca que:

f(a) * f(b) < 0

### ¿Por qué algunos métodos requieren derivadas?

Algunos algoritmos utilizan información sobre la pendiente de la
función para obtener una nueva aproximación de la raíz. Newton utiliza:

x(n+1) = x(n) - f(x(n))/f'(x(n))

La derivada permite determinar la dirección y magnitud de la siguiente
aproximación.

### ¿Qué ventajas tiene un método abierto frente a uno cerrado?

Los métodos abiertos pueden converger en menos iteraciones y no
requieren mantener un intervalo que encierre la raíz. Sin embargo,
pueden ser más sensibles al valor inicial.

### ¿Existe garantía de convergencia en todos los casos?

No. Los métodos cerrados presentan mayor robustez cuando se cumplen
las condiciones requeridas de continuidad y cambio de signo.
Los métodos abiertos pueden divergir o converger hacia una raíz
diferente dependiendo del valor inicial.

## Segunda función

## Resultados para la segunda función

La segunda función analizada fue:

f(x) = exp(-x) - x

Para los métodos cerrados se utilizó el intervalo inicial [0,1].
Para los métodos abiertos se utilizó el valor inicial x0 = 0.5.

Todos los métodos convergieron hacia una raíz aproximada de:

x = 0.5671432904

| Método | Raíz | Iteraciones | Observaciones |
|---|---:|---:|---|
| Bisección | 0.567143289372 | 28 | Convergencia estable, aunque presentó la mayor cantidad de iteraciones |
| False Position | 0.567143290410 | 7 | Convergencia rápida mediante interpolación |
| Brent | 0.567143290410 | 6 | Método cerrado rápido y robusto |
| Newton | 0.567143290410 | 4 | Método abierto con convergencia rápida |
| Secante | 0.567143290410 | 4 | Método abierto con convergencia rápida |
| Steffenson | 0.567143290410 | 4 | Método abierto con convergencia rápida |

## Comparación de convergencia de la segunda función

Para esta función, los métodos abiertos Newton, Secante y Steffenson
presentaron la menor cantidad de iteraciones, con 4 iteraciones cada uno.

Entre los métodos cerrados, Brent presentó el mejor desempeño con
6 iteraciones, seguido de False Position con 7 iteraciones.

Bisección necesitó 28 iteraciones, lo que confirma que es un método
robusto pero generalmente más lento que los otros métodos estudiados.

La función exp(-x) - x presenta una única raíz real, por lo que los
métodos convergieron hacia la misma solución bajo las condiciones
iniciales utilizadas.

## Discusión de resultados

### 1. ¿Qué método convergió en menos iteraciones?

Para la función f(x) = x^3 - 5x + 1, utilizando el intervalo [0,1]
para los métodos cerrados y x0 = 0.5 para los métodos abiertos, el
método de Newton presentó la menor cantidad de iteraciones, alcanzando
la raíz en 4 iteraciones.

Los resultados fueron:

- Bisección: 29 iteraciones.
- False Position: 8 iteraciones.
- Brent: 6 iteraciones.
- Newton: 4 iteraciones.
- Secante: 5 iteraciones.
- Steffenson: 5 iteraciones.

Para la segunda función, f(x) = exp(-x) - x, Newton, Secante y
Steffenson presentaron la menor cantidad de iteraciones, con 4
iteraciones cada uno.

Por lo tanto, en los experimentos realizados los métodos abiertos
presentaron las convergencias más rápidas. Sin embargo, el número de
iteraciones por sí solo no permite determinar cuál método es mejor en
todos los casos, ya que también se deben considerar la robustez y la
sensibilidad a las condiciones iniciales.


### 2. ¿Cuál método fue más sensible al intervalo o valor inicial?

Los métodos abiertos mostraron sensibilidad frente al valor inicial.

Para f(x) = x^3 - 5x + 1, utilizando x0 = 0.5, Newton, Secante y
Steffenson convergieron hacia:

x = 0.2016396757

Al cambiar únicamente el valor inicial a x0 = 2.0, los mismos métodos
convergieron hacia:

x = 2.1284190638

Las dos soluciones corresponden a raíces diferentes de la misma
función.

Este experimento demuestra que, cuando una función presenta múltiples
raíces, la selección del valor inicial puede determinar hacia cuál de
ellas converge un método abierto.

Los métodos cerrados también dependen de la selección del intervalo,
pero su funcionamiento está basado en mantener una raíz acotada dentro
de un intervalo apropiado.


### 3. ¿Cuál método resultó más robusto?

Entre los métodos estudiados, Brent presenta una combinación favorable
entre robustez y velocidad de convergencia.

En las dos funciones analizadas necesitó 6 iteraciones, mientras que
Bisección necesitó 29 iteraciones para la primera función y 28 para la
segunda.

Bisección también se caracteriza por su robustez cuando se parte de un
intervalo válido que contiene un cambio de signo, pero presentó una
convergencia considerablemente más lenta.

Por esta razón, dentro de los experimentos realizados, Brent presentó
un buen equilibrio entre la seguridad de un método cerrado y una
convergencia rápida.


### 4. ¿Qué método utilizaría en un problema de ingeniería?

La elección dependería de la información disponible sobre el problema.

Si se dispone de una función diferenciable, una buena aproximación
inicial y es importante obtener una solución rápidamente, Newton puede
ser una alternativa apropiada debido a su rápida convergencia. En los
experimentos realizados necesitó solamente 4 iteraciones.

Sin embargo, si se conoce un intervalo que contiene la raíz y se busca
mayor robustez, utilizaría Brent. En las dos funciones analizadas
convergió en 6 iteraciones y presentó un mejor desempeño que Bisección
entre los métodos cerrados.

Por lo tanto, en un problema de ingeniería no seleccionaría un método
únicamente por su velocidad. También tendría en cuenta la disponibilidad
de la derivada, la existencia de un intervalo válido, la sensibilidad a
las condiciones iniciales y la robustez requerida.


### 5. ¿Qué ocurre si el intervalo inicial no contiene una raíz?

Los métodos cerrados necesitan un intervalo adecuado para realizar la
búsqueda de la raíz. Para una función continua, normalmente se verifica
un cambio de signo entre los extremos:

f(a) * f(b) < 0

Esta condición permite establecer que existe al menos una raíz dentro
del intervalo.

Si el intervalo seleccionado no encierra una raíz y no presenta el
cambio de signo requerido, el método cerrado no puede iniciar
correctamente bajo esta condición y GSL puede reportar un error al
establecer el intervalo.

Por esta razón es importante evaluar la función en los extremos antes
de ejecutar métodos como Bisección, False Position o Brent.


## Conclusiones

Los seis métodos implementados con GNU Scientific Library permitieron
analizar diferentes estrategias numéricas para la búsqueda de raíces.

Para f(x) = x^3 - 5x + 1 se encontró, utilizando el intervalo [0,1],
una raíz aproximada de:

x = 0.2016396757

Newton presentó la menor cantidad de iteraciones con 4, mientras que
Bisección necesitó 29.

La prueba con x0 = 2.0 mostró que los métodos abiertos convergieron
hacia otra raíz:

x = 2.1284190638

lo cual permitió comprobar experimentalmente su sensibilidad frente al
valor inicial cuando existen múltiples raíces.

Para f(x) = exp(-x) - x se obtuvo:

x = 0.5671432904

Newton, Secante y Steffenson convergieron en 4 iteraciones, Brent en 6,
False Position en 7 y Bisección en 28.

Los resultados muestran que los métodos abiertos pueden ofrecer una
convergencia muy rápida cuando se selecciona una condición inicial
adecuada, mientras que los métodos cerrados proporcionan mayor control
sobre la región donde se busca la solución. Entre estos últimos, Brent
presentó un buen equilibrio entre rapidez y robustez.
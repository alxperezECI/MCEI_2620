### ----------------------------------------------------
### TALLER SOBRE INTEGRACIÓN Y DIFERENCIACIÓN NUMÉRICA
### ----------------------------------------------------

### --------------- EJERCICIO #1 -----------------------

## A. Análisis matemático
## 1. Valor de referencia
La función puede integrarse analíticamente. Evaluando la integral entre 0 y 8 se obtiene:

I_{ref}=2.559824508302
 
Este valor se utiliza como referencia para evaluar los métodos numéricos.
 
## 2. ¿Qué representa el área acumulada?
Representa el valor acumulado de la señal en el intervalo \([0,8]\). Su interpretación física depende de las variables; por ejemplo, si la señal fuera potencia en función del tiempo, el área representaría energía.
 
## 3. Características que afectan la aproximación
La función presenta una tendencia exponencial decreciente y una componente oscilatoria. Una discretización muy gruesa puede no representar adecuadamente estas variaciones, por lo que disminuir el paso mejora la aproximación.
 
---
## B. GNU Octave
Con la regla del trapecio se observó que al aumentar \(n\), el resultado converge al valor de referencia y el error disminuye.
 
Para \(n=10\):
 
I=2.494072510186,\qquad E=6.575\times10^{-2}
 
Para \(n=1000\):
 
I=2.559818732303,\qquad E=5.776\times10^{-6}
 
Con `quadgk`:

I=2.559824508302

E_{estimado}=6.391\times10^{-12}

La cuadratura adaptativa logró una alta precisión sin definir explícitamente el número de subintervalos.
 
---
## C. C/C++ con GSL
Con `gsl_integration_qag` se obtuvo:

I=2.559824508302

E_{estimado}=2.842\times10^{-14}
 
con tolerancias absoluta y relativa de \(10^{-10}\).
 
En el trapecio, el programador controla directamente \(n\), el paso y la suma. En GSL se especifican las tolerancias y parámetros, mientras la biblioteca realiza internamente la cuadratura adaptativa y estima el error.
 
---
## D. Python con SciPy
Con `scipy.integrate.quad` se obtuvo:

I=2.559824508302

E_{estimado}=1.458\times10^{-10}

con tolerancias absoluta y relativa de \(10^{-10}\).
 
SciPy presenta una interfaz más sencilla y compacta que GSL, mientras que GSL permite un control más explícito de los parámetros del algoritmo.
 
## 1. ¿Qué cambia en el trapecio al aumentar n?
 
Al aumentar \(n\), disminuye el paso \(h\), se representa mejor la función y disminuye el error. A cambio, aumenta el número de evaluaciones y el costo computacional.
 
## 2. ¿Qué diferencia hay entre aumentar n y reducir la tolerancia de un método adaptativo?
 
Aumentar \(n\) refina uniformemente todo el intervalo. Reducir la tolerancia exige mayor precisión a un método adaptativo, que decide internamente dónde necesita realizar más subdivisiones.
 
## 3. ¿Mayor precisión implica necesariamente mayor costo?
Generalmente sí implica más evaluaciones y mayor costo, pero no necesariamente de forma proporcional. Los métodos adaptativos pueden concentrar el esfuerzo computacional donde realmente se necesita.
 
## 4. Compare el error estimado por GSL/SciPy con el error respecto a la referencia
GSL obtuvo:
 
E_{estimado}=2.842\times10^{-14}

SciPy obtuvo:

E_{estimado}=1.458\times10^{-10}

En ambos casos, el error respecto a la referencia fue aproximadamente:

E_{ref}=4.441\times10^{-16}
 
El error estimado es calculado internamente por cada algoritmo, mientras que el error respecto a la referencia se obtiene comparando el resultado numérico con el valor analítico. Por ello, ambos errores no tienen que ser iguales.
 

### --------------- EJERCICIO #2 ----------------------- 
Se cargaron los 50 datos experimentales desde `datos_sensor.csv` utilizando NumPy. Se verificó que los datos fueran equiespaciados, con paso:

h=0.2
 
y que no existieran valores faltantes. Utilizando `scipy.integrate.trapezoid` se obtuvo:

I_{trap}=21.190846339900
 
Mediante `scipy.integrate.simpson` se obtuvo:

I_{simp}=21.192113098332
 
La diferencia absoluta entre los métodos fue:

|I_{simp}-I_{trap}|=1.266758\times10^{-3}

correspondiente a una diferencia relativa de:

0.005978\%
 
### Análisis
 
La diferencia entre las dos aproximaciones es pequeña respecto a la magnitud total de la integral, por lo que para este conjunto de datos ambos métodos producen resultados muy similares.
Es importante señalar que existen 50 puntos y, por tanto, 49 subintervalos. La regla de Simpson 1/3 compuesta estándar requiere un número par de subintervalos, razón por la cual no pudo aplicarse directamente mediante la implementación explícita realizada en Octave.
En Python se utilizó la función `scipy.integrate.simpson`, tal como se propone en el taller. Al tratarse de una función de biblioteca, esta gestiona internamente la integración de los datos disponibles y permite obtener una aproximación aun para este conjunto de 50 mediciones. Los resultados muestran además que el trapecio implementado en Octave, C/C++ y Python produjo el mismo valor:
 
I_{trap}=21.190846339900
 
lo cual confirma la consistencia de la implementación al trabajar sobre el mismo conjunto de datos.

# 6. Extensión: diferenciación numérica
 
Para los 50 datos equiespaciados, con paso:

h=0.2

se implementaron las aproximaciones por diferencia hacia adelante y diferencia central:

f'(x_i)\approx\frac{f(x_{i+1})-f(x_i)}{h}

f'(x_i)\approx\frac{f(x_{i+1})-f(x_{i-1})}{2h}
 
Las dos expresiones fueron implementadas en GNU Octave, C/C++ y Python, obteniéndose los mismos resultados en los tres entornos. Por ejemplo:
 
| x | Diferencia hacia adelante | Diferencia central |
|---:|---:|---:|
| 0.20 | 0.085202 | 0.147110 |
| 1.00 | -0.025335 | -0.047212 |
| 2.00 | 0.353136 | 0.338052 |
| 4.00 | -0.433332 | -0.461445 |
| 6.00 | -0.150228 | -0.100005 |
| 8.00 | 0.531667 | 0.490830 |
| 9.60 | -0.068266 | -0.044243 |

## Comparación de los métodos
 
La diferencia hacia adelante utiliza el punto actual y el siguiente:

f'(x_i)\approx\frac{f(x_{i+1})-f(x_i)}{h}
 
por lo que proporciona una aproximación basada únicamente en información hacia un lado del punto.
 
La diferencia central utiliza información a ambos lados:

f'(x_i)\approx\frac{f(x_{i+1})-f(x_{i-1})}{2h}
 
Esto produce una aproximación más simétrica alrededor de \(x_i\) y, para funciones suficientemente suaves, normalmente presenta mayor precisión que la diferencia hacia adelante para un mismo tamaño de paso.
 
## Tratamiento de los extremos
Las fórmulas no pueden utilizarse de la misma forma en todos los puntos. En \(x=0\) puede utilizarse la diferencia hacia adelante:

f'(x_0)\approx\frac{f(x_1)-f(x_0)}{h}
 
pero no puede calcularse la diferencia central porque no existe una medición anterior a \(x_0\).
 
En el último punto, \(x=9.8\), no puede aplicarse ninguna de las dos fórmulas implementadas directamente, ya que ambas requerirían un punto posterior. Por esta razón, en las implementaciones realizadas se asignó `NaN` cuando no existían los datos requeridos. Una alternativa para el último punto sería utilizar una diferencia hacia atrás:

f'(x_n)\approx\frac{f(x_n)-f(x_{n-1})}{h}
 
Sin embargo, se conservaron las dos aproximaciones solicitadas en el taller para realizar la comparación de manera consistente.
 
## Sensibilidad de la diferenciación al ruido
 
La diferenciación numérica suele ser más sensible al ruido que la integración porque calcula diferencias entre valores próximos y posteriormente divide estas diferencias por un paso \(h\).
 
Por ejemplo: f'(x_i)\approx\frac{f(x_{i+1})-f(x_i)}{h}
 
Si las mediciones contienen pequeñas perturbaciones o ruido, estas aparecen directamente en la diferencia del numerador. Al dividir por un valor pequeño de \(h\), dichas perturbaciones pueden amplificarse significativamente. La integración presenta el comportamiento contrario: al acumular o promediar información sobre un intervalo, las pequeñas perturbaciones pueden compensarse parcialmente. Por esta razón, disminuir el paso de discretización no garantiza automáticamente una mejor estimación de la derivada cuando se trabaja con datos experimentales ruidosos.
 
## Comparación entre entornos
Octave, C/C++ y Python produjeron los mismos valores de las derivadas porque se utilizaron los mismos datos, el mismo paso \(h=0.2\) y las mismas fórmulas de diferencias finitas. La principal diferencia entre las implementaciones se encuentra en la forma de representar y manipular los datos. Octave y Python permiten realizar las operaciones de manera vectorizada y compacta, mientras que en C/C++ fue necesario manejar explícitamente los vectores, índices y ciclos.
 
### -------------- Comparación final --------------------

A partir de las implementaciones realizadas en GNU Octave, C/C++ con GSL y Python con SciPy, se obtiene la siguiente comparación:
 
| Aspecto | GNU Octave | C/C++ + GSL | Python + SciPy |

| Facilidad de implementación | Alta | Media | Alta |

| Control del algoritmo | Medio | Alto | Medio |

| Manejo de datos | Sencillo mediante vectores y matrices | Requiere manejo explícito de vectores, índices y lectura de archivos | Sencillo mediante NumPy |

| Integración de funciones | `quadgk` | `gsl_integration_qag` | `scipy.integrate.quad` |

| Integración de datos | Implementación explícita del trapecio | Trapecio explícito e interpolación con GSL | `trapezoid` y `simpson` |

| Interpolación | Disponible mediante funciones del entorno | Spline cúbico mediante GSL | Disponible mediante SciPy |

| Diferenciación | Implementación directa y vectorizada | Implementación explícita mediante ciclos | Implementación directa con NumPy |

| Configuración | Baja | Mayor configuración de compilación y bibliotecas | Requiere entorno y paquetes NumPy/SciPy |

| Transparencia del procedimiento | Alta en implementaciones manuales | Muy alta | Alta en implementación manual |
 
## Comparación de los resultados
Para la integración de la función del Ejercicio 1, los tres entornos produjeron prácticamente el mismo resultado:

I_{ref}=2.559824508302
 
Las rutinas adaptativas de Octave, GSL y SciPy alcanzaron valores prácticamente coincidentes con esta referencia, aunque las estimaciones internas del error fueron diferentes.
 
Para los datos experimentales del Ejercicio 2, la regla del trapecio produjo exactamente el mismo resultado en los tres entornos:

I_{trap}=21.190846339900
 
Las estrategias adicionales produjeron:

I_{spline,GSL}=21.191888257606

I_{Simpson,SciPy}=21.192113098332

Las pequeñas diferencias se deben al método utilizado para representar e integrar la información entre las mediciones y no al lenguaje de programación.
 
## ¿Qué entorno ofrece mayor transparencia?
La implementación en C/C++ permite observar de manera explícita aspectos como los ciclos, índices, almacenamiento de datos, parámetros de las rutinas y manejo de memoria. Esto permite visualizar con mayor detalle las operaciones necesarias para ejecutar el método numérico.
 
Octave también ofrece buena transparencia cuando los métodos se implementan manualmente, pero permite trabajar de forma más directa con vectores y matrices.
 
Python presenta una sintaxis compacta y, mediante NumPy y SciPy, permite resolver los problemas con pocas instrucciones. Sin embargo, cuando se utilizan funciones de biblioteca, una parte importante del algoritmo queda encapsulada dentro de estas funciones.
 
## ¿Qué entorno ofrece mayor productividad?
Para los ejercicios realizados, Octave y Python permiten desarrollar las implementaciones con menos código que C/C++. Python facilita especialmente la lectura, procesamiento y manipulación de datos mediante NumPy, mientras que SciPy proporciona directamente diferentes algoritmos numéricos.
 
Octave también resulta conveniente para cálculos numéricos y operaciones matriciales, con una sintaxis orientada directamente a este tipo de problemas.
 
C/C++ requiere más instrucciones y una etapa adicional de compilación, pero permite un control más explícito sobre la implementación y el uso de bibliotecas como GSL.
 
## ¿En qué caso elegiría cada uno?
GNU Octave puede ser conveniente para el desarrollo académico, análisis numérico y validación rápida de algoritmos matemáticos.
 
C/C++ con GSL puede ser adecuado cuando se requiere un mayor control de la implementación, integración con software desarrollado en C/C++ o manejo explícito de recursos computacionales.
 
Python con NumPy y SciPy puede ser conveniente para análisis de datos, prototipado y aplicaciones donde sea importante combinar métodos numéricos con procesamiento, visualización y otras herramientas científicas.
 
### -------------- Reflexión y evocación -----------------
 
## 1. ¿Qué concepto previo resultó más importante para comprender el taller?
Uno de los conceptos más importantes fue la discretización. Comprender la relación entre el número de subintervalos \(n\), el tamaño del paso \(h\) y el error permite interpretar por qué una aproximación numérica mejora al refinar la malla.
 
También resultó importante distinguir entre trabajar con una funciónanalítica y trabajar únicamente con mediciones experimentales. Cuando se conoce la función es posible disponer de una referencia analítica y evaluar directamente el error. En cambio, con datos experimentales solo se dispone de las mediciones y cualquier interpolación entre ellas constituye una aproximación adicional.
 
## 2. ¿Qué error conceptual considera más probable al resolver estos problemas?
Un error conceptual posible es asumir que aumentar indiscriminadamente el número de puntos siempre produce una solución mejor. En integración, una discretización más fina generalmente reduce el error de truncamiento para funciones suficientemente suaves, aunque aumenta el costo computacional. En diferenciación de datos experimentales, un paso más pequeño puede aumentar la influencia del ruido debido a que se calculan diferencias entre valores cercanos y posteriormente se divide por \(h\).
 
Otro error posible es confundir el error estimado por una rutina adaptativa con el error real. El error estimado es calculado internamente por el algoritmo, mientras que el error respecto a una referencia solo puede determinarse directamente cuando se dispone de un valor de referencia confiable.
 
## COMPARACIÓN DE RESULTADOS
| Criterio | Octave | Python | C/C++ + GSL |
| Carga de datos | Se utilizó `dlmread` para cargar el archivo CSV. | Se utilizó `np.loadtxt` para cargar el archivo CSV. | Se realizó la lectura del CSV mediante `ifstream` y almacenamiento en vectores. |

| Cálculo de ẋ, ẏ | Se implementaron diferencias centrales de forma explícita y diferencias unilaterales en los extremos. | Se utilizó `np.gradient`, que aplica diferencias centrales en los puntos interiores y aproximaciones unilaterales en los extremos. | Se implementaron directamente las diferencias centrales sobre los datos y diferencias unilaterales en los extremos. |

| Cálculo de v | Se calculó a partir de las componentes ẋ y ẏ mediante operaciones vectorizadas. | Se calculó a partir de ẋ y ẏ utilizando operaciones con arreglos de NumPy. | Se calculó a partir de ẋ y ẏ mediante un ciclo sobre los vectores. |

| Cálculo de θ | Se obtuvo a partir de las componentes de velocidad y se corrigió la continuidad del ángulo. | Se obtuvo a partir de las componentes de velocidad y se corrigió la continuidad del ángulo. | Se obtuvo a partir de las componentes de velocidad y se implementó la corrección de los saltos angulares. |

| Cálculo de ω | Se aplicaron diferencias centrales sobre θ y diferencias unilaterales en los extremos. | Se utilizó `np.gradient` sobre θ. | Se aplicaron diferencias centrales sobre θ y diferencias unilaterales en los extremos. |

| Manejo de arreglos | Sencillo mediante vectores y operaciones directas. | Muy sencillo mediante arreglos y operaciones vectorizadas de NumPy. | Requiere declarar y recorrer explícitamente los vectores. |

| Facilidad de implementación | Alta; permite implementar directamente el método numérico con pocas líneas. | Alta; las funciones de NumPy reducen considerablemente el código necesario. | Requiere más código para la lectura, almacenamiento y procesamiento de los datos. |

| Control sobre el algoritmo | Alto, debido a que las diferencias se implementaron explícitamente. | El uso de `np.gradient` simplifica el procedimiento, aunque parte del cálculo queda gestionado por la biblioteca. | Alto, ya que las operaciones y el tratamiento de los datos se implementan explícitamente. |

| Tiempo de ejecución | 0.016039 s | 1.491737e-03 s | 1.346024e-03 s |

## PREGUNTAS DE ANÁLISIS
### 1. ¿Qué diferencia existe entre calcular la velocidad lineal a partir de ẋ, ẏ y calcularla directamente desde diferencias de posición?
Al calcular primero ẋ y ẏ se obtienen las componentes de la velocidad en cada dirección. Esto permite conocer no solo qué tan rápido se mueve el robot, sino también la dirección de su movimiento.
Si la velocidad se calcula directamente a partir de la distancia entre posiciones consecutivas, se obtiene principalmente su magnitud, pero se pierde información sobre las componentes del movimiento. Por esta razón, calcular primero ẋ y ẏ permite realizar posteriormente el cálculo de la orientación y de la velocidad angular.
 
### 2. ¿Por qué un error pequeño en ẋ, ẏ puede producir un efecto mayor al calcular ω?
Las componentes ẋ y ẏ se utilizan para determinar la orientación del movimiento del robot. Por esta razón, un pequeño error en estas componentes puede producir una variación en la orientación.
Posteriormente, la velocidad angular se obtiene calculando cómo cambia esa orientación con el tiempo. Como este proceso implica una nueva diferenciación numérica, las pequeñas variaciones o errores pueden amplificarse y producir un efecto mayor en el valor de la velocidad angular.
  
### 3. ¿Qué ocurre si se reduce h manteniendo un nivel de ruido fijo en las posiciones?
Al reducir el paso h se tienen muestras más cercanas entre sí, lo cual puede mejorar la aproximación de la derivada cuando los datos son suaves.
Sin embargo, si el nivel de ruido se mantiene, las pequeñas variaciones producidas por ese ruido adquieren mayor importancia al calcular las diferencias entre posiciones.
Por esta razón, disminuir h no siempre produce mejores resultados. Con datos ruidosos, la estimación de las velocidades puede volverse más sensible al ruido.

## REFLEXIÓN
### 1. Reconstrucción del procedimiento
El procedimiento inicia con los datos de posición del robot en los ejes x e y para diferentes instantes de tiempo. A partir de estas posiciones se calculan las derivadas respecto al tiempo, obteniendo las componentes de velocidad en cada dirección. Con estas componentes se determina la velocidad lineal del robot y su orientación instantánea. Antes de calcular la velocidad angular es necesario verificar que la orientación tenga un comportamiento continuo, evitando saltos asociados a la representación de los ángulos. Finalmente, se calcula la variación de la orientación respecto al tiempo para obtener la velocidad angular. De esta manera, el procedimiento sigue el orden:
 
posición → componentes de velocidad → velocidad lineal y orientación → velocidad angular.
 
Los principales errores numéricos pueden aparecer durante el cálculo de las derivadas, debido al tamaño del paso de tiempo y al ruido presente en los datos. También se debe tener cuidado en los extremos de la serie de datos, donde no es posible aplicar directamente el método de diferencias centrales.

### 2. Transferencia del aprendizaje
Si se trabajara con datos reales de un robot y con una frecuencia de muestreo diferente, se mantendría el procedimiento general utilizado en el taller para obtener la velocidad lineal y angular a partir de las posiciones medidas.
 
Sin embargo, antes de realizar la diferenciación sería importante revisar el intervalo de tiempo entre las muestras y el nivel de ruido de los sensores. Si las mediciones presentan variaciones o ruido, podría ser necesario realizar un tratamiento previo de los datos para evitar que estos errores se amplifiquen al calcular las velocidades.
 
Por lo tanto, la selección del método de diferenciación dependería principalmente de la frecuencia de muestreo, la calidad de los datos y el nivel de ruido presente en las mediciones.

## CONCLUSIONES
1. Los tres entornos utilizados permitieron obtener resultados consistentes para la velocidad lineal y angular del robot. Esto permitió comprobar que un mismo método de diferenciación numérica puede implementarse mediante diferentes herramientas computacionales y obtener resultados equivalentes.
 
2. La diferenciación numérica permite estimar las velocidades a partir de datos discretos de posición, pero sus resultados dependen del paso de tiempo y de la calidad de los datos. En aplicaciones reales es importante considerar el ruido de las mediciones, ya que este puede amplificarse durante el cálculo de las derivadas, especialmente al determinar la velocidad angular.
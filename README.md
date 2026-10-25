# Taller sobre Vecinos Cercanos y Agrupación – KNN y kd-tree
## 1. Preguntas exploratorias
### P1. ¿Qué diferencias conceptuales encuentras entre un método basado en distancias como KNN y una estructura espacial como el kd-tree?
KNN permite identificar los registros más cercanos a un punto mediante una métrica de distancia. En su implementación directa, calcula la distancia entre el punto de interés y los demás registros, seleccionando aquellos que presentan mayor similitud.
Por otra parte, kd-tree es una estructura de datos que organiza los registros mediante divisiones sucesivas del espacio multidimensional. Esta organización permite reducir potencialmente el número de comparaciones necesarias durante la búsqueda.
La principal diferencia es que KNN establece el criterio para encontrar los vecinos más cercanos, mientras que kd-tree proporciona una estructura para realizar esa búsqueda de manera más eficiente en determinadas condiciones.
 
### P2. ¿En qué escenarios prácticos crees que KNN puede ser más eficiente que un método basado en kd-tree, y viceversa?
KNN directo puede resultar más eficiente cuando se trabaja con conjuntos pequeños de datos, donde calcular las distancias de todos los registros requiere poco tiempo. También es conveniente cuando las consultas son ocasionales o cuando los datos cambian constantemente.
Por otra parte, kd-tree puede ofrecer ventajas cuando se manejan conjuntos más grandes, con pocas variables y numerosas consultas sobre datos relativamente estables.
En sistemas eléctricos, KNN directo podría utilizarse para analizar mediciones de un grupo reducido de sensores, mientras que kd-tree podría emplearse para consultar registros históricos de numerosos equipos.
 
### P3. ¿Qué impacto tienen la dimensionalidad y el tamaño de los datos en el desempeño de estos métodos?
 
A medida que aumenta el número de registros, KNN directo necesita calcular más distancias, incrementando el tiempo de procesamiento.
kd-tree puede reducir el número de comparaciones mediante la organización espacial de los datos. Sin embargo, cuando aumenta considerablemente la cantidad de variables, su eficiencia puede disminuir porque resulta más difícil descartar regiones durante la búsqueda.
Por esta razón, kd-tree suele ser más conveniente para conjuntos de baja dimensionalidad, mientras que KNN directo puede resultar competitivo para conjuntos pequeños o espacios de muchas dimensiones.
 
### P4. ¿Cómo influye la elección de la métrica de distancia (euclidiana, Manhattan, etc.) en los resultados de KNN?
La métrica de distancia determina cómo se evalúa la similitud entre los registros y puede modificar cuáles son seleccionados como vecinos más cercanos.
La distancia euclidiana considera la separación geométrica entre dos puntos, mientras que Manhattan suma las diferencias absolutas de cada variable.
En aplicaciones eléctricas, la elección debe considerar las características de las mediciones analizadas. También es necesario normalizar las variables cuando presentan diferentes escalas, para evitar que una de ellas domine el cálculo de distancias.
En este ejercicio se utilizó la distancia euclidiana con las variables previamente normalizadas.
 
### P5. ¿Qué limitaciones prácticas podrían surgir al implementar estos métodos en sistemas de ingeniería eléctrica de gran escala?
Las principales limitaciones incluyen el procesamiento de grandes volúmenes de datos, los tiempos de respuesta, la actualización continua de mediciones y la presencia de valores erróneos o faltantes.
También debe considerarse que el aumento del número de variables puede afectar el rendimiento de los métodos y dificultar la interpretación de los resultados.
En sistemas eléctricos, encontrar sensores con mediciones similares no necesariamente significa identificar una falla. Por ello, los resultados deben complementarse con información sobre condiciones operativas, valores nominales e historial de los equipos.
 
---
 
## 2. Ejercicio de aplicación en ingeniería eléctrica
### Actividad 1. Aplicación de KNN para identificar los cinco vecinos más cercanos
 
Se utilizó el archivo `Datos_MCEI.csv`, que contiene 100 registros con mediciones de voltaje, corriente y potencia aproximada. 
Para identificar el sensor con anomalía de voltaje, se seleccionó el registro que presentaba la mayor desviación absoluta respecto a la mediana de voltaje del conjunto.
El sensor seleccionado fue el 74, con las siguientes mediciones:
| Variable            |  Valor   |
| Voltaje             | 1.181 V  |
| Corriente           |  507 A   |
| Potencia aproximada | 60.129 W |
 
Posteriormente, se normalizaron las variables mediante Z-score y se aplicó KNN directo utilizando distancia euclidiana, con K = 5.
 
Los resultados fueron:
 
| Sensor vecino | Voltaje (V) | Corriente (A) | Potencia (W) | Distancia |
| 94 | 1.250 | 491 | 61.514 | 0,154176 |
| 48 | 1.188 | 488 | 57.271 | 0,261548 |
| 27 | 1.193 | 572 | 68.055 | 0,765876 |
| 86 | 1.203 | 409 | 49.947 | 1,038214 |
| 47 | 1.233 | 587 | 71.778 | 1,074008 |
 
Los sensores 94 y 48 presentan las menores distancias respecto al sensor 74, por lo que son los registros con mayor similitud considerando conjuntamente las tres variables.
 
### Actividad 2. Construcción de kd-tree y búsqueda de vecinos
 
Se construyó una estructura kd-tree mediante la función `cKDTree` de SciPy, utilizando los mismos datos normalizados empleados en KNN directo.
Posteriormente, se consultaron los cinco vecinos más cercanos al sensor 74, excluyendo el propio registro.
Los resultados fueron:
| Sensor vecino | Distancia |
| 94 | 0,154176 |
| 48 | 0,261548 |
| 27 | 0,765876 |
| 86 | 1,038214 |
| 47 | 1,074008 |
 
Los vecinos encontrados mediante kd-tree coinciden exactamente con los obtenidos mediante KNN directo, tanto en los registros seleccionados como en sus distancias.
 
### Actividad 3. Comparación de resultados: tiempo de ejecución, precisión y facilidad de interpretación
 
**Tiempo de ejecución**
 
Se realizaron 1.000 repeticiones para comparar los tiempos de consulta de ambos métodos, reutilizando la estructura kd-tree previamente construida.
 
| Indicador | KNN directo | kd-tree |
| Tiempo medio | 0,081477 ms | 0,125500 ms |
| Tiempo mediano | 0,056978 ms | 0,090102 ms |
| Repeticiones | 1.000 | 1.000 |
 
KNN directo presentó un tiempo mediano aproximadamente 36,8 % menor que kd-tree.
Este resultado corresponde al conjunto de 100 registros y no significa que KNN directo sea siempre más rápido para conjuntos de mayor tamaño.
 
**Precisión en la identificación de vecinos**
Ambos procedimientos identificaron los mismos cinco vecinos del sensor 74, con una coincidencia del 100 %.
Esta coincidencia demuestra que las dos implementaciones proporcionaron resultados equivalentes para la búsqueda realizada. No representa una medida de precisión para detectar fallas eléctricas reales.
 
**Facilidad de interpretación**
KNN directo resulta más sencillo de interpretar porque calcula las distancias respecto a todos los registros y selecciona los más cercanos.
kd-tree requiere organizar previamente los datos mediante una estructura espacial, aunque permite realizar consultas de vecinos de forma eficiente en determinadas condiciones.
 
**Representación gráfica de los sensores**
Distribución de sensores y vecinos cercanos (grafica_knn.png)
La gráfica muestra la distribución tridimensional de los sensores utilizando las variables normalizadas. El sensor 74 se representa en rojo y sus cinco vecinos más cercanos en azul.
 
Se observa que el sensor 74 pertenece a un grupo de registros con voltajes próximos a 1200 V, separado de otro grupo con voltajes cercanos a 12000 V.
 
**Comparación gráfica de tiempos**
Comparación de tiempos de ejecución (comparacion_tiempos.png)
La gráfica presenta los tiempos medianos de consulta obtenidos durante las 1.000 repeticiones. Para este conjunto de datos, KNN directo presentó un menor tiempo de ejecución.
 
### Actividad 4. ¿Cuál método sería más adecuado para un sistema de monitoreo en tiempo real en ingeniería eléctrica?
La elección del método depende principalmente del número de sensores, la cantidad de variables, la frecuencia de actualización de las mediciones y el tiempo de respuesta requerido. Para el conjunto de 100 sensores analizado, KNN directo resulta más adecuado, debido a que presentó menores tiempos de consulta y permitió identificar correctamente los cinco vecinos más cercanos.
Sin embargo, en sistemas eléctricos con una mayor cantidad de registros y consultas repetitivas, kd-tree podría ofrecer ventajas al utilizar una estructura espacial previamente construida.
También es necesario considerar que los sistemas de monitoreo eléctrico requieren interpretar las mediciones según las condiciones de operación y los valores nominales de los equipos. La identificación de vecinos cercanos permite encontrar comportamientos similares, pero no confirma por sí sola la existencia de una falla.
Por lo tanto, para este ejercicio se recomienda KNN directo, mientras que para sistemas de mayor escala sería necesario comparar nuevamente el rendimiento de ambos procedimientos.
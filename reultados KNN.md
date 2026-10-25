# Taller sobre Vecinos Cercanos y Agrupación - MCEI

Laura Vanessa Gómez Fonseca

## Parte 1: Preguntas Exploratorias

### 1: ¿Qué diferencias conceptuales encuentras entre un método basado en distancias como KNN y una estructura espacial

La diferencia principal radica en cómo abordan la búsqueda. El algoritmo KNN clásico (por fuerza bruta) es perezoso (lazy learning): no construye un modelo previo, sino que ante cada consulta calcula la distancia matemática (ej. euclidiana) desde el punto objetivo hacia todos los demás puntos del conjunto de datos. Por otro lado, un kd-tree es una estructura de datos jerárquica (un árbol binario) que pre-procesa la información dividiendo el espacio multidimensional mediante hiperplanos. Al hacer una consulta, el kd-tree descarta ramas enteras del árbol basándose en la geometría del espacio, evitando calcular distancias contra puntos que evidentemente están lejos.

### 2: ¿En qué escenarios prácticos crees que KNN puede ser más eficiente que un método basado en kd-tree, y viceversa?

**KNN es más eficiente cuando:** El conjunto de datos es muy dinámico (los puntos cambian, se añaden o eliminan constantemente). En estos casos, reconstruir la estructura del árbol kd-tree a cada momento sería computacionalmente más costoso que simplemente calcular las distancias en bruto. También es preferible en espacios de altísima dimensionalidad.

**kd-tree es más eficiente cuando:** Se tiene un conjunto de datos grande y relativamente estático (como un histórico de mediciones) sobre el cual se harán muchas consultas repetitivas (como el monitoreo de anomalías).

### 3: ¿Qué impacto tienen la dimensionalidad y el tamaño de los datos en el desempeño de estos métodos?

- *Tamaño de los datos (N):* En KNN por fuerza bruta, el tiempo de búsqueda crece linealmente respecto a la cantidad de datos. En kd-tree, el tiempo de búsqueda crece de forma logarítmica, haciéndolo enormemente superior en grandes volúmenes.
- *Dimensionalidad (D):* A medida que aumentan las variables (dimensiones), el rendimiento del kd-tree decae drásticamente (fenómeno conocido como la maldición de la dimensionalidad), llegando a comportarse igual o peor que una búsqueda de fuerza bruta si la cantidad de dimensiones es muy cercana a la cantidad de datos.

### 4: ¿Cómo influye la elección de la métrica de distancia (euclidiana, Manhattan, etc.) en los resultados de KNN?
La métrica define qué significa estar "cerca". La distancia Euclidiana mide la línea recta más corta entre dos puntos, lo cual es ideal para magnitudes físicas continuas y relacionadas geométricamente (como en este caso). La distancia de Manhattan, en cambio, mide la distancia por bloques o ejes ortogonales, lo cual puede ser mejor si las variables (voltaje, corriente, temperatura) tuvieran escalas dispares o no tuvieran una relación lineal directa. Usar la métrica incorrecta puede agrupar sensores que operativamente no tienen el mismo comportamiento de falla.

### 5: ¿Qué limitaciones prácticas podrían surgir al implementar estos métodos en sistemas de ingeniería eléctrica de gran escala?
En una red inteligente (Smart Grid) a gran escala, se generan millones de datos por segundo. Las principales limitaciones son:

Costo Computacional vs. Tiempo Real: El KNN estándar colapsaría el procesador tratando de comparar un dato nuevo contra millones de históricos cada segundo.

Uso de Memoria: Tanto el histórico de KNN como el árbol kd-tree deben residir en la memoria RAM para ser eficientes. Modelos masivos pueden exceder la capacidad física del hardware.

Falta de paralelización fácil en árboles: Mientras que KNN se puede paralelizar fácilmente con herramientas como OpenMP, recorrer un árbol kd-tree de forma paralela es un desafío técnico mayor.

## Parte 2: Ejercicio de Aplicación en Ingeniería Eléctrica

**Contexto del Problema:**
Se cuenta con una red de 100 sensores industriales, midiendo voltaje, corriente y temperatura. Se reporta una anomalía en un sensor específico con las mediciones: [240.5 V, 5.6 A, 85.2 °C]. El objetivo es encontrar los 5 sensores con comportamiento más similar para analizar la causa de la anomalía.

### 1 y 2. Implementación de KNN y kd-tree (Resultados en Python)
    
Tras ejecutar el script en Python utilizando la librería scikit-learn con los datos del archivo Datos_MCEI.csv, se obtuvieron los siguientes resultados:

==================================================
--- 1. Resultados KNN (K-Nearest Neighbours) ---
Sensores (Índices): [62 44 97 99 63]
Distancias: [np.float64(13151.57), np.float64(13492.56), np.float64(13658.11), np.float64(16326.94), np.float64(45741.99)]
Tiempo de ejecución: 0.005715 segundos

--- 2. Resultados kd-tree (Búsqueda Espacial) ---
Sensores (Índices): [62 44 97 99 63]
Distancias: [np.float64(13151.57), np.float64(13492.56), np.float64(13658.11), np.float64(16326.94), np.float64(45741.99)]
Tiempo de ejecución: 0.004143 segundos
==================================================

### 3. Comparación de Resultados
- Precisión de la identificación: Ambos algoritmos arrojaron un 100% de coincidencia al identificar los mismos 5 índices de sensores y reportar exactamente las mismas distancias, validando la precisión matemática de ambos.

- Tiempo de ejecución: El método estructurado kd-tree fue más rápido que el método de fuerza bruta (KNN), incluso operando en un conjunto de datos pequeño (100 sensores). La consulta espacial tardó aproximadamente 4.1 milisegundos, frente a los 5.7 milisegundos de la evaluación secuencial.

- Facilidad de interpretación: El algoritmo KNN es más directo e intuitivo, ya que su única instrucción es medir la distancia punto a punto. El kd-tree requiere comprender la partición del espacio tridimensional y la lógica de ramificación, haciéndolo conceptualmente más complejo pero algorítmicamente más robusto.

### 4. Conclusión para Monitoreo en Tiempo Real

Para un sistema de monitoreo en tiempo real en ingeniería eléctrica, el método basado en kd-tree es indiscutiblemente más adecuado.
En una red industrial real, los datos entran en flujos masivos y continuos de alta frecuencia. Evaluar iterativamente cada nueva señal contra la base histórica (como lo hace KNN puro) generaría retardos inaceptables para la activación de alarmas. Por el contrario, estructurar el espacio de datos en un kd-tree de forma periódica permite que las consultas de anomalías (como el evento de la caída de tensión en el sensor) se ejecuten en tiempos mínimos, garantizando la agilidad de los sistemas de protección o mantenimiento predictivo.
# Taller de Introducción a la Geometría Computacional con CGAL

---

## 1. Fundamentos y Preguntas de Investigación

### 2.1. ¿Qué es la geometría computacional y qué problemas de ingeniería permite resolver?

La geometría computacional es la disciplina que diseña y analiza algoritmos eficientes y estructuras de datos para resolver problemas geométricos mediante computadoras.

*   **Geometría euclidiana:** Sistema axiomático-teórico que estudia las propiedades del espacio sin un sistema de referencia.
*   **Geometría analítica:** Utiliza coordenadas y ecuaciones algebraicas para representar figuras.
*   **Geometría computacional:** Se enfoca en la **eficiencia algorítmica** y la **robustez numérica**, lidiando con las limitaciones del hardware (precisión finita).

**Objetos geométricos computacionales:**
*   **Puntos:** Tuplas de coordenadas (ej. `(x, y)` en 2D).
*   **Segmentos:** Definidos por dos puntos extremos.
*   **Polígonos y Poliedros:** Secuencias de segmentos o caras que forman regiones cerradas.
*   **Superficies y Nubes de Puntos:** Conjuntos discretos de puntos que aproximan formas continuas.

**Cinco problemas clásicos:**
1.  **Envolvente convexa (Convex Hull):** Polígono mínimo que contiene todos los puntos.
2.  **Triangulación de Delaunay:** Conexión de puntos mediante triángulos con círculos circunscritos vacíos.
3.  **Diagramas de Voronoi:** División del espacio en regiones de proximidad.
4.  **Búsqueda del vecino más cercano:** Encontrar el punto más próximo a una consulta.
5.  **Detección de intersecciones:** Determinar si dos o más objetos se cruzan.

**Aplicaciones en ingeniería y Contextualización:**
*   **Robótica:** Planificación de trayectorias libres de colisiones. *Entrada:* Mapa de obstáculos poligonales.
*   **Sistemas CAD:** Modelado de sólidos y operaciones booleanas. *Entrada:* Mallas poligonales de piezas.
*   **Imágenes médicas:** Reconstrucción 3D de estructuras anatómicas. *Entrada:* Nubes de puntos de tomografías.

---

### 2.2. Principales algoritmos y estructuras de datos

| Algoritmo | Problema que resuelve | Principio matemático | Complejidad | Aplicación en ingeniería |
| :--- | :--- | :--- | :--- | :--- |
| **Envolvente Convexa** | Contorno convexo mínimo. | Algoritmo de Andrew (*Monotone Chain*). | \(O(n \log n)\) | Detección de contornos en visión. |
| **Triangulación de Delaunay** | Conectar puntos con triángulos "vacíos". | Criterio del círculo circunscrito vacío. | \(O(n \log n)\) | Generación de mallas para FEM. |
| **Diagrama de Voronoi** | Dividir espacio por proximidad. | Dual de la triangulación de Delaunay. | \(O(n \log n)\) | Planificación de rutas y cobertura. |
| **Vecino más cercano** | Punto más próximo a una consulta. | Búsqueda en jerarquía espacial. | Consulta \(O(\log n)\) | Agrupamiento (*clustering*). |
| **Detección de intersecciones** | Saber si dos objetos se cruzan. | Barrido de plano (*plane sweep*). | \(O(n \log n)\) | Diseño de circuitos y colisiones. |

**Contextualización (100 sensores):** El **Diagrama de Voronoi** se usaría para estudiar la cobertura (cada región indica el área de influencia del sensor más cercano). La **Triangulación de Delaunay** serviría para analizar la conectividad de la red, ya que sus aristas conectan cada sensor con sus vecinos directos.

---

### 2.3. Precisión numérica y robustez

1.  **Problemas IEEE-754:** La precisión finita del punto flotante genera errores de redondeo, especialmente en operaciones con números muy cercanos.
2.  **Colinealidad:** Tres puntos son colineales si el área del triángulo es cero. Computacionalmente se evalúa con el signo de un determinante. Con errores de redondeo, un determinante que debería ser cero puede dar un valor muy pequeño, clasificando erróneamente los puntos.
3.  **Configuraciones degeneradas:** Casos límite como puntos colineales, cocirculares o duplicados. Los algoritmos suelen asumir "posición general" y pueden fallar si no se manejan.
4.  **Exactitud, precisión y robustez:** La **robustez** es la capacidad del algoritmo para dar una respuesta *cualitativamente correcta* (ej. "son colineales" o "no lo son") incluso con errores de redondeo.

**Experimento conceptual (\(\epsilon\)):** Con \(A=(0,0), B=(1,1), C=(2, 2+\epsilon)\). Si \(\epsilon\) es muy pequeño (ej. \(10^{-16}\)), la representación en `double` redondea \(C\) a \((2,2)\). El determinante de orientación da cero y los clasifica como colineales, cuando matemáticamente no lo son.

**Solución de CGAL:** Separa los **predicados** (tests geométricos) de las **construcciones** (crear objetos).
*   **EPICK (Exact Predicates Inexact Constructions):** Predicados exactos (robustos), construcciones con `double` (rápidas).
*   **EPECK (Exact Predicates Exact Constructions):** Todo exacto mediante números racionales. Más lento pero totalmente robusto.

---

### 2.4. ¿Qué es CGAL y cómo está organizada?

CGAL es una biblioteca de C++ de código abierto que proporciona algoritmos y estructuras de datos eficientes para geometría computacional. Está en C++ por su alto rendimiento y programación genérica (plantillas).

*   **Geometry Kernel:** Capa fundamental que define los objetos (puntos, segmentos) y las operaciones.
*   **Predicates:** Operaciones que devuelven un valor discreto (ej. orientación).
*   **Constructions:** Operaciones que crean nuevos objetos (ej. punto medio).

**Cinco paquetes de CGAL:**
1.  `2D Convex Hulls`: Envolvente convexa (detección de contornos).
2.  `2D Triangulations`: Triangulación de Delaunay (mallas FEM).
3.  `2D Voronoi Diagram Adaptor`: Diagrama de Voronoi (rutas).
4.  `Polygon Mesh Processing`: Operaciones booleanas en mallas 3D (CAD).
5.  `Point Set Processing`: Procesamiento de nubes de puntos (reconstrucción 3D).

---

### 2.5. CGAL en problemas reales de ingeniería

| Campo | Problema | Algoritmo | Paquete CGAL | Dificultad principal |
| :--- | :--- | :--- | :--- | :--- |
| **Robótica** | Colisiones. | Voronoi, Envolvente. | `Voronoi Adaptor` | Robustez en tangencias. |
| **CAD** | Sólidos. | Intersecciones. | `Polygon Mesh Processing` | Mallas con agujeros o auto-intersecciones. |
| **Civil** | Terrenos. | Delaunay 2.5D. | `2D Triangulations` | Grandes volúmenes de datos (LiDAR). |
| **Biomédica** | Anatomía. | Reconstrucción. | `Surface Mesh` | Superficies complejas (no manifold). |

---

## 2. Análisis Algorítmico y Numérico (Entregables 2 y 3)

*   **Esquema de la Envolvente Convexa:** 1) Ordenar puntos por coordenada X. 2) Construir la cadena inferior manteniendo solo giros a la izquierda. 3) Construir la cadena superior. Complejidad: \(O(n \log n)\) por la ordenación.
*   **Esquema de la Triangulación de Delaunay:** 1) Insertar puntos incrementalmente. 2) Realizar "flips" locales para restaurar el criterio del círculo vacío. Complejidad esperada: \(O(n \log n)\).
*   **Análisis Numérico:** El uso de `Exact_predicates_inexact_constructions_kernel` (EPICK) permite que las decisiones topológicas (predicados) sean exactas, evitando que los errores de punto flotante generen triangulaciones inválidas o intersecciones falsas, incluso si las coordenadas construidas tienen un pequeño margen de error.

---

## 3. Actividad Práctica: Nube de puntos, Envolvente y Delaunay

### 3.1. Implementación
Se desarrolló un programa en C++ utilizando la librería CGAL. En lugar de usar puntos aleatorios, el programa se adaptó para leer un archivo CSV con las coordenadas reales de 100 sensores. El programa calcula la envolvente convexa (`CGAL::convex_hull_2`) y la triangulación de Delaunay (`CGAL::Delaunay_triangulation_2`), y exporta los resultados.

### 3.2. Visualización
Se generó una gráfica (usando Python/Matplotlib) que muestra la nube de puntos (sensores), la frontera de la envolvente convexa y las aristas de Delaunay.

### 3.3. Resultados y análisis de complejidad

| Tamaño de la nube | Vértices en Envolvente | Aristas Delaunay | Caras finitas | Tiempo real |
| :--- | :--- | :--- | :--- | :--- |
| 100 (Sensores) | 12 | 285 | 186 | 0.005 s |
| 500 | 18 | 1479 | 980 | 0.017 s |
| 1 000 | 22 | 2975 | 1976 | 0.031 s |
| 5 000 | 23 | 14974 | 9975 | 0.110 s |

Se observa que el tiempo de ejecución se mantiene en milisegundos incluso para 5000 puntos. Al comparar los tiempos con el aumento de puntos, se confirma la eficiencia del algoritmo \(O(n \log n)\) de CGAL, el cual escala de manera muy favorable.

### 3.4. Respuestas a las preguntas de análisis (Sección 3.6)

**1. ¿Qué proporción de puntos pertenece a la frontera?**
La proporción es muy baja. Para 100 puntos, solo 12 (12%) están en la envolvente. A medida que aumentan los puntos (ej. 5000), la proporción cae al 0.46% (23 puntos), ya que es más probable que los nuevos puntos caigan en el interior.

**2. ¿Por qué la envolvente no contiene información de conectividad?**
Porque la envolvente es el contorno exterior. Al igual que una banda elástica estirada, ignora todo lo que está dentro. No nos dice qué sensores interiores están cerca entre sí.

**3. ¿Qué información adicional aporta la triangulación de Delaunay?**
Aporta la topología y conectividad de toda la red. Define cuáles sensores son "vecinos" directos, lo cual sirve para planificar rutas de comunicación o agrupar sensores.

**4. ¿Los resultados concuerdan con la relación de Euler?**
Sí. Considerando solo caras finitas, \(V - E + F = 1\). Por ejemplo, para 5000 puntos: \(5000 - 14974 + 9975 = 1\). Esto confirma la validez topológica de la malla.

**5. ¿Cómo cambia el tiempo de ejecución?**
Aumenta a medida que crece la nube, pero de forma eficiente. La Triangulación de Delaunay tiene complejidad \(O(n \log n)\). Factores como la carga del procesador pueden generar variaciones mínimas en la medición.

**6. ¿Qué ocurre con puntos repetidos, colineales o cocirculares?**
Se probó con 5 puntos en línea recta, incluyendo duplicados. CGAL demostró robustez: fusionó los duplicados (quedaron 4 vértices) y colapsó la envolvente a un segmento de 2 vértices. Al ser colineales, se generaron 0 caras finitas, demostrando que los predicados exactos evitan fallos del sistema.

**7. ¿Qué diferencias hay entre usar distintas semillas o la misma?**
Al usar la misma semilla, los puntos generados y los resultados son exactamente iguales (reproducibilidad). Al cambiar la semilla, la distribución espacial cambia, alterando la cantidad de vértices en la frontera, pero manteniendo la relación de Euler y la eficiencia algorítmica.

---

## 4. Cierre: Ideas Fundamentales

1.  **Los problemas geométricos son computacionales:** No basta con ecuaciones matemáticas; se necesitan algoritmos eficientes.
2.  **La robustez numérica es crucial:** Una respuesta rápida pero incorrecta (por errores de punto flotante) puede invalidar todo un sistema de ingeniería.
3.  **Uso de bibliotecas especializadas:** Herramientas como CGAL permiten construir soluciones de alto nivel. El ingeniero debe saber seleccionar, configurar y validar algoritmos existentes en lugar de reinventar la rueda.
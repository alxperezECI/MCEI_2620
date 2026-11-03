## PREGUNTAS DE INVESTIGACIÓN
 
### Pregunta 1. Geometría computacional y aplicaciones
**1. ¿Qué es la geometría computacional y en qué se diferencia de la geometría tradicional?**
Es el área que estudia algoritmos para representar y resolver problemas geométricos mediante computadores. La geometría euclidiana estudia propiedades y relaciones de las figuras; la analítica utiliza coordenadas y ecuaciones; la computacional incorpora estructuras de datos, eficiencia y robustez numérica.
 
**2. ¿Qué objetos geométricos se utilizan en dos y tres dimensiones?**
En 2D se emplean puntos, segmentos, rectas, polígonos y curvas. En 3D se utilizan puntos, planos, poliedros, superficies y mallas. También se procesan nubes de puntos obtenidas mediante sensores o escáneres.
 
**3. ¿Cómo se representan estos objetos computacionalmente?**
- Punto: coordenadas que determinan una posición.
- Segmento: dos extremos que delimitan una porción de recta.
- Polígono: secuencia de vértices y aristas que delimita una región.
- Poliedro: sólido delimitado por caras.
- Superficie: representación geométrica de una frontera, frecuentemente mediante una malla.
- Nube de puntos: conjunto de coordenadas que describe posiciones o muestras espaciales.
 
**4. ¿Cuáles son cinco problemas clásicos?**
Envolvente convexa, triangulación de Delaunay, diagramas de Voronoi, búsqueda del vecino más cercano y detección de intersecciones.
 
**5. ¿Dónde se aplican en ingeniería?**
Se utilizan en robótica, diseño CAD, sistemas geográficos, generación de mallas, visión artificial y análisis de distribución de sensores.
 
**Tres aplicaciones reales**
| Aplicación | Problema geométrico | Información de entrada |
| Robótica móvil | Evitar obstáculos y planificar trayectorias | Posición del robot y geometría de obstáculos |

| Sistemas CAD | Detectar intersecciones y modelar sólidos | Vértices, superficies y modelos tridimensionales |

| Sistemas geográficos | Delimitar regiones y analizar proximidad | Coordenadas geográficas y límites territoriales |
 
### Pregunta 2. Algoritmos y estructuras de datos
Se comparan cinco problemas clásicos, indicando su funcionamiento, algoritmo, complejidad y aplicación.
 
| Problema | Principio y algoritmo | Complejidad habitual | Aplicación |
| Convex Hull | Construye el menor contorno convexo; algoritmo de Andrew | O(n log n), n puntos | Delimitación espacial |

| Delaunay | Triángulos con círculos circunscritos vacíos; inserción incremental | O(n log n) esperado en condiciones habituales; peor caso O(n²) para ciertas estrategias | Generación de mallas |

| Voronoi | Divide el plano según el sitio más cercano; algoritmo de Fortune | O(n log n), n sitios puntuales | Zonas de influencia |

| Nearest Neighbor | Minimiza la distancia a un punto de consulta; árbol k-d | Construcción O(n log n); consulta O(log n) promedio en condiciones favorables, O(n) en peor caso | Localización de sensores próximos |

| Intersection Detection | Evalúa cruces entre segmentos; barrido de Bentley-Ottmann | O((n+k) log n) bajo supuestos habituales, n segmentos y k intersecciones | Detección de conflictos geométricos |
 
**Contextualización: 100 sensores**
El diagrama de Voronoi permitiría dividir la región en zonas donde cada ubicación está asociada al sensor más cercano. La triangulación de Delaunay permitiría identificar relaciones geométricas de vecindad entre sensores.
 
En este taller se implementó Delaunay con CGAL. Voronoi se propone como una extensión para analizar zonas de proximidad, sin asumir que representan cobertura física real.
 
**Esquema de dos algoritmos**
*Envolvente convexa mediante Andrew:*
1. Ordenar los puntos por coordenadas.
2. Construir la cadena inferior mediante pruebas de orientación.
3. Construir la cadena superior.
4. Unir ambas cadenas para obtener el contorno convexo.
La ordenación determina una complejidad O(n log n).
 
*Triangulación de Delaunay mediante inserción incremental:*
1. Inicializar la estructura de triangulación.
2. Insertar los puntos.
3. Localizar la región afectada por cada inserción.
4. Actualizar las conexiones para mantener el criterio del círculo vacío.
5. Obtener vértices, aristas y caras.
La eficiencia depende de la estrategia de localización, el orden de inserción y la distribución de los puntos.
 
### Pregunta 3. Precisión numérica y robustez
**1. ¿Qué problemas introduce IEEE-754?**
Los números de punto flotante representan valores con precisión finita. Esto introduce redondeos, pérdida de precisión y cancelación numérica en operaciones con cantidades muy próximas.
 
**2. ¿Qué significa que tres puntos sean colineales?**
Significa que pertenecen a una misma recta. Computacionalmente, determinarlo puede ser difícil cuando el resultado de la prueba de orientación es muy cercano a cero.
 
**3. ¿Qué son las configuraciones degeneradas?**
Son situaciones geométricas especiales, como puntos repetidos, puntos colineales o cuatro puntos cocirculares, que pueden requerir un tratamiento particular.
 
**4. ¿Cómo afectan los errores de redondeo?**
Pueden clasificar incorrectamente la orientación de puntos, detectar intersecciones inexistentes o generar decisiones inconsistentes durante una triangulación.
 
**5. Diferencia entre exactitud, precisión y robustez**
- Exactitud geométrica: corrección matemática del resultado respecto a los datos representados.
- Precisión numérica: capacidad para representar y calcular valores con suficiente detalle.
- Robustez algorítmica: capacidad del algoritmo para producir resultados consistentes, incluso en configuraciones difíciles.
 
**Experimento conceptual**
Se consideran los puntos A=(0,0), B=(1,1) y C=(2,2+ε).
Si ε es distinto de cero, los puntos no son matemáticamente colineales. Sin embargo, cuando ε es demasiado pequeño para distinguir 2+ε de 2 en la representación utilizada, el computador puede almacenar las mismas coordenadas y clasificarlos como colineales.
 
CGAL utiliza predicados geométricos exactos para evitar errores de clasificación respecto a los valores representados. No puede recuperar información que ya se perdió al convertir las coordenadas de entrada.
 
**Predicados y construcciones**
Un predicado responde una pregunta geométrica, como la orientación de tres puntos. Una construcción genera un objeto geométrico, como un punto de intersección.
El kernel EPICK ofrece predicados exactos y construcciones inexactas. El kernel EPECK ofrece predicados y construcciones exactos, con un posible costo computacional mayor.
 
### Pregunta 4. Arquitectura y ventajas de CGAL
**1. ¿Qué significa CGAL?**
Computational Geometry Algorithms Library. Es una biblioteca de algoritmos y estructuras de datos para geometría computacional.
 
**2. ¿Por qué utiliza C++?**
Porque permite implementar estructuras geométricas eficientes, programación genérica y control sobre los tipos numéricos.
 
**3. ¿Qué es un Geometry Kernel?**
Es el conjunto de tipos geométricos, operaciones, predicados y construcciones que utilizan los algoritmos.
 
**4. ¿Qué son predicates y constructions?**
Los predicados determinan relaciones geométricas; las construcciones generan nuevos objetos geométricos.
 
**5. ¿En qué se diferencian EPICK y EPECK?**
EPICK combina predicados exactos con construcciones inexactas. EPECK proporciona exactitud tanto en predicados como en construcciones.
 
**6. ¿Qué ventajas y limitaciones tiene CGAL?**
Ventajas: algoritmos probados, estructuras especializadas, robustez numérica y documentación.
Limitaciones: curva de aprendizaje, dependencias de compilación, complejidad de las plantillas de C++ y posibles costos de rendimiento al utilizar operaciones exactas.
 
**Cinco paquetes de CGAL**
| Paquete | Funcionalidad y datos | Aplicación |
| 2D Convex Hulls | Envolventes de conjuntos de puntos 2D | Límites espaciales |
| 2D Triangulations | Vértices, aristas y caras de triangulaciones | Mallas y vecindad |
| 2D Voronoi Diagram Adaptor | Diagramas de proximidad a partir de estructuras duales | Regiones de influencia |
| 2D Arrangements | Curvas, intersecciones y subdivisiones del plano | Diseño geométrico |
| Polygon Mesh Processing | Operaciones y análisis de mallas superficiales | Modelado CAD y superficies 3D |
 
### Pregunta 5. Aplicaciones reales de ingeniería
Se analizan cuatro campos, considerando el problema, las entradas, el algoritmo, el paquete de CGAL y las dificultades.

| Campo | Problema y datos de entrada | Algoritmo y paquete CGAL | Dificultades |
| Robótica | Planificar movimientos con mapas y obstáculos | Intersecciones y `2D Arrangements` | Obstáculos complejos y precisión |

| Agricultura de precisión | Reconstruir terrenos desde coordenadas 3D | Reconstrucción superficial y `Poisson Surface Reconstruction` | Ruido, vacíos y densidad de muestras |

| Diseño mecánico | Comprobar intersecciones y combinar sólidos CAD | Operaciones booleanas y `Polygon Mesh Processing` | Mallas defectuosas y degeneraciones |

| Ingeniería civil | Generar mallas para representar terrenos | Triangulación y `2D Triangulations` | Puntos irregulares y cambios abruptos del terreno |
 
**Reto de integración: análisis geométrico de 100 sensores** 
Se propone analizar las posiciones de sensores distribuidos en una región mediante CGAL.
- Entradas: archivo CSV con identificadores y coordenadas X, Y en metros.
- Algoritmos: envolvente convexa y triangulación de Delaunay.
- Resultados: contorno exterior, relaciones geométricas de vecindad, número de vértices, aristas y caras.
- Métricas: tiempo de ejecución, número de elementos geométricos y verificación de consistencia topológica.
 
**Resultados obtenidos**
Se procesaron 100 sensores y se obtuvieron 12 vértices convexos, 285 aristas y 186 caras finitas.
La aplicación demuestra el procesamiento de datos espaciales con CGAL. Una evaluación real de cobertura requeriría información adicional sobre alcance, obstáculos y características de cada sensor.


## ANÁLISIS DE RESULTADOS DE LA CTIVIDAD PRÁCTICA 
### 1. ¿Qué proporción de puntos pertenece a la envolvente convexa?
| Puntos | Semilla 42 | Semilla 123 | Semilla 2026 |
| 100 | 11,00 % | 10,00 % | 13,00 % |
| 500 | 3,40 % | 3,20 % | 3,20 % |
| 1.000 | 1,70 % | 1,80 % | 1,50 % |
| 5.000 | 0,40 % | 0,40 % | 0,32 % |
La proporción disminuye al aumentar la cantidad de puntos, porque una mayor parte de ellos queda en el interior del contorno convexo.
 
### 2. ¿Por qué la envolvente convexa no representa la conectividad interior?
Porque solamente identifica el contorno convexo exterior. No establece conexiones entre los puntos interiores ni representa su distribución detallada.
 
### 3. ¿Qué información adicional aporta Delaunay?
Proporciona una estructura de triángulos y aristas que permite identificar relaciones de vecindad, analizar conectividad geométrica y construir mallas.
 
### 4. ¿Cuántos vértices, aristas y caras aparecen? ¿Se cumple Euler?
Las cantidades de las 12 ejecuciones están registradas en la tabla de resultados experimentales.
Para todos los casos se verificó:
- E = 3V - H - 3
- F = 2V - H - 2
Donde V corresponde a los vértices distintos, H a los vértices de la envolvente convexa, E a las aristas y F a las caras finitas.
Los resultados cumplen estas relaciones para las configuraciones bidimensionales analizadas.
 
### 5. ¿Cómo cambia el tiempo de ejecución?
Los tiempos aumentan generalmente con el número de puntos. Delaunay presentó un costo mayor que la envolvente convexa. Los promedios de las tres semillas fueron:
| Puntos | Hull (ms) | Delaunay (ms) |
| 100 | 0,0528 | 0,2821 |
| 500 | 0,1911 | 1,2372 |
| 1.000 | 0,3079 | 7,7447 |
| 5.000 | 1,4724 | 15,6166 |
 
Las mediciones pueden variar por carga del procesador, administración de memoria, distribución de puntos, caché y condiciones del sistema.
 
### 6. ¿Qué ocurre con puntos repetidos, colineales y cocirculares?
- Repetidos: Delaunay no genera vértices adicionales para coordenadas idénticas.
- Colineales: la triangulación tiene dimensión uno y no contiene caras triangulares finitas.
- Cocirculares: pueden existir distintas triangulaciones de Delaunay válidas, porque el criterio del círculo vacío no determina necesariamente una única diagonal.
 
Estos casos se estudian mediante el programa independiente `degenerados.cpp`.
 
### 7. ¿Qué diferencias existen entre semillas distintas y una misma semilla?
Cambiar la semilla modifica las coordenadas generadas y, por tanto, la cantidad de vértices convexos y la estructura de Delaunay. 
Repetir una semilla con el mismo generador y entorno reproduce el conjunto de puntos y sus resultados geométricos, aunque los tiempos pueden variar entre ejecuciones.

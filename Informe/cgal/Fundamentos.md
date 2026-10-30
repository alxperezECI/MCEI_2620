# Fundamentos de geometría computacional

Autores: Jimmy Alexander Calderón Trujillo y Sebastián Barahona.
Curso: Métodos Computacionales en Ingeniería.
Docente: Alexander Pérez Ruiz.

## 1. ¿Qué es la geometría computacional?

La geometría computacional estudia algoritmos y estructuras de datos
para resolver problemas sobre objetos geométricos: puntos, segmentos,
polígonos, superficies y sólidos. Además del resultado matemático,
analiza el costo computacional y la confiabilidad de las decisiones
tomadas con representaciones numéricas finitas.

La geometría analítica proporciona ecuaciones y relaciones entre
objetos. La geometría computacional organiza esas operaciones en
procedimientos que pueden procesar grandes conjuntos de datos.
Los gráficos por computador se concentran en generar imágenes;
la geometría computacional puede producir estructuras y relaciones
espaciales aunque no exista una visualización.

Sus aplicaciones incluyen detección de colisiones en robótica,
modelado CAD, generación de mallas para simulaciones, análisis de
terrenos y procesamiento de nubes de puntos.

En ingeniería eléctrica puede apoyar el estudio espacial de sensores,
la identificación de proximidades entre equipos y la evaluación
geométrica de alternativas de trazado. La proximidad no determina
por sí sola la viabilidad eléctrica, constructiva o de comunicaciones.

## 2. Algoritmos y estructuras fundamentales

Las complejidades siguientes corresponden a algoritmos concretos,
no a todos los procedimientos posibles para resolver cada problema.
Se supone dimensión fija y operaciones geométricas de costo unitario.
Con aritmética exacta, el tamaño de los números también afecta el costo.

### 2.1. Envolvente convexa

Problema: encontrar el menor conjunto convexo que contiene los puntos.
En el plano, la salida puede ser un polígono, un segmento o un punto.

Principio: un conjunto es convexo si contiene el segmento que une
cualquier pareja de sus puntos. Los predicados de orientación permiten
identificar qué puntos deben permanecer en el contorno.

Algoritmo de ejemplo: cadena monótona de Andrew.
Estructuras: lista ordenada de puntos y pilas para las cadenas.

Complejidad: O(n log n) para n puntos sin ordenar y O(n) de memoria.
La ordenación domina; cada punto entra y sale de cada pila a lo sumo
una vez durante los recorridos.

Aplicación: obtener una primera delimitación convexa de una red de
sensores. No representa necesariamente una frontera física ni
describe huecos o concavidades de la región.

### 2.2. Triangulación de Delaunay

Problema: conectar puntos del plano mediante triángulos sin cruces
de aristas, cubriendo su envolvente convexa.

Principio: el interior del círculo circunscrito de cada triángulo
no contiene otros puntos de entrada. En presencia de puntos
cocirculares pueden existir varias triangulaciones válidas.

Algoritmo de ejemplo: inserción incremental con localización del
punto y legalización de aristas mediante el predicado de círculo.
Estructura: vértices y caras con referencias a sus vecinos.

Complejidad: existen construcciones planas O(n log n). Una variante
incremental aleatorizada con localización adecuada alcanza ese
tiempo esperado; una implementación incremental sencilla puede
requerir O(n²). No se atribuye automáticamente la primera cota
a cualquier llamada o implementación de CGAL.

Aplicación: mallas iniciales de terrenos y relaciones de proximidad
entre sensores. Una triangulación sin restricciones no representa
por sí sola obstáculos, huecos o límites no convexos.

### 2.3. Diagramas de Voronoi

Problema: dividir el plano en regiones asignadas al sitio más cercano.

Principio: la celda de un sitio p contiene los puntos cuya distancia
a p no supera su distancia a los demás sitios. Con distancia
euclidiana, las fronteras se encuentran sobre mediatrices.

Algoritmo de ejemplo: barrido de Fortune.
Estructuras: cola de eventos, árbol de la línea de playa y
representación de aristas y celdas.

Complejidad: O(n log n) de tiempo y O(n) de espacio para n sitios
puntuales del plano, con manejo apropiado de degeneraciones.

Aplicación: asignar posiciones de una región al sensor más cercano.
Las celdas exteriores pueden ser no acotadas; para estudiar un
predio se deben recortar con su dominio.

### 2.4. Búsqueda de vecinos cercanos

Problema: encontrar el punto, o los k puntos, más próximos a una
consulta según una métrica especificada.

Principio: comparar distancias y descartar regiones cuya distancia
mínima posible ya supera la mejor solución encontrada.

Algoritmo de ejemplo: búsqueda con poda en un kd-tree.
Estructura: árbol que divide el espacio por coordenadas.

Complejidad: un kd-tree equilibrado puede construirse en O(n log n)
para dimensión fija. La consulta suele ser eficiente en baja
dimensión, pero una búsqueda exacta puede visitar O(n) puntos en
el peor caso. La búsqueda exhaustiva del vecino más cercano
requiere O(n) evaluaciones de distancia.

Aplicación: localizar el sensor espacialmente más cercano a un
equipo. Debe distinguirse la distancia física de una distancia
entre variables como voltaje y corriente.

### 2.5. Detección de intersecciones

Problema: determinar qué segmentos u objetos se cruzan o se tocan.

Principio: utilizar orientaciones y pruebas de pertenencia;
para muchos segmentos se puede barrer el plano procesando eventos.

Algoritmo de ejemplo: Bentley-Ottmann.
Estructuras: cola de eventos y árbol ordenado de segmentos activos.

Complejidad: O((n+k) log n) para reportar k intersecciones entre
n segmentos bajo los supuestos del algoritmo clásico, como ausencia
de solapamientos y tratamiento consistente de eventos coincidentes.
Los casos degenerados requieren reglas adicionales.

Aplicación: detectar cruces de trazados propuestos. Un cruce en
proyección 2D no demuestra una colisión 3D: deben verificarse
elevaciones, dimensiones y distancias de separación.

## Contextualización: 100 sensores

Las coordenadas x_m e y_m permiten estudiar proximidad física.
Voronoi asignaría cada posición de la región al sensor más cercano.
Delaunay proporciona conexiones geométricas entre sitios; en
posición general es el dual del diagrama de Voronoi.

Si hay cocircularidad, una triangulación puede añadir diagonales
cuya correspondencia dual es degenerada, por lo que la relación
entre vecinos debe interpretarse con cuidado.

En el archivo suministrado por el profesor se obtuvieron 100
vértices distintos, 12 vértices extremos, 285 aristas finitas y
186 caras finitas. La envolvente tiene el 12 % de los sensores
como vértices y se cumple 100 - 285 + 186 = 1.

Estas conexiones no prueban que exista cableado, cobertura de radio
o comunicación efectiva. Para ello harían falta restricciones
físicas y técnicas adicionales.

La práctica realizada calcula la envolvente y Delaunay.
La construcción y el recorte de Voronoi se describen aquí como
aplicación conceptual; no se presentan como una ejecución realizada.

## Referencias de esta sección

- CGAL, 2D Convex Hulls:
  https://doc.cgal.org/latest/Convex_hull_2/index.html
- CGAL, 2D Triangulations:
  https://doc.cgal.org/latest/Triangulation_2/index.html
- de Berg, Cheong, van Kreveld y Overmars.
  Computational Geometry: Algorithms and Applications, tercera
  edición, Springer, 2008. Referencia bibliográfica para los
  algoritmos clásicos y su análisis.

## 3. Precisión numérica y robustez

### Representación IEEE-754

Los números de punto flotante representan un conjunto finito de
valores. Muchos números decimales no tienen una representación
binaria exacta y las operaciones pueden introducir redondeo.
También existen límites de rango, desbordamiento y subdesbordamiento.

La resta de valores próximos puede amplificar el error relativo.
En geometría, un error pequeño puede cambiar el signo de un
determinante y alterar una decisión discreta, como la orientación
de tres puntos o la existencia de una intersección.

### Colinealidad y configuraciones degeneradas

Tres puntos son colineales si pertenecen a una misma recta.
En el plano, el determinante de orientación es cero para esos puntos.
Cuando el determinante es muy pequeño, una evaluación aproximada
puede confundir una configuración casi colineal con una colineal.

Son ejemplos de configuraciones degeneradas los puntos repetidos,
los puntos colineales, cuatro puntos cocirculares y los segmentos
solapados. No son necesariamente entradas incorrectas, pero requieren
tratamientos que los algoritmos deben definir explícitamente.

Una orientación incorrecta puede producir un contorno inconsistente.
Una prueba de intersección incorrecta puede omitir un cruce.
Una prueba de círculo incorrecta puede alterar la conectividad de
una triangulación de Delaunay.

### Exactitud, precisión y robustez

- Exactitud geométrica: correspondencia del resultado con la geometría
  matemática de los datos de entrada.
- Precisión numérica: resolución y cantidad de información que permite
  conservar la representación numérica utilizada.
- Robustez algorítmica: capacidad de producir resultados consistentes
  ante entradas difíciles y configuraciones degeneradas.

Un cálculo exacto respecto de sus entradas no implica que las
coordenadas medidas representen exactamente la realidad física.

### Predicados y construcciones

Un predicado responde una pregunta geométrica: orientación,
pertenencia o comparación de posiciones. Una construcción produce
un objeto geométrico nuevo, como una intersección o un circuncentro.

CGAL permite combinar filtros numéricos con evaluación exacta cuando
es necesaria para decidir correctamente. La garantía depende del
kernel y de las operaciones seleccionadas.

En nuestro experimento, para epsilon=10^-16, 10^-17 y 10^-20,
la suma double 2+epsilon se redondeó a 2. Tanto EPICK como EPECK
alimentado con ese double clasificaron los puntos como colineales.
EPECK con epsilon construido racionalmente mantuvo el giro izquierdo.

Esto demuestra pérdida de información al preparar la entrada,
no un fallo de los predicados exactos.

El procedimiento y los resultados completos están documentados en
[Análisis numérico](Analisis_numerico.md).

## 4. CGAL: organización, ventajas y limitaciones

CGAL significa Computational Geometry Algorithms Library.
Ofrece algoritmos y estructuras de datos para resolver problemas
geométricos de manera eficiente y con garantías numéricas definidas.

Su implementación principal en C++ permite combinar rendimiento,
gestión de estructuras complejas y programación genérica mediante
plantillas. Los algoritmos pueden trabajar con distintos tipos
geométricos y representaciones numéricas.

### Organización

- Kernel: define objetos básicos, como puntos y segmentos, además
  de predicados y construcciones.
- Paquetes: agrupan algoritmos para problemas concretos.
- Estructuras de datos: almacenan relaciones entre vértices,
  aristas, caras y otros objetos.
- Conceptos y traits: especifican las operaciones y tipos que
  necesitan los algoritmos, facilitando su adaptación.

### Kernels utilizados

| Kernel | Predicados | Construcciones | Uso en la práctica |
|---|---|---|---|
| Exact_predicates_inexact_constructions_kernel (EPICK) | Exactos para las operaciones cubiertas por el kernel | En general inexactas | Envolvente y Delaunay sobre coordenadas double |
| Exact_predicates_exact_constructions_kernel (EPECK) | Exactos | Exactas para las operaciones soportadas | Experimento con desplazamientos racionales pequeños |

Los kernels no recuperan información perdida antes de recibir
la entrada. EPECK tampoco representa automáticamente cualquier
operación matemática imaginable: deben respetarse sus tipos y
operaciones disponibles.

### Ventajas y limitaciones

CGAL proporciona implementaciones especializadas, manejo de
degeneraciones, documentación y componentes reutilizables.
Reduce el esfuerzo necesario para desarrollar y validar desde cero.

Sus limitaciones incluyen la curva de aprendizaje de C++ y plantillas,
el tiempo de compilación, el consumo de memoria y el costo de ciertas
operaciones exactas. También deben revisarse las precondiciones,
las dependencias y la licencia del paquete utilizado.

Implementar un algoritmo propio resulta útil para aprender o atender
una necesidad específica, pero obliga a comprobar degeneraciones,
estabilidad numérica, rendimiento y consistencia de las estructuras.

### Cinco paquetes de interés

| Paquete | Problema y herramientas | Aplicación |
|---|---|---|
| 2D Convex Hulls | Calcula puntos extremos con funciones como convex_hull_2 y ch_graham_andrew | Delimitación convexa de sensores |
| 2D Triangulations | Representa triangulaciones; incluye Delaunay_triangulation_2 | Conectividad geométrica y modelos de terreno |
| Spatial Searching | Ofrece kd-trees y búsquedas de vecinos, como Kd_tree y Orthogonal_k_neighbor_search | Encontrar equipos o mediciones cercanas |
| Polygon Mesh Processing | Procesa mallas poligonales; ofrece operaciones de reparación, remallado y operaciones booleanas bajo sus precondiciones | Preparación de modelos CAD y anatómicos |
| 3D Mesh Generation | Genera mallas tetraédricas de dominios tridimensionales mediante criterios de calidad | Discretización para simulaciones físicas |

Solo los dos primeros paquetes se emplean en la implementación
principal del taller. Los demás se presentan como exploración
tecnológica, no como experimentos ejecutados.

## 5. Aplicaciones en ingeniería

### Robótica

Problema: comprobar si un movimiento produce una colisión.
Entradas: geometría del robot, obstáculos y posiciones.
Algoritmos: consultas de intersección aceleradas por jerarquías
de volúmenes envolventes.
Paquete: AABB Tree, con primitivas como triángulos.

Dificultades: modelos incompletos, contactos casi tangentes y
actualización de objetos móviles. Verificar posiciones aisladas
no garantiza ausencia de colisiones durante todo el movimiento.
El árbol apoya las consultas, pero no constituye por sí solo
un planificador de trayectorias.

### Diseño mecánico y CAD

Problema: unir, intersectar o sustraer piezas.
Entradas: mallas de las superficies de los sólidos.
Algoritmos: intersección de superficies, subdivisión común y
clasificación de las regiones resultantes.
Paquete: Polygon Mesh Processing, mediante corefinement y
operaciones booleanas.

Dificultades: orientación de caras, autointersecciones, superficies
abiertas y contactos degenerados. Las entradas deben cumplir
las precondiciones de la operación seleccionada.

### Ingeniería civil

Problema: construir un modelo geométrico de un terreno.
Entradas: posiciones planimétricas y elevaciones de un levantamiento.
Algoritmo: triangulación de Delaunay en planta, asociando las
elevaciones a los vértices.
Paquete: 2D Triangulations.

Dificultades: ruido, posiciones duplicadas, discontinuidades y
límites del dominio. Si se deben respetar líneas de quiebre,
se requiere una triangulación con restricciones.
Un modelo de altura sobre el plano no representa bien voladizos
o superficies con varias alturas en la misma posición.

### Ingeniería biomédica

Problema: preparar una geometría anatómica para simulación.
Entradas: superficies segmentadas de imágenes médicas.
Algoritmos: reparación de mallas superficiales y mallado tetraédrico.
Paquetes: Polygon Mesh Processing y 3D Mesh Generation.

Dificultades: errores de segmentación, conservación de detalles,
calidad de los elementos y costo de procesamiento.
La validez geométrica no reemplaza la validación anatómica ni
la definición del modelo físico y sus condiciones de frontera.

### Reto de integración: distribución espacial de sensores

Se propone estudiar la distribución de sensores de una instalación
eléctrica y localizar zonas alejadas de los dispositivos existentes.

Entradas:
- Identificador y coordenadas métricas de cada sensor.
- Polígono de la región que se desea estudiar.
- Obstáculos y distancia máxima admisible, si están disponibles.

Procedimiento propuesto:
1. Validar coordenadas, identificadores y posiciones repetidas.
2. Calcular la envolvente para describir la extensión convexa.
3. Construir Delaunay para obtener relaciones geométricas de proximidad.
4. Construir Voronoi y recortarlo con el dominio de estudio.
5. Evaluar distancias al sensor más cercano y señalar zonas que
   superen el umbral definido para la aplicación.

Resultados esperados:
- Mapa de sensores, envolvente y conexiones geométricas.
- Regiones asignadas a cada sensor por proximidad.
- Candidatos a nuevas ubicaciones para reducir grandes distancias.

Métricas:
- Tiempo de cálculo y memoria requerida.
- Número de vértices, aristas y caras.
- Comprobaciones de validez y relaciones de Euler.
- Distancia máxima al sensor más cercano dentro del dominio.
- Porcentaje del área dentro del umbral establecido.

La distancia máxima y el porcentaje de área requieren cálculos
adicionales. Una evaluación sobre una cuadrícula sería aproximada
y debería informar su resolución y comprobar convergencia.

Alcance realizado: se procesaron las 100 ubicaciones del profesor,
se calcularon envolvente y Delaunay, se exportaron los resultados
y se generó su visualización.

Alcance propuesto: Voronoi recortado y evaluación de cobertura.
No se presentan como resultados ya obtenidos.

La cobertura definida aquí es geométrica. No demuestra cobertura
de radio, capacidad de detectar una variable física ni viabilidad
de una red eléctrica; esas conclusiones requieren modelos adicionales.

## Documentación complementaria

- Kernels:
  https://doc.cgal.org/latest/Kernel_23/index.html
- Catálogo de paquetes:
  https://doc.cgal.org/latest/Manual/packages.html
- Búsqueda espacial:
  https://doc.cgal.org/latest/Spatial_searching/index.html
- Procesamiento de mallas:
  https://doc.cgal.org/latest/Polygon_mesh_processing/index.html
- Mallado tridimensional:
  https://doc.cgal.org/latest/Mesh_3/index.html
- Árbol AABB:
  https://doc.cgal.org/latest/AABB_tree/index.html

La práctica se ejecutó con CGAL 5.6. Los enlaces latest pueden
mostrar una versión posterior; para reproducirla deben comprobarse
las interfaces de la versión instalada.

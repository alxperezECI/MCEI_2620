# Análisis de dos algoritmos

Autores: Jimmy Alexander Calderón Trujillo y Sebastián Barahona.

Estos esquemas explican algoritmos clásicos. No son una transcripción
del código interno de las funciones de CGAL utilizadas.

## 1. Envolvente convexa: cadena monótona de Andrew

Entrada: n puntos del plano.
Salida: vértices extremos de la envolvente en orden cíclico.

### Esquema

1. Ordenar los puntos por coordenada x y, en caso de empate, por y.
2. Eliminar posiciones duplicadas.
3. Recorrer los puntos ordenados para construir la cadena inferior.
4. Mientras los dos últimos puntos de la cadena y el nuevo punto
   no formen un giro a la izquierda, retirar el último de la cadena.
5. Añadir el nuevo punto.
6. Repetir el procedimiento en orden inverso para la cadena superior.
7. Unir ambas cadenas sin repetir sus extremos.

El criterio de retirar también los giros colineales conserva solo
los extremos de cada segmento de frontera. Si todos los puntos
son colineales, la salida contiene los dos extremos; si solo existe
una posición distinta, la salida contiene ese punto.

### Fundamento

El signo del determinante de orientación permite decidir si el
contorno mantiene la convexidad. Los puntos retirados no son
necesarios como vértices extremos de la cadena.

### Complejidad

La ordenación requiere O(n log n). Cada punto se añade y se retira
como máximo una vez en cada recorrido, por lo que la construcción
de las cadenas requiere O(n). El espacio adicional es O(n).

Se supone que las comparaciones y los predicados tienen costo unitario.
La aritmética exacta puede añadir costos dependientes del tamaño
de los números.

### Relación con nuestra implementación

El programa utiliza CGAL::convex_hull_2. CGAL ofrece distintos
algoritmos de envolvente y la selección de esa función depende
de la categoría de iteradores. Por ello no identificamos las
mediciones realizadas como tiempos de una implementación propia
de Andrew.

## 2. Delaunay: inserción incremental y legalización

Entrada: puntos distintos del plano.
Salida: triangulación con círculos circunscritos de interior vacío.

### Esquema conceptual para el caso bidimensional

1. Inicializar una triangulación auxiliar que contenga la entrada.
2. Insertar un punto y localizar el triángulo o la arista que lo contiene.
3. Dividir la región afectada para incorporar el nuevo vértice.
4. Examinar las aristas cuya condición de Delaunay pudo alterarse.
5. Para dos triángulos vecinos que formen un cuadrilátero convexo,
   evaluar si el vértice opuesto queda dentro del circuncírculo.
6. Si la arista es ilegal, intercambiar la diagonal y continuar
   revisando las aristas afectadas.
7. Repetir para todos los puntos y retirar los elementos auxiliares.

Una implementación debe manejar explícitamente puntos sobre aristas,
duplicados, colinealidad y cocircularidad. En un empate cocircular,
más de una diagonal puede ser válida y se necesita una política
consistente.

### Fundamento

Los cambios locales restablecen el criterio del círculo vacío.
Los predicados de orientación y de posición respecto de un círculo
son esenciales para mantener una estructura consistente.

### Complejidad

Una versión incremental aleatorizada con una estructura adecuada
de localización puede alcanzar O(n log n) de tiempo esperado.
Una versión sencilla con búsquedas lineales puede requerir O(n²).
En el plano, la triangulación ocupa O(n) espacio.

Estas cotas no se deducen de nuestras cuatro cantidades de puntos
ni constituyen una garantía automática de cada llamada de CGAL.

### Relación con nuestra implementación

El programa utiliza Delaunay_triangulation_2 y la inserción de un
rango de puntos. La biblioteca gestiona las relaciones entre caras,
vértices y el exterior mediante su propia representación.

El esquema anterior es una explicación del método incremental;
no implica que CGAL utilice literalmente un supertriángulo ni
el mismo orden de inserción descrito.

## Validación mediante Euler

Para una triangulación plana conectada de un dominio convexo,
de dimensión 2 y sin huecos:

    V - E + F = 1

Aquí F cuenta solo las caras finitas. Al incluir la cara exterior,
la relación pasa a V - E + (F + 1) = 2.

Si b es el número de vértices de la frontera, contando también
posibles puntos colineales sobre sus lados:

    3F = 2E - b
    E = 3V - b - 3
    F = 2V - b - 2

No siempre b coincide con el número de vértices extremos que
devuelve convex_hull_2: puede haber puntos intermedios colineales
en la frontera.

Para los sensores del profesor, los conteos obtenidos son
compatibles con b=12:

    E = 3*100 - 12 - 3 = 285
    F = 2*100 - 12 - 2 = 186

Las fórmulas de triangulación bidimensional no se aplican al caso
completamente colineal. En ese caso obtuvimos una cadena con
V=5, E=4 y F=0, que también cumple V-E+F=1.

Euler es una comprobación necesaria de los conteos, pero no
demuestra por sí sola la propiedad de Delaunay. También se
ejecutó dt.is_valid().

## Referencias

- https://doc.cgal.org/latest/Convex_hull_2/index.html
- https://doc.cgal.org/latest/Triangulation_2/index.html

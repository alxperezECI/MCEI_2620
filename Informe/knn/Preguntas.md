# Taller de vecinos cercanos: preguntas exploratorias

Autor: Jimmy Alexander Calderón Trujillo
Entrega individual

## P1. Diferencia conceptual entre KNN y kd-tree

KNN busca los k puntos más próximos a una consulta según una métrica.
Puede utilizarse como parte de métodos de clasificación o regresión.
En este taller se realiza búsqueda de vecinos, sin etiquetas de clase.

Un kd-tree es una estructura que divide el espacio mediante cortes
por coordenadas y permite descartar regiones durante la búsqueda.
No son conceptos excluyentes: un kd-tree puede implementar una
búsqueda KNN. Aquí se compara búsqueda exhaustiva con búsqueda exacta
mediante kd-tree.

Encontrar vecinos no constituye por sí solo un método de agrupación
automática de todos los sensores.

## P2. Escenarios de mayor eficiencia

La búsqueda exhaustiva es sencilla, no requiere construir un índice
espacial y puede ser conveniente con pocos datos, pocas consultas
o dimensiones elevadas.

El kd-tree puede resultar conveniente con muchos puntos de baja
dimensión y consultas repetidas sobre un conjunto relativamente
estable. Su construcción y actualización también tienen costo.

En esta práctica hay 99 candidatos en tres dimensiones. La mediana
de consulta exhaustiva fue 18.488 microsegundos, frente a 38.123
microsegundos del árbol ya construido. Por tanto, la búsqueda
exhaustiva resultó más rápida en el experimento realizado.

## P3. Dimensionalidad y tamaño

Calcular todas las distancias exige un trabajo proporcional al
número de candidatos multiplicado por el número de variables.
Nuestra implementación además ordena todas las distancias.

El kd-tree puede reducir los puntos examinados al descartar regiones,
pero su beneficio depende de la distribución de los datos.
En dimensiones elevadas suele perder eficacia porque necesita
explorar más regiones. No garantiza consultas logarítmicas en todos
los casos.

Al crecer el conjunto también aumentan los requisitos de memoria
y el costo de construir o reconstruir el índice.

## P4. Elección de la métrica

La distancia euclidiana es la raíz de la suma de diferencias
cuadradas. Manhattan suma los valores absolutos de las diferencias.
Cada una define de manera distinta qué significa estar cerca.

Antes de medir distancias se estandarizaron las variables usando
la media y desviación estándar de las 99 muestras de referencia.
La consulta se transformó con esos mismos parámetros.

Para la muestra 100:
- Euclidiana: 47, 80, 20, 82, 3.
- Manhattan: 20, 47, 3, 80, 82.

El conjunto coincide, pero cambia su orden y el vecino más próximo.
Este resultado corresponde a esta consulta y no implica que las
dos métricas siempre seleccionen el mismo conjunto.

## P5. Limitaciones en sistemas eléctricos de gran escala

Deben considerarse ruido, datos faltantes, errores de unidades,
escalas diferentes, cambios de operación y actualización del índice.
También importan la memoria disponible y la latencia de adquisición,
procesamiento y comunicación.

Las variables correlacionadas pueden dar peso repetido a un mismo
fenómeno. En este trabajo la potencia se interpretó utilizando
voltaje y corriente, por lo que no representa información independiente.

Una muestra estadísticamente atípica no demuestra una falla eléctrica.
Se necesitan límites operativos, contexto y validación.
La cercanía entre mediciones tampoco implica cercanía geográfica.

Los tiempos medios o medianos de consultas aisladas no garantizan
el cumplimiento de un plazo máximo en un sistema de tiempo real.

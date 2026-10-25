# Taller KNN y kd-tree

Autor: Jimmy Alexander Calderón Trujillo
Curso: Métodos Computacionales en Ingeniería
Entrega individual

## Alcance y supuestos

Se utilizaron las 100 muestras de Datos_MCEI.csv.

El archivo recibido contiene voltaje, corriente y potencia, pero no
temperatura. Por ello se adapta el ejercicio a esas tres variables.
Esta adaptación difiere de las variables indicadas en el enunciado.

Los valores recibidos parecen carecer de separadores decimales.
Se conservaron intactos en datos/knn/Datos_MCEI_original.csv.
La interpretación es una hipótesis de trabajo autorizada por el
estudiante, no una corrección confirmada por el profesor.

Para cada valor se consideran divisores 10^n, con n entre 0 y 4.
Se elige el candidato con menor distancia logarítmica a:
- 120 V para voltaje.
- 5 A para corriente.
- V*I para potencia, utilizando los valores ya interpretados.

La conversión se registra por muestra y variable en
datos/knn/registro_conversion.csv.

Esta regla no garantiza recuperar los valores originales.
Las conclusiones numéricas quedan condicionadas a estos supuestos.
La potencia está relacionada con voltaje y corriente, por lo que
incluirla puede dar peso repetido a información relacionada.

## Selección de la consulta

Se seleccionó la muestra con mayor desviación absoluta respecto
a la mediana del voltaje: muestra 100.

Valores interpretados:
- Voltaje: 151.5 V.
- Corriente: 10.8 A.
- Potencia aproximada: 1636.2 W.

Los límites estadísticos Q1 - 1.5*IQR y Q3 + 1.5*IQR fueron:
115.85875 V y 124.06875 V.

La muestra 100 supera el límite superior. Esto la identifica como
atípica por ese criterio, pero no prueba una falla eléctrica ni
implica que sea la única muestra atípica.

## Preparación y métodos

Se excluyó la propia muestra 100: quedan 99 candidatos.
Se calculó la media y desviación estándar de cada variable sobre
esos candidatos y se aplicó z = (x - media)/desviación estándar.
La consulta utiliza los mismos parámetros.

Búsqueda exhaustiva:
se calculan todas las distancias y se ordenan para seleccionar cinco
vecinos. Los empates exactos se ordenan por identificador.

KDTree de SciPy:
se construye el árbol con los mismos datos estandarizados y se
consulta con k=5, p=2 y eps=0 para búsqueda euclidiana exacta.
En caso de empates, el árbol no necesariamente usa nuestro mismo
criterio de ordenamiento por identificador.

## Resultados euclidianos

| Posición | Muestra | Distancia estandarizada |
|---|---:|---:|
| 1 | 47 | 21.06424646 |
| 2 | 80 | 21.21978829 |
| 3 | 20 | 21.27856497 |
| 4 | 82 | 21.36483780 |
| 5 | 3 | 21.40990996 |

Ambos métodos coincidieron en los cinco identificadores y su orden.
Las distancias coincidieron dentro de la tolerancia numérica usada
por np.allclose. La coincidencia de vecinos fue 5/5 para esta consulta.

Esto verifica concordancia entre búsquedas, no precisión de un
clasificador ni recuperación correcta de los decimales originales.

Las distancias son adimensionales y grandes: los vecinos más próximos
disponibles no necesariamente son muy similares a la consulta.

## Comparación de métricas

Euclidiana: 47, 80, 20, 82, 3.
Manhattan: 20, 47, 3, 80, 82.

Cambió el orden, pero no el conjunto de vecinos.
Para ambas métricas coincidieron la búsqueda exhaustiva y KDTree.

## Tiempos

Se realizaron 100 operaciones de calentamiento por función y
15 bloques de 1000 operaciones. Se rotó el orden de medición.
Se reporta la mediana del tiempo por operación de los bloques.

| Operación | Mediana en microsegundos |
|---|---:|
| Consulta exhaustiva | 18.488 |
| Construcción del árbol | 47.712 |
| Consulta del árbol construido | 38.123 |

La consulta exhaustiva fue aproximadamente 2.06 veces más rápida.
La suma de las medianas de construcción y consulta es 85.835
microsegundos; es una estimación, no una medición conjunta.

Los tiempos excluyen lectura del CSV y estandarización.
Corresponden a consultas repetidas sobre el mismo conjunto y punto;
no representan una secuencia de consultas variadas en producción.
Se comparan estas implementaciones concretas, incluyendo el costo
de llamadas de Python y el ordenamiento de la búsqueda exhaustiva.

## Interpretación y monitoreo en tiempo real

La búsqueda exhaustiva permite explicar directamente cada distancia.
El árbol requiere entender las particiones espaciales y la poda,
aunque devuelve vecinos con la misma interpretación.

Para este conjunto pequeño elegiría la búsqueda exhaustiva:
es sencilla y tuvo menor tiempo de consulta.
En estas mediciones, reutilizar el árbol no compensa la diferencia,
pues incluso su consulta sin construcción fue más lenta.

Con más datos y pocas dimensiones, evaluaría nuevamente KDTree.
No se debe extrapolar este resultado a todos los tamaños.

Un sistema de tiempo real debe medir también adquisición, limpieza,
actualización de datos, comunicación y latencias máximas.
Una mediana pequeña no garantiza cumplir todos los plazos.

## Reproducción

Desde la raíz del repositorio, con NumPy y SciPy instalados:

    source .venv/bin/activate
    python Python/knn/preparar_datos.py
    python Python/knn/analizar_vecinos.py
    python Python/knn/medir_tiempos.py
    python Python/knn/comparar_metricas.py

Los programas generan CSV de conversión, vecinos, tiempos y métricas
en datos/knn. Nuevas ejecuciones sobrescriben esos CSV; los tiempos
pueden variar. Los TXT guardados con tee documentan la ejecución
utilizada en este informe.

Las respuestas P1-P5 están en [Preguntas.md](Preguntas.md).

## Fuentes y asistencia

- Enunciado Taller_KNN.pdf, suministrado por el profesor.
- Datos_MCEI.csv, suministrado por el estudiante como archivo del taller.
- Documentación de SciPy: https://docs.scipy.org/doc/scipy/reference/generated/scipy.spatial.KDTree.html

Se utilizó asistencia de IA para preparar los programas, interpretar
los resultados y redactar la documentación.

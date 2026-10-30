# Taller de geometría computacional con CGAL

Autores:
- Jimmy Alexander Calderón Trujillo.
- Sebastián Barahona.

Curso: Métodos Computacionales en Ingeniería.
Docente: Alexander Pérez Ruiz.
Entrega grupal.

## Objetivo

Estudiar fundamentos y robustez numérica de algoritmos geométricos,
calcular envolventes convexas y triangulaciones de Delaunay con CGAL,
y validar resultados mediante experimentos reproducibles.

## Contenido de la entrega

| Componente | Documento |
|---|---|
| Cinco preguntas de investigación, paquetes y aplicación propuesta | [Fundamentos](Fundamentos.md) |
| Esquemas de Andrew e inserción incremental de Delaunay | [Algoritmos](Algoritmos.md) |
| Experimento de precisión con EPICK y EPECK | [Análisis numérico](Analisis_numerico.md) |
| Tablas, gráficas y siete respuestas de análisis | [Resultados](Resultados.md) |
| Instalación, compilación y ejecución | [Reproducción](REPRODUCIR.md) |

## Implementación

- Código C++: ../../C_C++/cgal/
- Scripts Python: ../../Python/cgal/
- Datos originales y resultados: ../../datos/cgal/
- Visualizaciones: figuras/

C++ y CGAL realizan los cálculos geométricos.
Python automatiza experimentos, genera tablas y dibuja resultados.

## Experimentos realizados

- Cuatro tamaños: 100, 500, 1000 y 5000 puntos.
- Tres semillas: 42, 123 y 2026.
- Cinco mediciones por combinación: 60 mediciones registradas.
- Comprobaciones de validez, Euler y reproducibilidad de archivos.
- Casos de puntos repetidos, colineales y cocirculares.
- Procesamiento de las 100 ubicaciones suministradas por el profesor.
- Comparación de orientación con entradas double y racionales exactas.

## Resultados principales

En las nubes ensayadas, la proporción de vértices extremos disminuyó
del intervalo 10-13 % para 100 puntos al intervalo 0.32-0.40 %
para 5000 puntos. Los tiempos aumentaron con el tamaño.

Los sensores del profesor produjeron 100 vértices distintos,
12 vértices extremos, 285 aristas finitas y 186 caras finitas.
Se verificó 100 - 285 + 186 = 1.

El experimento numérico mostró que un kernel exacto no recupera
un desplazamiento perdido al construir previamente una entrada
en double.

## Alcance y limitaciones

Las mediciones corresponden al equipo y entorno documentados.
No prueban una complejidad asintótica ni garantizan tiempos máximos.

Las conexiones de Delaunay representan relaciones geométricas,
no cableado ni comunicación efectiva.

Voronoi recortado y la evaluación de cobertura se presentan como
propuesta conceptual; no se reportan como implementaciones realizadas.

## Fuentes y asistencia

Se utilizó el enunciado del taller y el CSV de ubicaciones entregados
por el profesor. Las referencias técnicas aparecen en los documentos.

Numerical Recipes y el cuadernillo de NotebookLM fueron indicados
por el profesor como recursos de apoyo. No se atribuyen a esos
materiales contenidos específicos que no hayan sido consultados.

Se utilizó asistencia de IA para preparar código y documentación,
interpretar resultados y orientar las comprobaciones.
Los resultados experimentales reportados proceden de las ejecuciones
realizadas en Ubuntu/WSL.

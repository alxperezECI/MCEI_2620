# Práctica MolSSI: MPI con Python y OpenMP con C++

Autor: Jimmy Alexander Calderón Trujillo
Curso: Métodos Computacionales en Ingeniería
Entrega individual

## Objetivo

Estudiar la distribución del trabajo, la comunicación entre procesos
y la sincronización entre hilos mediante los ejemplos de MolSSI.
Comprobar los resultados numéricos y analizar los tiempos medidos.

## Fuente y autoría

Guía: https://education.molssi.org/parallel-programming/index.html

Repositorio: https://github.com/MolSSI-Education/parallel-programming

Los ejemplos de Monte Carlo y OpenMP proceden de MolSSI.
Esta entrega contiene su ejecución, adaptaciones y análisis.
Los programas MPI de saludo y arreglos se elaboraron siguiendo la guía.
Se utilizó asistencia de IA para preparar código, revisar errores
e interpretar los resultados.

La revisión utilizada está registrada en:
../../datos/molssi/version_fuente_molssi.txt

Se adjunta la licencia original en LICENCIA_MolSSI.md.

## Entorno

Ubuntu sobre WSL, Python 3.12.3, NumPy 2.5.3, mpi4py 4.1.2,
Open MPI 4.1.6 y GCC/G++ 13.3.0.

El equipo reportó cuatro procesadores lógicos. Las ejecuciones MPI
utilizaron --use-hwthread-cpus. Esto no implica cuatro núcleos físicos.
Las características completas están en ../../datos/molssi/entorno.txt.

## MPI con Python

### Ejemplo 1: saludo

Se ejecutó con dos y cuatro procesos. COMM_WORLD agrupa los procesos;
Get_rank identifica cada uno y Get_size indica el total.
El orden de impresión puede variar.

### Ejemplo 2: arreglos distribuidos

Se utilizaron N = 10000000 elementos:
a[i] = 1, b[i] = i + 1.

El promedio esperado es (N + 3)/2 = 5000001.5.
Cada proceso almacena únicamente su bloque de los arreglos.
El reparto contempla el resto cuando N no es divisible por el
número de procesos.

Se implementaron dos formas de reunir las sumas:
- Reduce: operación colectiva con resultado en el proceso 0.
- Send/Recv: envío explícito de las sumas al proceso 0.

Todas las ejecuciones mostradas dieron el promedio esperado y error cero.

Resultados con Reduce: mediana de tres ejecuciones por configuración.

| Procesos | Tiempo (s) | Aceleración | Eficiencia |
|---|---:|---:|---:|
| 1 | 12.719255 | 1.00 | 100 % |
| 2 | 8.400621 | 1.51 | 75.7 % |
| 4 | 7.761444 | 1.64 | 41.0 % |

Aceleración = T1/Tp. Eficiencia = aceleración/p.
La referencia es el mismo programa MPI con un proceso.

Send/Recv con cuatro procesos tardó 8.739324 s en una ejecución.
No hay suficientes repeticiones de esta variante para establecer
una comparación concluyente frente a Reduce.

### Ejemplo 3: Monte Carlo

Se ejecutaron 10000 pasos del ejemplo de Lennard-Jones.

| Versión | Tiempo (s) | Energía final |
|---|---:|---:|
| Original, 1 proceso | 29.099497 | -5.734179189975 |
| MPI, 1 proceso | 33.000723 | -5.734179189975 |
| MPI, 4 procesos | 27.657796 | -5.734179190176 |

La diferencia final es aproximadamente 2e-10.
La versión paralela reparte las interacciones entre procesos.
Reduce reúne las energías en el proceso 0, que decide los movimientos.
Bcast distribuye la configuración y las propuestas para mantener
sincronizados los procesos.

En esta implementación Reduce es suficiente porque solo el proceso 0
utiliza las energías globales para decidir. Allreduce sería necesario
si todos necesitaran disponer del resultado global.

## OpenMP con C++

### Ejemplo 1: regiones paralelas

La primera región ejecutó cuatro saludos, la sección secuencial un
mensaje y la segunda región tres despedidas por num_threads(3).
Esto muestra la creación y reunión de grupos de hilos.

### Ejemplo 2: arreglos

Se redujo N de 1000000000 a 10000000 en ambas versiones.
Los dos arreglos int requieren aproximadamente 80 MB si int ocupa
4 bytes. La versión secuencial se obtuvo retirando las directivas
OpenMP de la versión instrumentada, conservando sus temporizadores.

La suma paralela utiliza reduction(+:average).
Las cuatro pruebas iniciales dieron el promedio 5000001.5.

Medianas de cinco ejecuciones:

| Configuración | Tiempo (s) | Aceleración frente a secuencial |
|---|---:|---:|
| Secuencial | 0.127763 | 1.00 |
| OpenMP, 1 hilo | 0.117947 | 1.08 |
| OpenMP, 2 hilos | 0.083354 | 1.53 |
| OpenMP, 4 hilos | 0.068105 | 1.88 |

La diferencia con un hilo no se debe a concurrencia.
Pueden influir el código compilado y la variabilidad temporal.

### Ejemplo 3: fuerzas

Se conservaron 1600 partículas y 1000 iteraciones.

| Versión | Tiempo (s) |
|---|---:|
| Original, 1 hilo | 16.044852 |
| Reformulada, 1 hilo | 32.290090 |
| Reformulada, 4 hilos | 20.318291 |

La energía final impresa fue 92079.129718 y la potencial 16253.127101
en las tres ejecuciones.

La reformulación asigna cada forces[i] a una iteración del bucle
exterior. Recorre cada pareja dos veces y divide por dos su
contribución a la energía potencial.

Con cuatro hilos mejora frente a la misma versión con uno,
pero tarda aproximadamente 26.6 % más que el original.
El trabajo adicional no quedó compensado por la paralelización.

Se eliminó una declaración local de forces sin uso en main
de ambas versiones. Las mediciones preceden a esa limpieza.

### Ejemplo 4: dinámica molecular

Se conservaron 8000 partículas y 1000 pasos.

La versión paralela utiliza listas de vecinos por hilo, arreglos
privados de fuerzas y una sección critical para combinar resultados.

Correcciones realizadas sobre la versión final de MolSSI:
- Comparar thrneigh[i].size() con npair en el balanceo.
- Usar single para asignar nthreads sin escrituras concurrentes.
- Usar std::size_t para índices y conteos de listas.
- Ajustar la división entera para contemplar cero parejas.

| Versión | Fuerzas (s) | Vecinos (s) | Total (s) |
|---|---:|---:|---:|
| Original, 1 hilo | 24.12 | 29.30 | 53.65 |
| Paralela, 1 hilo | 19.99 | 28.94 | 49.21 |
| Paralela, 4 hilos | 9.22 | 10.02 | 19.59 |

La aceleración frente al original fue 2.74 y frente a la misma
versión paralela con un hilo fue 2.51.

La energía total final fue -15425.51136 en el original y
-15425.51137 con cuatro hilos. La temperatura y la presión
coincidieron a la precisión impresa: 0.495 y 0.03703601.

## Limitaciones

Las simulaciones se midieron una vez por configuración.
Sus tiempos son observaciones, no estimaciones estadísticas robustas.
Las repeticiones de arreglos se hicieron en un orden fijo y pueden
estar afectadas por carga del equipo, caché y frecuencia del procesador.

Los temporizadores corresponden a las regiones definidas en cada
programa y no incluyen necesariamente el lanzamiento del proceso.
En los arreglos OpenMP, el total incluye impresiones y liberación
de memoria. No equivale exactamente al alcance temporal de MPI.

La coincidencia de salidas impresas no prueba igualdad bit a bit
ni constituye una validación física completa de las simulaciones.

No se comparan directamente MPI/Python y OpenMP/C++ como si solo
cambiara la tecnología: también cambian lenguaje, tipos de datos,
implementación y compilación.

## Conclusiones

1. MPI requiere repartir explícitamente datos y trabajo; lanzar
   más procesos sin reparto puede duplicar el cálculo.
2. OpenMP permite compartir memoria, pero exige evitar condiciones
   de carrera mediante reducción, datos privados o sincronización.
3. Más procesos o hilos no garantizan una aceleración proporcional.
4. El ejemplo de fuerzas muestra que una reformulación puede ser
   más lenta que el original aunque mejore al aumentar los hilos.
5. Primero se debe comprobar el resultado y después interpretar
   el rendimiento.
6. Los resultados corresponden a este equipo y a estas mediciones.

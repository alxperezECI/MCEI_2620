## -------------------------------------------------------------
# OpenMP - Integración numérica
## -------------------------------------------------------------
## Archivos
- `hola_openmp.c`: ejecución de hilos con OpenMP.
- `simpson_openmp.c`: integración paralela mediante Simpson 1/3.
- `trapecio_openmp.c`: integración paralela mediante trapecios.
 
## Compilación
```bash
gcc -fopenmp hola_openmp.c -o hola_openmp
gcc -O2 -fopenmp simpson_openmp.c -lm -o simpson_openmp
gcc -O2 -fopenmp trapecio_openmp.c -lm -o trapecio_openmp
```
## Ejecución
```bash
OMP_NUM_THREADS=4 ./hola_openmp
OMP_NUM_THREADS=4 ./simpson_openmp
OMP_NUM_THREADS=4 ./trapecio_openmp
```
Para comparar el rendimiento, ejecutar los métodos de integración con 1, 2, 4 y 8 hilos.
 
## Resultados
Se utilizaron 1.000.000 de subintervalos:
| Hilos | Simpson (s) | Trapecio (s) |
| 1     | 0.062752    | 0.055348     |
| 2     | 0.030458    | 0.031882     |
| 4     | 0.027565    | 0.021910     |
| 8     | 0.045403    | 0.019995     |
Ambos métodos obtuvieron un resultado de `2.0000000000`.
 
## Conclusiones
- Simpson obtuvo el menor tiempo con 4 hilos: 0.027565 s.
- Trapecio obtuvo el menor tiempo con 8 hilos: 0.019995 s.
- La paralelización redujo los tiempos de ejecución respecto a un solo hilo.

## -------------------------------------------------------------
## MPI - Operaciones con arreglos NumPy
## -------------------------------------------------------------
### Programas desarrollados
- `operaciones_arreglos_mpi.py`: implementación secuencial.
- `operaciones_arreglos_mpi_paralelo.py`: implementación paralela con MPI.
 
### Implementación
 
**Versión secuencial:**
- Inicialización de dos arreglos de 10 millones de elementos.
- Suma de los arreglos.
- Cálculo del promedio.
- Medición de los tiempos de ejecución.
 
**Versión paralela:**
- Distribución de los arreglos mediante `MPI.Send` y `MPI.Recv`.
- Procesamiento de los datos entre varios procesos.
- Cálculo de las sumas parciales.
- Obtención del resultado global mediante `MPI.Reduce`.
- Medición del tiempo de ejecución con `MPI.Wtime`.
 
### Resultados secuenciales
| Operación            | Tiempo (s) |
| Inicialización A     | 0.160576   |
| Inicialización B     | 4.874800   |
| Suma de arreglos     | 13.114829  |
| Cálculo del promedio | 5.894204  |
| **Total**            |  **24.044409** |
 
### Resultados paralelos
| Procesos MPI | Tiempo (s) |
| 1            | 0.571446 |
| 2            | 0.164992 |
| 4            | 0.159188 |
| 8            | 0.581627 |
 
El promedio calculado en ambas implementaciones fue
`5000001.500000`.
 
### Análisis
La implementación paralela obtuvo su menor tiempo con cuatro procesos MPI (0.159188 s).
Con ocho procesos, el tiempo aumentó a 0.581627 s, debido posiblemente a la sobresuscripción y la competencia por recursos.
La versión paralela también utiliza operaciones vectorizadas de NumPy, mientras que la secuencial emplea ciclos de Python. Por esta razón, la diferencia de tiempos entre ambas versiones no representa exclusivamente la aceleración obtenida mediante MPI.

# -------------------------------------------------------------
# MPI - Simulación Monte Carlo (MolSSI)
# -------------------------------------------------------------
**Archivo:** `taller_mpi_openmp/mpi_python/monte_carlo_mpi.py`
 
**Implementación:**
- Simulación Monte Carlo de 100 partículas durante 10.000 pasos.
- Distribución del cálculo de energía entre procesos MPI.
- Uso de `MPI.Allreduce` para sumar las energías parciales.
- Uso de `MPI.Bcast` y `bcast` para sincronizar los movimientos y las decisiones de aceptación.
 
**Resultados:**
| Procesos MPI | Tiempo total (s) | Energía final |
| 1            | 68.8431          | -5.734179     |
| 2            | 81.2049          | -5.734179     |
| 4            | 52.2377          | -5.734179     |
 
**Análisis:**
- La simulación produjo la misma energía final con 1, 2 y 4 procesos MPI.
- El menor tiempo se obtuvo con cuatro procesos (52.24 s), alcanzando una reducción del 24 1 % respecto a la ejecución con un proceso.
- Con dos procesos, el tiempo aumentó a 81.20 s, posiblemente debido a los costos de comunicación y sincronización MPI.

# -------------------------------------------------------------
# Ejercicios OpenMP – MolSSI
# -------------------------------------------------------------
Se desarrollaron tres ejemplos de programación paralela con memoria compartida utilizando C++ y OpenMP, siguiendo la guía de MolSSI.
 
## Example 1 – Hello World con OpenMP
Se implementó un programa para ejecutar instrucciones mediante múltiples hilos, utilizando las directivas de OpenMP.
Se utilizaron `#pragma omp parallel`, `omp_get_thread_num()` y `num_threads()` para identificar los hilos y controlar su cantidad.
**Resultados:**
- Primera región paralela ejecutada con 4 hilos.
- Segunda región paralela ejecutada con 3 hilos.
- Cada hilo imprimió su identificador.
- Se observó que el orden de ejecución de los hilos puede variar.
**Conclusión:** Se verificó el funcionamiento de las regiones paralelas de OpenMP y la ejecución de instrucciones mediante múltiples hilos.
 
## Example 2 – Operaciones con arreglos
Se implementó la inicialización, suma y cálculo del promedio de dos arreglos de 10.000.000 de elementos.
Se comparó la ejecución secuencial con una implementación paralela mediante `#pragma omp parallel for` y `reduction(+:average)`.
El tamaño de los arreglos se ajustó para evitar problemas de memoria en WSL.
**Resultados:**
| Ejecución             | Tiempo total (s) | Aceleración |
| Original (secuencial) | 0.296685         | 1.00        |
| 1 hilo (OpenMP)       | 0.411119         | 0.72        |
| 2 hilos               | 0.195199         | 1.52        |
| 4 hilos               | 0.175828         | 1.69        |
| 8 hilos               | 0.135377         | 2.19        |
Todas las ejecuciones obtuvieron el mismo promedio: **5000001.500000**.
**Conclusión:** La ejecución con 8 hilos presentó el mejor rendimiento, reduciendo el tiempo aproximadamente un 54.4 % respecto a la versión secuencial. La ejecución con un hilo de OpenMP fue más lenta que la original.
 
## Example 3 – Simulación de partículas
Se implementó la paralelización de una simulación de 1.600 partículas distribuidas en una estructura bidimensional, durante 1.000 iteraciones. 
**Resultados:**
| Ejecución             | Tiempo total (s) | Aceleración |
| Original (secuencial) | 27.478013        | 1.00        |
| 2 hilos               | 20.189002        | 1.36        |
| 4 hilos               | 9.585511         | 2.87        |
| 8 hilos               | 5.428162         | 5.06        |
Todas las ejecuciones conservaron los resultados finales:
- Energía: 92079.129718
- Energía potencial: 16253.127101
**Conclusión:** La ejecución con 8 hilos presentó el mejor rendimiento, reduciendo el tiempo aproximadamente un 80.2 % respecto a la versión original. La paralelización permitió acelerar principalmente el cálculo de fuerzas sin alterar los resultados numéricos mostrados.

## Example 4 – Dinámica molecular con OpenMP
Se implementó la paralelización de una simulación de dinámica molecular utilizando C++ y OpenMP.
El programa original de MolSSI se ajustó de 8.000 a 2.000 partículas para reducir el tiempo de procesamiento, conservando los 1.000 pasos de simulación.
Se paralelizaron la construcción de listas de vecinos, el cálculo de fuerzas y la actualización de posiciones y velocidades.
**Resultados:**
| Ejecución             | Fuerzas (s) | Vecinos (s) | Tiempo total (s) | Aceleración |
| Original (secuencial) | 4.07        | 2.60        | 6.73             | 1.00        |
| 2 hilos               | 2.85        | 1.80        | 4.71             | 1.43        |
| 4 hilos               | 1.57        | 1.06        | 2.68             | 2.51        |
| 8 hilos               | 1.89        | 0.73        | 2.98             | 2.26        |
Todas las ejecuciones conservaron los resultados finales:
- Energía cinética: 859.07737
- Energía potencial: -3495.03962
- Energía total: -2635.96225
- Temperatura: 0.428
- Presión: -0.00008083
**Conclusión:** La ejecución con 4 hilos presentó el mejor rendimiento, reduciendo el tiempo total aproximadamente un 60.2 % respecto a la versión original. Aunque la construcción de vecinos fue más rápida con 8 hilos, el aumento del tiempo de cálculo de fuerzas redujo el rendimiento general.
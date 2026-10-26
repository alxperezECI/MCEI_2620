import numpy as np
from mpi4py import MPI
 
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
 
N = 10_000_000
 
# Inicialización del arreglo A
inicio = MPI.Wtime()
a = np.ones(N)
tiempo_a = MPI.Wtime() - inicio
 
# Inicialización del arreglo B
inicio = MPI.Wtime()
b = np.zeros(N)
 
for i in range(N):
    b[i] = 1.0 + i
 
tiempo_b = MPI.Wtime() - inicio
 
# Suma de los arreglos
inicio = MPI.Wtime()
 
for i in range(N):
    a[i] = a[i] + b[i]
 
tiempo_suma = MPI.Wtime() - inicio
 
# Cálculo del promedio
inicio = MPI.Wtime()
 
suma = 0.0
for i in range(N):
    suma += a[i]
 
promedio = suma / N
tiempo_promedio = MPI.Wtime() - inicio
 
# Resultados
if rank == 0:
    print("RESULTADOS - OPERACIONES CON ARREGLOS")
    print(f"Inicialización A: {tiempo_a:.6f} s")
    print(f"Inicialización B: {tiempo_b:.6f} s")
    print(f"Suma de arreglos: {tiempo_suma:.6f} s")
    print(f"Cálculo promedio: {tiempo_promedio:.6f} s")
    print(f"Promedio: {promedio:.6f}")
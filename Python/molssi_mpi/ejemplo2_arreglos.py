"""Arreglos distribuidos: memoria local y reduccion de sumas."""
import numpy as np
from mpi4py import MPI

comm = MPI.COMM_WORLD
rango = comm.Get_rank()
procesos = comm.Get_size()
N = 10_000_000

# Repartir todos los elementos, incluso si N no es divisible.
base, resto = divmod(N, procesos)
cantidad = base + int(rango < resto)
inicio = rango * base + min(rango, resto)

comm.Barrier()
t_total = MPI.Wtime()
tiempos = []

t = MPI.Wtime()
a = np.ones(cantidad, dtype=np.float64)
tiempos.append(MPI.Wtime() - t)

t = MPI.Wtime()
b = np.empty(cantidad, dtype=np.float64)
for i in range(cantidad):
    b[i] = 1.0 + inicio + i
tiempos.append(MPI.Wtime() - t)

t = MPI.Wtime()
for i in range(cantidad):
    a[i] += b[i]
tiempos.append(MPI.Wtime() - t)

t = MPI.Wtime()
suma = 0.0
for i in range(cantidad):
    suma += a[i]

local = np.array([suma], dtype=np.float64)
global_ = np.zeros(1, dtype=np.float64)
comm.Reduce(local, global_, op=MPI.SUM, root=0)
tiempos.append(MPI.Wtime() - t)
duracion = MPI.Wtime() - t_total

# Reportar el mayor tiempo entre procesos para cada etapa.
maximos = np.zeros(4, dtype=np.float64)
comm.Reduce(np.array(tiempos, dtype=np.float64), maximos, op=MPI.MAX, root=0)
total_max = comm.reduce(duracion, op=MPI.MAX, root=0)
reparto = comm.gather(cantidad, root=0)

if rango == 0:
    promedio = global_[0] / N
    esperado = (N + 3) / 2
    print(f"Elementos: {N}; procesos: {procesos}")
    print(f"Reparto: {reparto}")
    print(f"Promedio: {promedio:.6f}")
    print(f"Esperado: {esperado:.6f}")
    print(f"Error absoluto: {abs(promedio - esperado):.3e}")
    etapas = ["Inicializar a", "Inicializar b", "Sumar arreglos",
              "Sumar elementos y comunicar"]
    for nombre, segundos in zip(etapas, maximos):
        print(f"{nombre}: {segundos:.6f} s")
    print(f"Tiempo total (maximo): {total_max:.6f} s")

from mpi4py import MPI
import numpy as np
 
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()
 
N = 10_000_000
 
if N % size != 0:
    if rank == 0:
        print("El número de procesos debe dividir exactamente N.")
    comm.Abort(1)
 
n_local = N // size
 
a_local = np.empty(n_local, dtype=np.float64)
b_local = np.empty(n_local, dtype=np.float64)
 
comm.Barrier()
inicio = MPI.Wtime()
 
# El proceso 0 crea y distribuye los arreglos
if rank == 0:
    a = np.ones(N, dtype=np.float64)
    b = np.arange(1, N + 1, dtype=np.float64)
 
    a_local[:] = a[:n_local]
    b_local[:] = b[:n_local]
 
    for destino in range(1, size):
        i = destino * n_local
        j = i + n_local
 
        comm.Send(a[i:j], dest=destino, tag=0)
        comm.Send(b[i:j], dest=destino, tag=1)
 
else:
    comm.Recv(a_local, source=0, tag=0)
    comm.Recv(b_local, source=0, tag=1)
 
# Cada proceso suma sus elementos
a_local += b_local
 
# Cada proceso calcula su suma parcial
suma_local = np.sum(a_local)
 
# Reducción de las sumas parciales
suma_total = comm.reduce(
    suma_local,
    op=MPI.SUM,
    root=0
)
 
tiempo_local = MPI.Wtime() - inicio
 
# Tiempo del proceso que más tardó
tiempo_total = comm.reduce(
    tiempo_local,
    op=MPI.MAX,
    root=0
)
 
if rank == 0:
    promedio = suma_total / N
 
    print("RESULTADOS - MPI PARALELO")
    print(f"Procesos: {size}")
    print(f"Elementos: {N}")
    print(f"Promedio: {promedio:.6f}")
    print(f"Tiempo total: {tiempo_total:.6f} s")
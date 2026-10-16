"""Ejemplo 1: identificar los procesos del comunicador MPI."""
from mpi4py import MPI

comm = MPI.COMM_WORLD
rango = comm.Get_rank()
total = comm.Get_size()

print(f"Hola desde el proceso {rango} de {total}", flush=True)

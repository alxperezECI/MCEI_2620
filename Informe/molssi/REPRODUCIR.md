# Reproducción de la práctica

Ejecutar los comandos desde la raíz del repositorio en Ubuntu.
Requisitos: Python, NumPy, mpi4py, Open MPI y g++ con OpenMP.
Las versiones utilizadas están en datos/molssi/entorno.txt.

## Preparación

    source .venv/bin/activate
    mkdir -p /tmp/molssi_bin
    export OMP_DYNAMIC=FALSE

## MPI con Python

    mpiexec --use-hwthread-cpus -n 4 python Python/molssi_mpi/ejemplo1_hola.py
    mpiexec --use-hwthread-cpus -n 4 python Python/molssi_mpi/ejemplo2_arreglos.py
    mpiexec --use-hwthread-cpus -n 4 python Python/molssi_mpi/ejemplo2_arreglos_p2p.py
    mpiexec --use-hwthread-cpus -n 4 python Python/molssi_mpi/ejemplo3_mpi.py

Cambiar -n 4 por -n 1 o -n 2 para otras configuraciones.
Referencia original de Monte Carlo:

    python Python/molssi_mpi/ejemplo3_original.py

## Compilar OpenMP

    for fuente in C_C++/molssi_openmp/ejemplo*/*.cpp; do
        ejemplo=$(basename "$(dirname "$fuente")")
        nombre=$(basename "$fuente" .cpp)
        g++ -O2 -Wall -Wextra -fopenmp "$fuente" -o "/tmp/molssi_bin/${ejemplo}_${nombre}" || break
    done

## Ejecutar OpenMP

    OMP_NUM_THREADS=4 /tmp/molssi_bin/ejemplo1_hola
    /tmp/molssi_bin/ejemplo2_arreglos_secuencial
    OMP_NUM_THREADS=4 /tmp/molssi_bin/ejemplo2_arreglos_paralelo
    OMP_NUM_THREADS=1 /tmp/molssi_bin/ejemplo3_original
    OMP_NUM_THREADS=4 /tmp/molssi_bin/ejemplo3_paralelo
    OMP_NUM_THREADS=1 /tmp/molssi_bin/ejemplo4_original
    OMP_NUM_THREADS=4 /tmp/molssi_bin/ejemplo4_paralelo

Cambiar OMP_NUM_THREADS para comparar cantidades de hilos.
Los ejecutables quedan en /tmp y no se incluyen en Git.
Los tiempos pueden variar entre ejecuciones.

#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p build ../../datos/openmp
cc=${CC:-gcc}
"$cc" -O2 -Wall -Wextra -std=c11 -fopenmp hola.c -o build/hola
"$cc" -O2 -Wall -Wextra -std=c11 -fopenmp integracion.c -lm -o build/integracion
OMP_NUM_THREADS=4 ./build/hola
cd ../../datos/openmp
{
 date -Is
 uname -a
 "$cc" --version
 lscpu
} > entorno.txt
../../C_C++/openmp/build/integracion | tee salida.txt

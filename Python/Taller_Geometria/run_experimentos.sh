#!/usr/bin/env bash
# Corre los experimentos y guarda resultados.csv
set -e
EXE=./build/taller_cgal
OUT=resultados.csv
echo "mode,n,seed,V,hull,E,F,euler,t_hull_ms,t_delaunay_ms" > $OUT
for n in 100 500 1000 5000; do
  for seed in 1 2 3 42 123; do
    $EXE $n $seed random | tail -1 >> $OUT
  done
done
# Casos degenerados
for m in duplicates collinear cocircular grid; do
  $EXE 100 42 $m | tail -1 >> $OUT
done
# Misma semilla repetida (pregunta 7)
for r in 1 2 3; do $EXE 1000 42 random | tail -1 >> $OUT; done
echo "Listo: $OUT"
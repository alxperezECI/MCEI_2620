"""Compara Euclidiana y Manhattan con la misma estandarizacion."""
import csv
import runpy
from pathlib import Path
import numpy as np
from scipy.spatial import KDTree

carpeta = Path(__file__).resolve().parent
datos = runpy.run_path(str(carpeta / "analizar_vecinos.py"))
Z, q, ids = datos["Z"], datos["q"], datos["ids_ref"]
arbol = KDTree(Z)
filas = []

for nombre, p in [("euclidiana", 2), ("manhattan", 1)]:
    diferencia = abs(Z - q)
    if p == 2:
        distancias = np.sqrt(np.sum(diferencia**2, axis=1))
    else:
        distancias = np.sum(diferencia, axis=1)

    orden = np.lexsort((ids, distancias))[:5]
    dist_kd, ind_kd = arbol.query(q, k=5, p=p, eps=0)
    coinciden = set(ids[orden]) == set(ids[ind_kd])
    assert np.allclose(distancias[orden], dist_kd)

    print(f"\n{nombre}: vecinos {ids[orden]}")
    print("Coinciden ambos metodos:", coinciden)
    for posicion, indice in enumerate(orden, start=1):
        filas.append([
            nombre, posicion, int(ids[indice]),
            float(distancias[indice])
        ])

salida = datos["raiz"] / "datos" / "knn" / "comparacion_metricas.csv"
with salida.open("w", newline="", encoding="utf-8") as f:
    escritor = csv.writer(f)
    escritor.writerow(["metrica", "posicion", "muestra", "distancia"])
    escritor.writerows(filas)

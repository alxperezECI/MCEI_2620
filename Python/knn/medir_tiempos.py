"""Mediciones repetidas; no incluyen lectura ni estandarizacion."""
import csv
import runpy
from pathlib import Path
from time import perf_counter_ns
import numpy as np
from scipy.spatial import KDTree

carpeta = Path(__file__).resolve().parent
datos = runpy.run_path(str(carpeta / "analizar_vecinos.py"))
Z, q, ids_ref = datos["Z"], datos["q"], datos["ids_ref"]
arbol = KDTree(Z)

def exhaustiva():
    dist = np.linalg.norm(Z - q, axis=1)
    ind = np.lexsort((ids_ref, dist))[:5]
    return dist[ind], ind

def construir():
    return KDTree(Z)

def consultar():
    return arbol.query(q, k=5, p=2, eps=0)

funciones = {
    "exhaustiva": exhaustiva,
    "construccion_kdtree": construir,
    "consulta_kdtree": consultar
}

# Calentamiento.
for funcion in funciones.values():
    for _ in range(100):
        funcion()

# Rotar el orden entre bloques para reducir sesgos temporales.
nombres = list(funciones)
registros = []
repeticiones = 1000
for bloque in range(15):
    orden = nombres[bloque % 3:] + nombres[:bloque % 3]
    for nombre in orden:
        funcion = funciones[nombre]
        inicio = perf_counter_ns()
        for _ in range(repeticiones):
            funcion()
        microsegundos = (
            (perf_counter_ns() - inicio) / repeticiones / 1000
        )
        registros.append([bloque + 1, nombre, microsegundos])

salida = datos["raiz"] / "datos" / "knn" / "tiempos.csv"
with salida.open("w", newline="", encoding="utf-8") as f:
    escritor = csv.writer(f)
    escritor.writerow(["bloque", "operacion", "microsegundos_por_operacion"])
    escritor.writerows(registros)

print("\nTIEMPOS: mediana de 15 bloques de 1000 operaciones")
for nombre in nombres:
    valores = [r[2] for r in registros if r[1] == nombre]
    print(f"{nombre}: {np.median(valores):.3f} microsegundos")

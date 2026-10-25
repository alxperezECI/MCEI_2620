"""Compara busqueda exhaustiva y KDTree sobre datos interpretados."""
from pathlib import Path
import numpy as np
from scipy.spatial import KDTree

raiz = Path(__file__).resolve().parents[2]
carpeta = raiz / "datos" / "knn"
datos = np.genfromtxt(
    carpeta / "sensores_interpretados.csv",
    delimiter=",", names=True, encoding="utf-8"
)
ids = datos["muestra"].astype(int)
X = np.column_stack([
    datos["voltaje_V"],
    datos["corriente_A"],
    datos["potencia_W_aprox"]
])
assert np.isfinite(X).all(), "Hay datos no finitos"
assert len(np.unique(ids)) == len(ids), "Hay identificadores duplicados"

# Seleccion por desviacion respecto a la mediana del voltaje.
voltajes = X[:, 0]
consulta = int(np.argmax(abs(voltajes - np.median(voltajes))))
q1, q3 = np.percentile(voltajes, [25, 75])
limites = (q1 - 1.5 * (q3 - q1), q3 + 1.5 * (q3 - q1))
atipico = not limites[0] <= voltajes[consulta] <= limites[1]

# Excluir la consulta y ajustar la escala con las otras muestras.
mascara = np.arange(len(X)) != consulta
referencia = X[mascara]
ids_ref = ids[mascara]
media = referencia.mean(axis=0)
escala = referencia.std(axis=0)
assert np.all(escala > 0), "Hay variables constantes"
Z = (referencia - media) / escala
q = (X[consulta] - media) / escala

# KNN exhaustivo: todas las distancias euclidianas.
distancias = np.linalg.norm(Z - q, axis=1)
orden = np.lexsort((ids_ref, distancias))[:5]

# KDTree: busqueda exacta sobre los mismos datos y escala.
arbol = KDTree(Z)
dist_kd, ind_kd = arbol.query(q, k=5, p=2, eps=0)

print("Consulta:", ids[consulta], "Valores:", X[consulta])
print("Limites IQR del voltaje:", limites)
print("Voltaje atipico por IQR:", atipico)
print("Vecinos exhaustivos:", ids_ref[orden])
print("Vecinos KDTree:", ids_ref[ind_kd])
print("Distancias exhaustivas:", distancias[orden])
print("Distancias KDTree:", dist_kd)
print("Coinciden los conjuntos:",
      set(ids_ref[orden]) == set(ids_ref[ind_kd]))

assert np.allclose(distancias[orden], dist_kd), "Distancias diferentes"

tabla = np.column_stack([
    ids_ref[orden], referencia[orden], distancias[orden]
])
np.savetxt(
    carpeta / "vecinos_euclidianos.csv", tabla, delimiter=",",
    header="muestra,voltaje_V,corriente_A,potencia_W_aprox,distancia",
    comments="", fmt=["%d", "%.2f", "%.2f", "%.2f", "%.10f"]
)

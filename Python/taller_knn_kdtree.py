import time
import numpy as np
import pandas as pd
from scipy.spatial import cKDTree
 
# ==========================================
# 1. LECTURA DE DATOS
# ==========================================
 
datos = pd.read_csv("Datos_MCEI.csv", sep=";")
columnas = ["voltaje_V", "corriente_A", "potencia_W_aprox"]
X = datos[columnas].to_numpy(dtype=float)
print("Numero de sensores:", len(datos))
print("\nPrimeros registros:")
print(datos.head().to_string(index=False))

# ==========================================
# 2. IDENTIFICACION DEL SENSOR ANOMALO
# ==========================================
# Se selecciona el voltaje con mayor desviacion
# absoluta respecto a la mediana.
voltajes = datos["voltaje_V"].to_numpy()
mediana = np.median(voltajes)
indice_anomalo = np.argmax(np.abs(voltajes - mediana))
print("\nSENSOR CON ANOMALIA DE VOLTAJE")

print(datos.iloc[indice_anomalo].to_string())
 
# ==========================================
# 3. NORMALIZACION
# ==========================================
media = X.mean(axis=0)
desviacion = X.std(axis=0)
if np.any(desviacion == 0):
    raise ValueError("Hay variables sin variacion.")
X_norm = (X - media) / desviacion
punto = X_norm[indice_anomalo]

k = 5

# ==========================================
# 4. KNN POR FUERZA BRUTA
# ==========================================
inicio = time.perf_counter()
distancias = np.linalg.norm(X_norm - punto, axis=1)
distancias[indice_anomalo] = np.inf
indices_knn = np.argsort(distancias)[:k]
distancias_knn = distancias[indices_knn]
tiempo_knn = time.perf_counter() - inicio
 
# ==========================================
# 5. KD-TREE
# ==========================================
inicio = time.perf_counter()
arbol = cKDTree(X_norm)
tiempo_construccion = time.perf_counter() - inicio
inicio = time.perf_counter()

# k+1 porque el propio sensor es un vecino
distancias_tree, indices_tree = arbol.query(
    punto, k=k + 1
)
 
pares = [
    (int(i), float(d))
    for i, d in zip(
        np.atleast_1d(indices_tree),
        np.atleast_1d(distancias_tree)
    )
    if i != indice_anomalo
][:k]
 
indices_kdtree = np.array([i for i, d in pares])
distancias_kdtree = np.array([d for i, d in pares])
tiempo_consulta = time.perf_counter() - inicio
 
# ==========================================
# 6. RESULTADOS
# ==========================================
resultado_knn = datos.iloc[indices_knn].copy()
resultado_knn["distancia"] = distancias_knn

resultado_tree = datos.iloc[indices_kdtree].copy()
resultado_tree["distancia"] = distancias_kdtree
print("\nCINCO VECINOS MAS CERCANOS - KNN")
print(resultado_knn.to_string(index=False))
print("\nCINCO VECINOS MAS CERCANOS - KD-TREE")
print(resultado_tree.to_string(index=False))
 
# ==========================================
# 7. COMPARACION
# ==========================================
coincidencia = (
    set(indices_knn.tolist())
    == set(indices_kdtree.tolist())
)
 
print("\nCOMPARACION DE METODOS")
print(f"Tiempo KNN: {tiempo_knn:.8f} s")
print(f"Construccion kd-tree: {tiempo_construccion:.8f} s")
print(f"Consulta kd-tree: {tiempo_consulta:.8f} s")
print(f"Coincidencia de vecinos: {coincidencia}")

# ==========================================
# 8. VISUALIZACION DE LOS SENSORES
# ==========================================
import matplotlib.pyplot as plt
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection="3d")

# Todos los sensores
ax.scatter(
    X_norm[:, 0],
    X_norm[:, 1],
    X_norm[:, 2],
    color="lightgray",
    alpha=0.6,
    label="Sensores"
)
 
# Cinco vecinos mas cercanos
ax.scatter(
    X_norm[indices_knn, 0],
    X_norm[indices_knn, 1],
    X_norm[indices_knn, 2],
    color="blue",
    s=75,
    label="5 vecinos cercanos"
)
 
# Sensor anomalo
ax.scatter(
    punto[0],
    punto[1],
    punto[2],
    color="red",
    edgecolors="black",
    linewidths=1,
    marker="*",
    s=450,
    depthshade=False,
    label="Sensor anomalo"
)
 
# Etiquetas de los cinco vecinos
for i in indices_knn:
    ax.text(
        X_norm[i, 0] + 0.08,
        X_norm[i, 1] + 0.08,
        X_norm[i, 2] + 0.08,
        f"{int(datos.iloc[i]['muestra'])}",
        fontsize=9,
        fontweight="bold"
    )
 
# Etiqueta del sensor anomalo
ax.text(
    punto[0] - 0.25,
    punto[1],
    punto[2] + 0.2,
    "Sensor 74",
    color="red",
    fontsize=10,
    fontweight="bold"
)
ax.set_xlabel("Voltaje normalizado")
ax.set_ylabel("Corriente normalizada")
ax.set_zlabel("Potencia normalizada")
ax.set_title("KNN - Sensores electricos")
ax.legend()
plt.tight_layout()
plt.savefig("grafica_knn.png", dpi=300, bbox_inches="tight")
plt.close()

# ==========================================
# 7.1 COMPARACION ESTADISTICA DE TIEMPOS
# ==========================================
repeticiones = 1000
tiempos_knn = []
tiempos_kdtree = []
# El arbol se construye una sola vez
arbol_prueba = cKDTree(X_norm)
for _ in range(repeticiones):
    # KNN por fuerza bruta
    inicio = time.perf_counter()
    dist = np.linalg.norm(X_norm - punto, axis=1)
    dist[indice_anomalo] = np.inf
    vecinos = np.argsort(dist)[:k]
    tiempos_knn.append(
        (time.perf_counter() - inicio) * 1000
    )
 
    # Consulta kd-tree
    inicio = time.perf_counter()
    dist_tree, idx_tree = arbol_prueba.query(
        punto, k=k + 1
    )
    tiempos_kdtree.append(
        (time.perf_counter() - inicio) * 1000
    )
 
# Estadisticas
media_knn = np.mean(tiempos_knn)
media_tree = np.mean(tiempos_kdtree)
mediana_knn = np.median(tiempos_knn)
mediana_tree = np.median(tiempos_kdtree)
print("\nCOMPARACION ESTADISTICA")
print(f"Repeticiones: {repeticiones}")
print(f"Media KNN: {media_knn:.6f} ms")
print(f"Media kd-tree: {media_tree:.6f} ms")
print(f"Mediana KNN: {mediana_knn:.6f} ms")
print(f"Mediana kd-tree: {mediana_tree:.6f} ms")

# ==========================================
# 9. GRAFICA COMPARATIVA DE TIEMPOS
# ==========================================
fig, ax = plt.subplots(figsize=(8, 5))
metodos = ["KNN directo", "kd-tree"]
tiempos = [mediana_knn, mediana_tree]
barras = ax.bar(
    metodos,
    tiempos,
    color=["steelblue", "seagreen"],
    width=0.5
)
 
for barra, tiempo in zip(barras, tiempos):
    ax.text(
        barra.get_x() + barra.get_width() / 2,
        barra.get_height(),
        f"{tiempo:.5f} ms",
        ha="center",
        va="bottom"
    )
ax.set_ylabel("Tiempo de consulta (ms)")
ax.set_title("Comparacion KNN vs kd-tree - 1000 repeticiones")
ax.set_ylim(0, max(tiempos) * 1.2)
plt.tight_layout()
plt.savefig("comparacion_tiempos.png", dpi=300)
plt.close()
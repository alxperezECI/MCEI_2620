import pandas as pd
import numpy as np
from sklearn.neighbors import NearestNeighbors, KDTree
import time

# Cargar los datos de los 100 sensores
# Ajustar el delimitador y nombres de columnas según la estructura real de Datos_MCEI.csv
try:
    df = pd.read_csv("Datos_MCEI.csv", sep=";") 
    # Seleccionamos las 3 variables (Voltaje, Corriente, Temperatura/Potencia)
    X = df[['voltaje_V', 'corriente_A', 'potencia_W_aprox']].values
except FileNotFoundError:
    print("Error: El archivo 'Datos_MCEI.csv' no se encuentra.")
    exit()
except KeyError:
    # Si las columnas se llaman diferente, forzamos el uso de las 3 primeras columnas numéricas
    X = df.iloc[:, 1:4].values 

# Sensor anómalo reportado (Voltaje, Corriente, Temperatura)
anomalia = np.array([[240.5, 5.6, 85.2]])

# ==========================================
# Actividad 1: Método KNN (Fuerza Bruta)
# ==========================================
start_knn = time.perf_counter()
# Usamos algorithm='brute' para forzar el cálculo de distancia contra todos los puntos
knn = NearestNeighbors(n_neighbors=5, algorithm='brute')
knn.fit(X)
dist_knn, ind_knn = knn.kneighbors(anomalia)
time_knn = time.perf_counter() - start_knn

# ==========================================
# Actividad 2: Estructura kd-tree
# ==========================================
start_kdtree = time.perf_counter()
kdtree = KDTree(X)
dist_kdtree, ind_kdtree = kdtree.query(anomalia, k=5)
time_kdtree = time.perf_counter() - start_kdtree

# ==========================================
# Resultados
# ==========================================
print("\n" + "="*50)
print("--- 1. Resultados KNN (K-Nearest Neighbours) ---")
print(f"Sensores (Índices): {ind_knn[0]}")
print(f"Distancias: {[round(d, 2) for d in dist_knn[0]]}")
print(f"Tiempo de ejecución: {time_knn:.6f} segundos")

print("\n--- 2. Resultados kd-tree (Búsqueda Espacial) ---")
print(f"Sensores (Índices): {ind_kdtree[0]}")
print(f"Distancias: {[round(d, 2) for d in dist_kdtree[0]]}")
print(f"Tiempo de ejecución: {time_kdtree:.6f} segundos")
print("="*50 + "\n")
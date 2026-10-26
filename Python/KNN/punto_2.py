import time
import numpy as np
import pandas as pd
from sklearn.neighbors import NearestNeighbors
from sklearn.preprocessing import StandardScaler

# Cargar los datos del archivo CSV
df = pd.read_csv('Datos_MCEI.csv', sep=';')

print('--- Primeras filas del Dataset ---')
print(df.head())

# Seleccionar las características para el análisis KNN
# Variables: voltaje_V, corriente_A, potencia_W_aprox
X = df[['voltaje_V', 'corriente_A', 'potencia_W_aprox']].values

# Normalizar los datos (StandardScaler)
# Importante para que variables de mayor escala (como potencia o voltaje) no dominen sobre la corriente en el cálculo de distancias
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# Identificar el sensor anómalo el cual es el sensor con la caída de voltaje más extrema (mínimo voltaje)
anomaly_idx = df['voltaje_V'].idxmin()
sensor_anomalo = df.loc[anomaly_idx]

print('\nSensor anómalo detectado:')
print(f"Muestra ID: {sensor_anomalo['muestra']}")
print(f"Voltaje: {sensor_anomalo['voltaje_V']} V")
print(f"Corriente: {sensor_anomalo['corriente_A']} A")
print(f"Potencia Aprox: {sensor_anomalo['potencia_W_aprox']} W")

# Aplicar kd-tree para encontrar los 5 vecinos más cercanos (+ 1 porque el primero es el mismo sensor anómalo)
k = 5

# Inicio del cronómetro 
start_time = time.time()  # Medición de tiempo para el Punto 3

# Usamos algorithm='kd_tree' para construir la estructura espacial de búsqueda
nbrs = NearestNeighbors(n_neighbors=k + 1, algorithm='kd_tree', metric='euclidean')
nbrs.fit(X_scaled)

# Obtener distancias e índices del sensor anómalo
distances, indices = nbrs.kneighbors(X_scaled[anomaly_idx : anomaly_idx + 1])

end_time = time.time()
# Fin del cronómetro

execution_time = (end_time - start_time) * 1000  # en milisegundos

# Mostrar los resultados
print(f'\nLos {k} Vecinos Más Cercanos al Sensor Anómalo (kd-tree):')
vecinos_indices = indices[0][1:]
vecinos_distancias = distances[0][1:]

for i, (idx, dist) in enumerate(zip(vecinos_indices, vecinos_distancias), 1):
  sensor_vecino = df.loc[idx]
  print(f'\nVecino {i}:')
  print(f"  - Muestra ID: {sensor_vecino['muestra']}")
  print(f"  - Voltaje: {sensor_vecino['voltaje_V']} V")
  print(f"  - Corriente: {sensor_vecino['corriente_A']} A")
  print(f"  - Potencia Aprox: {sensor_vecino['potencia_W_aprox']} W")
  print(f'  - Distancia Euclidiana (Normalizada): {dist:.4f}')

print(f'\nTiempo de ejecución (kd-tree): {execution_time:.4f} ms')
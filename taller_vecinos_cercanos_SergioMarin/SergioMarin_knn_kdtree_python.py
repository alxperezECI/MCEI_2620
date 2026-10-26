#SERGIO ANDRÉS MARIN PATIÑO

import numpy as np 
import pandas as pd 
import time
import os
import matplotlib; matplotlib.use('Agg')
import matplotlib.pyplot as plt
from scipy.spatial import cKDTree

# 1) Cargar: CSV , separador ';', con encabezado
AQUI = os.path.dirname(os.path.abspath(__file__))
df = pd.read_csv(os.path.join(AQUI, "Datos_MCEI.csv"), sep=";")
m = df["muestra"].to_numpy(int)
X = df[["voltaje_V","corriente_A","potencia_W_aprox"]].to_numpy(float)
n = len(X)

# 2) Estandarizar.
mu, sd = X.mean(0), X.std(0, ddof=0)
Xz = (X - mu) / sd

# 3) Sensor ancla = anomalia en voltaje = |z_voltaje| maximo
zv = (X[:,0]-mu[0]) / sd[0]
a = int(np.argmax(np.abs(zv)))
k = 5
q = Xz[a]
print(f"Ancla: muestra {m[a]}  V={X[a,0]:.0f} I={X[a,1]:.0f} P={X[a,2]:.0f}  z_volt={zv[a]:.2f}")

# 4) KNN fuerza bruta
d = np.sqrt(((Xz - q)**2).sum(1))
nb_bruto = np.argsort(d); nb_bruto = nb_bruto[nb_bruto != a][:k]

# 5) kd-tree (scipy)
tree = cKDTree(Xz)
dist, idx = tree.query(q, k=k+1)        # +1: el primero es la propia ancla
nb_kd = idx[idx != a][:k]

print("KNN fuerza bruta -> muestras:", m[nb_bruto].tolist())
print("kd-tree          -> muestras:", m[nb_kd].tolist())
print("Vecinos identicos:", np.array_equal(np.sort(nb_bruto), np.sort(nb_kd)))

# 6) Tiempos
R=20000
t0=time.perf_counter()
for _ in range(R):
    dd=np.sqrt(((Xz-q)**2).sum(1)); np.argsort(dd)
tb=(time.perf_counter()-t0)/R*1e6
t0=time.perf_counter()
for _ in range(R): tree.query(q,k=k+1)
tk=(time.perf_counter()-t0)/R*1e6
t0=time.perf_counter(); cKDTree(Xz); tbuild=(time.perf_counter()-t0)*1e6
print(f"Tiempos us: bruto={tb:.1f}  kd_consulta={tk:.1f}  kd_construir={tbuild:.1f}")

# 7) Grafica 3D
fig=plt.figure(figsize=(7,6)); ax=fig.add_subplot(111,projection='3d')
ax.scatter(Xz[:,0],Xz[:,1],Xz[:,2],c='steelblue',s=20,label='sensores')
ax.scatter(Xz[a,0],Xz[a,1],Xz[a,2],c='red',s=90,label='ancla')
ax.scatter(Xz[nb_bruto,0],Xz[nb_bruto,1],Xz[nb_bruto,2],c='limegreen',s=60,label='5 vecinos')
ax.set_xlabel('voltaje (z)'); ax.set_ylabel('corriente (z)'); ax.set_zlabel('potencia (z)')
ax.legend(); plt.title('KNN sobre sensores estandarizados'); plt.tight_layout()
plt.savefig('py_knn_3d.png',dpi=120); plt.close()
print("Grafica: py_knn_3d.png")
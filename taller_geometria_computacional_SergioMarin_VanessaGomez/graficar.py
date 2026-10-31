import numpy as np
import matplotlib; matplotlib.use('Agg')
import matplotlib.pyplot as plt

P = np.genfromtxt("puntos.csv", delimiter=",", skip_header=1)
H = np.genfromtxt("hull.csv",   delimiter=",", skip_header=1)
E = np.genfromtxt("delaunay_edges.csv", delimiter=",", skip_header=1)

fig, ax = plt.subplots(figsize=(8,8))

# aristas Delaunay
for x1,y1,x2,y2 in E:
    ax.plot([x1,x2],[y1,y2], color="0.8", lw=0.6, zorder=1)
# puntos
ax.scatter(P[:,0], P[:,1], s=18, color="steelblue", zorder=3, label="sensores")
# envolvente (cerrar el poligono)
Hc = np.vstack([H, H[0]])
ax.plot(Hc[:,0], Hc[:,1], color="crimson", lw=2, zorder=2, label="envolvente convexa")

ax.set_aspect("equal"); ax.set_xlabel("x [m]"); ax.set_ylabel("y [m]")
ax.set_title("Nube de sensores: envolvente convexa y Delaunay")
ax.legend()
plt.tight_layout(); plt.savefig("figura_geometria.png", dpi=130)
print("Figura: figura_geometria.png")
import sys
import matplotlib.pyplot as plt

archivo = sys.argv[1] if len(sys.argv) > 1 else "datos.txt"
sec, pts, hull, edges = None, [], [], []
for linea in open(archivo):
    linea = linea.strip()
    if not linea:
        continue
    if linea.startswith("#"):
        sec = linea.split(":")[0].lstrip("# ").strip()
        continue
    v = list(map(float, linea.split()))
    {"points": pts, "convex_hull": hull, "delaunay_edges": edges}[sec].append(v)

fig, ax = plt.subplots(figsize=(7, 7))
for x1, y1, x2, y2 in edges:
    ax.plot([x1, x2], [y1, y2], color="0.6", lw=0.6, zorder=1)
ax.scatter([p[0] for p in pts], [p[1] for p in pts], s=10, color="tab:blue", zorder=2, label="Puntos")
hx = [p[0] for p in hull] + [hull[0][0]]
hy = [p[1] for p in hull] + [hull[0][1]]
ax.plot(hx, hy, color="tab:red", lw=2, zorder=3, label="Envolvente convexa")
ax.plot([], [], color="0.6", label="Aristas de Delaunay")
ax.set_aspect("equal")
ax.legend(loc="upper right")
ax.set_title(f"Nube, envolvente y Delaunay (n={len(pts)})")
plt.tight_layout()
plt.savefig(sys.argv[2] if len(sys.argv) > 2 else "figura.png", dpi=200)
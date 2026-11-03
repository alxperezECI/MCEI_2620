import csv
from pathlib import Path
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
 
BASE = Path(__file__).resolve().parent

RESULTADOS = BASE / "resultados"
 
 
def leer_csv(nombre):
    with open(RESULTADOS / nombre, newline="", encoding="utf-8") as archivo:
        return list(csv.DictReader(archivo))
 
 
puntos = leer_csv("puntos.csv")
hull = leer_csv("convex_hull.csv")
aristas = leer_csv("delaunay.csv")
 
x = [float(p["x"]) for p in puntos]
y = [float(p["y"]) for p in puntos]
hx = [float(p["x"]) for p in hull]
hy = [float(p["y"]) for p in hull]
 
# Cerrar el poligono de la envolvente convexa
if hx:
    hx.append(hx[0])
    hy.append(hy[0])
 
def configurar_grafica(titulo):
    plt.figure(figsize=(9, 7))
    plt.title(titulo)
    plt.xlabel("Coordenada X")
    plt.ylabel("Coordenada Y")
    plt.axis("equal")
    plt.grid(True, alpha=0.25)
 
# Figura 1: nube de puntos
configurar_grafica("Distribucion de puntos")

plt.scatter(x, y, s=12, color="royalblue")
plt.tight_layout()
plt.savefig(RESULTADOS / "01_puntos.png", dpi=200)
plt.close()
 
# Figura 2: envolvente convexa
configurar_grafica("Envolvente convexa - CGAL")

plt.scatter(x, y, s=12, color="royalblue", label="Puntos")
plt.plot(hx, hy, color="red", linewidth=2, label="Convex Hull")
plt.legend()
plt.tight_layout()
plt.savefig(RESULTADOS / "02_convex_hull.png", dpi=200)
plt.close()
 
# Figura 3: triangulacion de Delaunay

configurar_grafica("Triangulacion de Delaunay - CGAL")
for a in aristas:
    x1 = float(a["x1"])
    y1 = float(a["y1"])
    x2 = float(a["x2"])
    y2 = float(a["y2"])
 
    plt.plot([x1, x2], [y1, y2],
             color="gray", linewidth=0.6)
 
plt.scatter(x, y, s=12, color="royalblue")
plt.tight_layout()
plt.savefig(RESULTADOS / "03_delaunay.png", dpi=200)
plt.close()
 
print("Graficas generadas correctamente:")
print(" - resultados/01_puntos.png")
print(" - resultados/02_convex_hull.png")
print(" - resultados/03_delaunay.png")
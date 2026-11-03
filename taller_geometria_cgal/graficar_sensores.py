import csv
from pathlib import Path
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
 
BASE = Path(__file__).resolve().parent
DATOS = BASE / "datos"
RESULTADOS = BASE / "resultados" / "sensores"
RESULTADOS.mkdir(parents=True, exist_ok=True)
 
def leer_csv(ruta):
    with open(ruta, newline="", encoding="utf-8-sig") as archivo:
        return list(csv.DictReader(archivo))
 
 
# Coordenadas originales e identificadores
sensores = leer_csv(DATOS / "ubicaciones_100_sensores_xy.csv")
 
# Geometrias calculadas por CGAL
hull = leer_csv(RESULTADOS / "convex_hull.csv")
aristas = leer_csv(RESULTADOS / "delaunay.csv")
x = [float(s["x_m"]) for s in sensores]
y = [float(s["y_m"]) for s in sensores]
hx = [float(p["x"]) for p in hull]
hy = [float(p["y"]) for p in hull]
 
# Cerrar poligono convexo
if hx:
    hx.append(hx[0])
    hy.append(hy[0])
 
 
def configurar(titulo):
    plt.figure(figsize=(10, 8))
    plt.title(titulo)
    plt.xlabel("Coordenada X (m)")
    plt.ylabel("Coordenada Y (m)")
    plt.axis("equal")
    plt.grid(True, alpha=0.25)
 
# Figura 1: ubicacion de sensores
configurar("Distribucion espacial de 100 sensores")
plt.scatter(x, y, s=20, color="royalblue")
plt.tight_layout()
plt.savefig(RESULTADOS / "01_sensores.png", dpi=200)
plt.close()

# Figura 2: envolvente convexa
configurar("Envolvente convexa de los sensores - CGAL")
plt.scatter(
    x, y, s=20, color="royalblue", label="Sensores"
)
 
plt.plot(
    hx, hy, color="red", linewidth=2,
    label="Envolvente convexa"
)
 
plt.legend()
plt.tight_layout()
plt.savefig(RESULTADOS / "02_convex_hull_sensores.png", dpi=200)
plt.close()

# Figura 3: triangulacion de Delaunay
configurar("Triangulacion de Delaunay de los sensores - CGAL")
for a in aristas:
    x1 = float(a["x1"])
    y1 = float(a["y1"])
    x2 = float(a["x2"])
    y2 = float(a["y2"])
 
    plt.plot(
        [x1, x2], [y1, y2],
        color="gray", linewidth=0.6
    )
 
plt.scatter(x, y, s=20, color="royalblue")
plt.tight_layout()
plt.savefig(RESULTADOS / "03_delaunay_sensores.png", dpi=200)
plt.close()
 
# Figura 4: visualizacion conjunta
configurar("Analisis geometrico de ubicacion de sensores")
for a in aristas:
    plt.plot(
        [float(a["x1"]), float(a["x2"])],
        [float(a["y1"]), float(a["y2"])],
        color="gray", linewidth=0.5, alpha=0.7
    )
 
plt.scatter(
    x, y, s=20, color="royalblue",
    label="Sensores", zorder=3
)
 
plt.plot(
    hx, hy, color="red", linewidth=2,
    label="Envolvente convexa", zorder=4
)
 
plt.legend()
plt.tight_layout()
plt.savefig(RESULTADOS / "04_analisis_completo.png", dpi=200)
plt.close()
 
print("Visualizaciones generadas correctamente:")
for nombre in [
    "01_sensores.png",

    "02_convex_hull_sensores.png",

    "03_delaunay_sensores.png",

    "04_analisis_completo.png",

]:
    print(f" - resultados/sensores/{nombre}")
"""Grafica las medianas medidas y la proporcion de vertices extremos."""
import csv
from pathlib import Path
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

raiz = Path(__file__).resolve().parents[2]
entrada = raiz / "datos/cgal/resumen_experimentos.csv"
salida = raiz / "Informe/cgal/figuras"
salida.mkdir(parents=True, exist_ok=True)

with entrada.open(newline="") as archivo:
    filas = list(csv.DictReader(archivo))

fig, ejes = plt.subplots(1, 3, figsize=(15, 4.8))
semillas = sorted({int(f["semilla"]) for f in filas})

for semilla in semillas:
    grupo = sorted(
        (f for f in filas if int(f["semilla"]) == semilla),
        key=lambda f: int(f["n"])
    )
    n = [int(f["n"]) for f in grupo]

    for ax, columna in zip(
        ejes,
        ("hull_mediana_ms", "delaunay_mediana_ms", "porcentaje_hull")
    ):
        valores = [float(f[columna]) for f in grupo]
        ax.plot(n, valores, marker="o", label=f"Semilla {semilla}")

titulos = (
    "Tiempo de envolvente convexa",
    "Tiempo de triangulación de Delaunay",
    "Puntos que son vértices de la envolvente"
)
for ax, titulo in zip(ejes, titulos):
    ax.set_title(titulo, fontsize=11)
    ax.set_xlabel("Número de puntos")
    ax.set_xscale("log")
    ax.set_xticks([100, 500, 1000, 5000])
    ax.set_xticklabels(["100", "500", "1000", "5000"])
    ax.grid(alpha=0.25)
    ax.legend(fontsize=8)

for ax in ejes[:2]:
    ax.set_ylabel("Mediana de 5 ejecuciones (ms)")
    ax.set_yscale("log")

ejes[2].set_ylabel("Porcentaje (%)")
fig.tight_layout()
archivo = salida / "comparacion_experimentos.png"
fig.savefig(archivo, dpi=180, bbox_inches="tight")
plt.close(fig)
print("Gráfica guardada en:", archivo)

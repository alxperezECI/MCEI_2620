"""Visualiza los resultados exportados por el programa C++ con CGAL."""
from pathlib import Path
import argparse
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection

raiz = Path(__file__).resolve().parents[2]
parser = argparse.ArgumentParser()
parser.add_argument("caso", nargs="?", default="n100_s42")
args = parser.parse_args()

carpeta = raiz / "datos" / "cgal" / args.caso
salida = raiz / "Informe" / "cgal" / "figuras"
salida.mkdir(parents=True, exist_ok=True)

puntos = np.loadtxt(carpeta / "puntos.csv", delimiter=",", skiprows=1,
                    ndmin=2)
envolvente = np.loadtxt(carpeta / "envolvente.csv", delimiter=",",
                        skiprows=1, ndmin=2)
aristas = np.loadtxt(carpeta / "aristas.csv", delimiter=",", skiprows=1,
                     ndmin=2)

fig, ax = plt.subplots(figsize=(9, 8))
segmentos = aristas.reshape(-1, 2, 2)
ax.add_collection(LineCollection(
    segmentos, colors="steelblue", linewidths=0.6,
    alpha=0.65, label="Aristas de Delaunay"
))

contorno = np.vstack([envolvente, envolvente[0]])
ax.plot(contorno[:, 0], contorno[:, 1], color="crimson",
        linewidth=2, label="Envolvente convexa")
ax.scatter(puntos[:, 0], puntos[:, 1], s=18, color="black",
           zorder=3, label=f"Puntos ({len(puntos)})")

ax.set(
    title=f"Envolvente convexa y triangulación de Delaunay\n{args.caso}",
    xlabel="Coordenada x",
    ylabel="Coordenada y"
)
ax.set_aspect("equal", adjustable="box")
ax.margins(0.05)
ax.grid(alpha=0.2)
ax.legend(loc="upper left", bbox_to_anchor=(0, -0.08), ncol=2)
fig.tight_layout()

archivo = salida / f"{args.caso}.png"
fig.savefig(archivo, dpi=180, bbox_inches="tight")
plt.close(fig)
print("Gráfica guardada en:", archivo)

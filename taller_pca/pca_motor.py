import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from pathlib import Path

ruta = Path(__file__).resolve().parent
resultados = ruta / "resultados"
resultados.mkdir(exist_ok=True)

X = np.array([

    [48, 5.1, 2.1],
    [50, 5.3, 2.4],
    [51, 5.4, 2.5],
    [53, 5.7, 2.8],
    [55, 5.8, 3.0],
    [56, 6.0, 3.1],
    [58, 6.2, 3.5],
    [60, 6.3, 3.6],
    [61, 6.5, 3.8],
    [63, 6.7, 4.0]
], dtype=float)

np.set_printoptions(precision=6, suppress=True)
media = np.mean(X, axis=0)
desviacion = np.std(X, axis=0, ddof=1)
Z = (X - media) / desviacion
S = np.cov(Z, rowvar=False)
autovalores, autovectores = np.linalg.eigh(S)
orden = np.argsort(autovalores)[::-1]
autovalores = autovalores[orden]
autovectores = autovectores[:, orden]

for j in range(3):

    i = np.argmax(np.abs(autovectores[:, j]))

    if autovectores[i, j] < 0:

        autovectores[:, j] *= -1

varianza = autovalores / np.sum(autovalores) * 100

acumulada = np.cumsum(varianza)

proyeccion = Z @ autovectores[:, :2]

print("\nMATRIZ ORIGINAL")

print(X)

print("\nMEDIAS")

print(media)

print("\nDESVIACIONES ESTANDAR")

print(desviacion)

print("\nMATRIZ ESTANDARIZADA")

print(Z)

print("\nMATRIZ DE COVARIANZA")

print(S)

print("\nAUTOVALORES")

print(autovalores)

print("\nAUTOVECTORES")

print(autovectores)

print("\nSUMA AUTOVALORES")

print(np.sum(autovalores))

print("\nTRAZA COVARIANZA")

print(np.trace(S))

print("\nVARIANZA EXPLICADA (%)")

print(varianza)

print("\nVARIANZA ACUMULADA (%)")

print(acumulada)

print("\nPROYECCION PC1-PC2")

print(proyeccion)

assert np.allclose(np.sum(autovalores), np.trace(S))

fig, ax = plt.subplots(figsize=(9, 6))

ax.scatter(proyeccion[:, 0], proyeccion[:, 1], s=75)

for i, (x, y) in enumerate(proyeccion, 1):

    ax.annotate(str(i), (x, y), xytext=(6, 6),

                textcoords="offset points")

ax.set_xlabel(f"PC1 ({varianza[0]:.4f}%)")

ax.set_ylabel(f"PC2 ({varianza[1]:.4f}%)")

ax.set_title("Proyeccion PCA - Motor electrico")

ax.grid(alpha=0.3)

fig.tight_layout()

fig.savefig(resultados / "proyeccion_pca.png", dpi=200)

plt.close(fig)

fig, ax = plt.subplots(figsize=(9, 6))

ax.bar(["PC1", "PC2", "PC3"], varianza)

ax.plot(["PC1", "PC2", "PC3"], acumulada,

        marker="o", label="Varianza acumulada")

ax.set_ylabel("Varianza explicada (%)")

ax.set_title("Varianza explicada por PCA")

ax.set_ylim(0, 110)

ax.legend()

ax.grid(axis="y", alpha=0.3)

fig.tight_layout()

fig.savefig(resultados / "varianza_explicada.png", dpi=200)

plt.close(fig)

print("\nRESULTADO FINAL")
print(f"PC1: {varianza[0]:.4f}%")
print(f"PC1 + PC2: {acumulada[1]:.4f}%")
print(f"Varianza descartada: {varianza[2]:.4f}%")
print("\nGraficas generadas correctamente.")
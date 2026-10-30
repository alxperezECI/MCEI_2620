"""Ejecuta CGAL y registra tiempos y reproducibilidad."""
import csv
import hashlib
import subprocess
from pathlib import Path
from statistics import median

raiz = Path(__file__).resolve().parents[2]
ejecutable = raiz / "C_C++/cgal/build/taller_cgal"
destino = raiz / "datos/cgal"
filas = []
resumen = []

def huella(carpeta):
    return tuple(
        hashlib.sha256((carpeta / nombre).read_bytes()).hexdigest()
        for nombre in ("puntos.csv", "envolvente.csv", "aristas.csv")
    )

for n in (100, 500, 1000, 5000):
    for semilla in (42, 123, 2026):
        carpeta = destino / f"n{n}_s{semilla}"

        # Ejecucion previa, excluida de las mediciones registradas.
        comando = [str(ejecutable), str(n), str(semilla), str(carpeta)]
        subprocess.run(comando, check=True, capture_output=True, text=True)
        referencia = huella(carpeta)
        grupo = []

        for repeticion in range(1, 6):
            subprocess.run(comando, check=True, capture_output=True, text=True)

            with (carpeta / "resumen.csv").open(newline="") as archivo:
                fila = next(csv.DictReader(archivo))

            assert fila["valida"] == "1", "Triangulacion invalida"
            assert fila["euler"] == "1", "Fallo en relacion de Euler"
            assert huella(carpeta) == referencia, "Cambio en la geometria"

            fila["repeticion"] = repeticion
            fila["geometria_reproducible"] = 1
            filas.append(fila)
            grupo.append(fila)

        ultima = grupo[-1]
        resultado = {
            "n": n,
            "semilla": semilla,
            "vertices_hull": int(ultima["vertices_hull"]),
            "porcentaje_hull": 100 * int(ultima["vertices_hull"]) / n,
            "vertices": int(ultima["vertices"]),
            "aristas_finitas": int(ultima["aristas_finitas"]),
            "caras_finitas": int(ultima["caras_finitas"]),
            "hull_mediana_ms": median(float(f["hull_ms"]) for f in grupo),
            "delaunay_mediana_ms": median(
                float(f["delaunay_ms"]) for f in grupo
            ),
        }
        resumen.append(resultado)
        print(
            f"n={n:4d}, semilla={semilla:4d}: "
            f"hull={resultado['vertices_hull']:2d}, "
            f"envolvente={resultado['hull_mediana_ms']:.6f} ms, "
            f"Delaunay={resultado['delaunay_mediana_ms']:.6f} ms"
        )

for nombre, registros in (
    ("mediciones.csv", filas),
    ("resumen_experimentos.csv", resumen),
):
    with (destino / nombre).open("w", newline="") as archivo:
        escritor = csv.DictWriter(archivo, fieldnames=list(registros[0]))
        escritor.writeheader()
        escritor.writerows(registros)

print("\n60 mediciones guardadas; validacion y reproducibilidad correctas.")
print("Los CSV geometricos de cada caso corresponden a su ultima ejecucion.")

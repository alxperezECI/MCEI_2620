"""Reconstruccion hipotetica de decimales; conserva el CSV original."""
import csv
import math
from pathlib import Path

raiz = Path(__file__).resolve().parents[2]
carpeta = raiz / "datos" / "knn"

def interpretar(texto, referencia):
    valor = float(texto)
    if valor <= 0:
        raise ValueError("Esta regla requiere valores positivos")
    candidatos = [(valor / 10**n, n) for n in range(5)]
    return min(
        candidatos,
        key=lambda par: abs(math.log(par[0] / referencia))
    )

with (carpeta / "Datos_MCEI_original.csv").open(
    encoding="utf-8-sig", newline=""
) as archivo:
    filas = list(csv.DictReader(archivo, delimiter=";"))

salida = []
registro = []
for fila in filas:
    identificador = int(fila["muestra"])
    v, nv = interpretar(fila["voltaje_V"], 120.0)
    i, ni = interpretar(fila["corriente_A"], 5.0)
    p, np_ = interpretar(fila["potencia_W_aprox"], v * i)
    salida.append([identificador, v, i, p])
    registro.append([identificador, nv, ni, np_])

for nombre, encabezado, contenido in [
    ("sensores_interpretados.csv",
     ["muestra", "voltaje_V", "corriente_A", "potencia_W_aprox"],
     salida),
    ("registro_conversion.csv",
     ["muestra", "exponente_divisor_V", "exponente_divisor_I",
      "exponente_divisor_P"], registro)
]:
    with (carpeta / nombre).open("w", newline="", encoding="utf-8") as f:
        escritor = csv.writer(f)
        escritor.writerow(encabezado)
        escritor.writerows(contenido)

print("Registros interpretados:", len(salida))
print("Primero:", salida[0])
print("Ultimo:", salida[-1])
print("Rango de voltaje:", min(r[1] for r in salida),
      max(r[1] for r in salida))
print("ADVERTENCIA: decimales reconstruidos bajo supuestos.")

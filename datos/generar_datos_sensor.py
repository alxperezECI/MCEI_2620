import numpy as np

# ============================================================
# GENERACION DE DATOS DEL SENSOR
# Taller de integracion y diferenciacion numerica
# Jimmy Alexander Calderon Trujillo
# Sebastian Barahona
# ============================================================

# 50 mediciones igualmente espaciadas
i = np.arange(50)

# xi = 0.2*i
x = 0.2 * i

# Ecuacion utilizada UNICAMENTE para generar los datos
y = (
    2.0
    + 0.35 * np.sin(0.7 * x)
    + 0.15 * np.cos(2.1 * x)
    + 0.03 * x
)

# Guardar archivo CSV
datos = np.column_stack((x, y))

np.savetxt(
    "datos/datos_sensor.csv",
    datos,
    delimiter=",",
    header="x,y",
    comments="",
    fmt="%.10f"
)

print("Archivo datos_sensor.csv generado correctamente.")
print(f"Numero de mediciones: {len(x)}")
print(f"x inicial: {x[0]:.1f}")
print(f"x final: {x[-1]:.1f}")
print(f"Espaciado: {x[1] - x[0]:.1f}")


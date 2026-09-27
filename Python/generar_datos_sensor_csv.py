import numpy as np
 
# Generacion de los 50 datos del sensor
i = np.arange(50)
 
x = 0.2 * i
 
y = (
    2.0
    + 0.35 * np.sin(0.7 * x)
    + 0.15 * np.cos(2.1 * x)
    + 0.03 * x
)
 
# Guardar archivo CSV
datos = np.column_stack((x, y))
 
np.savetxt(
    "datos_sensor.csv",
    datos,
    delimiter=",",
    header="x,y",
    comments="",
    fmt="%.10f"
)
 
print("Archivo datos_sensor.csv generado correctamente.")
print(f"Numero de datos: {len(x)}")
print(f"x inicial: {x[0]:.1f}")
print(f"x final:   {x[-1]:.1f}")
print(f"Paso h:    {x[1] - x[0]:.1f}")
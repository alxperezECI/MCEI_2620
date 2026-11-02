import numpy as np

i = np.arange(51)
t = 0.2 * i

x = 0.08*t**2 + 0.40*np.sin(0.45*t)
y = 0.50*t + 0.30*(1 - np.cos(0.45*t))

datos = np.column_stack([t, x, y])

np.savetxt("trayectoria_robot.csv", datos,
           delimiter=",", header="t,x,y", comments="")

print("Archivo trayectoria_robot.csv generado correctamente.")
print("Numero de filas:", len(t))
print("Columnas: t, x, y")
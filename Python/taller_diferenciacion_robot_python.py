import numpy as np

import matplotlib.pyplot as plt

import time
 
# Taller de diferenciacion numerica

# Cinematica diferencial de un robot

# Parte 2 - Python / NumPy
 
# Medicion del tiempo de ejecucion

inicio = time.perf_counter()
 
# Cargar los mismos datos utilizados en Octave

data = np.loadtxt(

    "../trayectoria_robot.csv",

    delimiter=",",

    skiprows=1

)
 
t, x, y = data[:, 0], data[:, 1], data[:, 2]
 
# Derivadas de la posicion

# np.gradient utiliza diferencias centrales en puntos interiores

# y aproximaciones unilaterales en los extremos

vx = np.gradient(x, t)

vy = np.gradient(y, t)
 
# Velocidad lineal

v = np.sqrt(vx**2 + vy**2)
 
# Orientacion instantanea

theta = np.arctan2(vy, vx)
 
# Desenvolvimiento angular

theta = np.unwrap(theta)
 
# Velocidad angular

omega = np.gradient(theta, t)
 
# Tiempo de ejecucion del calculo

fin = time.perf_counter()
 
# Mostrar informacion

print(f"Numero de muestras: {len(t)}")

print(f"Paso temporal h: {t[1] - t[0]:.2f} s")

print(f"Velocidad inicial: {v[0]:.6f} m/s")

print(f"Velocidad final: {v[-1]:.6f} m/s")

print(f"Omega inicial: {omega[0]:.6f} rad/s")

print(f"Omega final: {omega[-1]:.6f} rad/s")

print(f"Tiempo de ejecucion: {fin - inicio:.6e} s")
 
# Graficas

fig, ax = plt.subplots(2, 2, figsize=(10, 8))
 
# Trayectoria

ax[0, 0].plot(x, y)

ax[0, 0].grid()

ax[0, 0].set_xlabel("x [m]")

ax[0, 0].set_ylabel("y [m]")

ax[0, 0].set_title("Trayectoria del robot")
 
# Velocidad lineal

ax[0, 1].plot(t, v)

ax[0, 1].grid()

ax[0, 1].set_xlabel("t [s]")

ax[0, 1].set_ylabel("v [m/s]")

ax[0, 1].set_title("Velocidad lineal")
 
# Orientacion

ax[1, 0].plot(t, theta)

ax[1, 0].grid()

ax[1, 0].set_xlabel("t [s]")

ax[1, 0].set_ylabel("theta [rad]")

ax[1, 0].set_title("Orientacion")
 
# Velocidad angular

ax[1, 1].plot(t, omega)

ax[1, 1].grid()

ax[1, 1].set_xlabel("t [s]")

ax[1, 1].set_ylabel("omega [rad/s]")

ax[1, 1].set_title("Velocidad angular")
 
plt.tight_layout()
 
# Guardar grafica

plt.savefig(

    "diferenciacion_robot_python.png",

    dpi=300,

    bbox_inches="tight"

)
 
print("Grafica guardada en diferenciacion_robot_python.png")
 
plt.show()
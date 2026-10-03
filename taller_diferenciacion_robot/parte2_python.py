import numpy as np
import matplotlib
matplotlib.use("Agg")   # para que funcione sin entorno gráfico (WSL, SSH, etc.)
import matplotlib.pyplot as plt


data = np.loadtxt("trayectoria_robot.csv", delimiter=",", skiprows=1)
t, x, y = data[:, 0], data[:, 1], data[:, 2]


vx = np.gradient(x, t)
vy = np.gradient(y, t)


v = np.sqrt(vx**2 + vy**2)

theta = np.arctan2(vy, vx)
theta_u = np.unwrap(theta)


omega = np.gradient(theta_u, t)


plt.figure()
plt.plot(x, y, "b-")
plt.axis("equal")
plt.xlabel("x [m]")
plt.ylabel("y [m]")
plt.title("Trayectoria del robot")
plt.grid(True)
plt.savefig("resultados/trayectoria.png", dpi=150)

plt.figure()
plt.plot(t, v, "r-")
plt.xlabel("t [s]")
plt.ylabel("v [m/s]")
plt.title("Velocidad lineal")
plt.grid(True)
plt.savefig("resultados/v.png", dpi=150)

plt.figure()
plt.plot(t, theta_u, "g-")
plt.xlabel("t [s]")
plt.ylabel("theta [rad]")
plt.title("Orientacion (unwrap)")
plt.grid(True)
plt.savefig("resultados/theta.png", dpi=150)

plt.figure()
plt.plot(t, omega, "m-")
plt.xlabel("t [s]")
plt.ylabel("omega [rad/s]")
plt.title("Velocidad angular")
plt.grid(True)
plt.savefig("resultados/omega.png", dpi=150)

print("v: min =", v.min(), " max =", v.max())
print("omega: min =", omega.min(), " max =", omega.max())
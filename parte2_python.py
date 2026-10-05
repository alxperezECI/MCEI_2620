import numpy as np
import matplotlib.pyplot as plt

# 1. Cargar los datos desde el CSV
data = np.loadtxt("trayectoria_robot.csv", delimiter=",", skiprows=1)
t = data[:, 0]
x = data[:, 1]
y = data[:, 2]

# 2. Estimación de vx e vy con np.gradient (diferencias centradas en el interior)
vx = np.gradient(x, t)
vy = np.gradient(y, t)

# 3. Velocidad lineal total
v = np.sqrt(vx**2 + vy**2)

# 4. Orientación y desenvolvimiento angular (unwrap)
theta = np.unwrap(np.arctan2(vy, vx))

# 5. Velocidad angular
omega = np.gradient(theta, t)

# 6. Generación de las 4 gráficas requeridas
fig, axs = plt.subplots(2, 2, figsize=(11, 7))

axs[0, 0].plot(x, y, 'b-', linewidth=1.5)
axs[0, 0].set_title("Trayectoria del Robot (y vs x)")
axs[0, 0].set_xlabel("x [m]")
axs[0, 0].set_ylabel("y [m]")
axs[0, 0].grid(True)

axs[0, 1].plot(t, v, 'r-', linewidth=1.5)
axs[0, 1].set_title("Velocidad Lineal v(t)")
axs[0, 1].set_xlabel("t [s]")
axs[0, 1].set_ylabel("v [m/s]")
axs[0, 1].grid(True)

axs[1, 0].plot(t, theta, 'g-', linewidth=1.5)
axs[1, 0].set_title(r"Orientación $\theta(t)$")
axs[1, 0].set_xlabel("t [s]")
axs[1, 0].set_ylabel(r"$\theta$ [rad]")
axs[1, 0].grid(True)

axs[1, 1].plot(t, omega, 'm-', linewidth=1.5)
axs[1, 1].set_title(r"Velocidad Angular $\omega(t)$")
axs[1, 1].set_xlabel("t [s]")
axs[1, 1].set_ylabel(r"$\omega$ [rad/s]")
axs[1, 1].grid(True)

plt.tight_layout()
plt.savefig("graficas_python.png")
plt.show()


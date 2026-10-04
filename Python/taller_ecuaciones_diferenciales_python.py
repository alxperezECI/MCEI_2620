import numpy as np

import matplotlib.pyplot as plt

import time
 
# Taller - Ecuaciones diferenciales

# Sistema masa-resorte-amortiguador

# Metodo de Runge-Kutta de cuarto orden (RK4)
 
# Parametros del sistema

m = 20.0

k = 20.0
 
# Coeficientes de amortiguamiento

c_valores = [5.0, 40.0, 200.0]
 
# Condiciones iniciales

x0 = 1.0

v0 = 0.0
 
# Intervalo de tiempo

t0 = 0.0

tf = 15.0

h = 0.01
 
t = np.arange(t0, tf + h, h)

N = len(t)
 
# Matriz para almacenar desplazamientos

X = np.zeros((N, len(c_valores)))
 
inicio = time.perf_counter()
 
# Resolver para cada valor de amortiguamiento

for j, c in enumerate(c_valores):
 
    # Estado:

    # y[0] = desplazamiento x

    # y[1] = velocidad dx/dt

    y = np.zeros((2, N))
 
    # Condiciones iniciales

    y[:, 0] = [x0, v0]
 
    # Sistema de ecuaciones diferenciales

    def f(ti, yi):

        return np.array([

            yi[1],

            -(c/m)*yi[1] - (k/m)*yi[0]

        ])
 
    # Metodo RK4

    for i in range(N - 1):
 
        k1 = f(t[i], y[:, i])
 
        k2 = f(

            t[i] + h/2,

            y[:, i] + (h/2)*k1

        )
 
        k3 = f(

            t[i] + h/2,

            y[:, i] + (h/2)*k2

        )
 
        k4 = f(

            t[i] + h,

            y[:, i] + h*k3

        )
 
        y[:, i+1] = (

            y[:, i]

            + (h/6)*(k1 + 2*k2 + 2*k3 + k4)

        )
 
    X[:, j] = y[0, :]
 
fin = time.perf_counter()
 
tiempo_ejecucion = fin - inicio
 
# Mostrar resultados

print("Metodo: Runge-Kutta de cuarto orden (RK4)")

print(f"Masa m = {m:.2f} kg")

print(f"Constante k = {k:.2f} N/m")

print(f"Paso h = {h:.3f} s")

print(f"Numero de pasos = {N-1}")

print(f"Tiempo de ejecucion = {tiempo_ejecucion:.6f} s")

print()
 
for j, c in enumerate(c_valores):

    print(

        f"c = {c:.0f} N*s/m -> "

        f"x(15 s) = {X[-1,j]:.6f} m"

    )
 
# Grafica

plt.figure(figsize=(10, 6))
 
plt.plot(t, X[:, 0], label="c = 5 N s/m - Subamortiguado")

plt.plot(t, X[:, 1], label="c = 40 N s/m - Critico")

plt.plot(t, X[:, 2], label="c = 200 N s/m - Sobreamortiguado")
 
plt.grid()

plt.xlabel("Tiempo [s]")

plt.ylabel("Desplazamiento x [m]")

plt.title("Sistema masa-resorte-amortiguador - RK4")

plt.legend()
 
plt.tight_layout()
 
# Guardar grafica

plt.savefig(

    "masa_resorte_amortiguador_python.png",

    dpi=300,

    bbox_inches="tight"

)
 
print("\nGrafica guardada en masa_resorte_amortiguador_python.png")
 
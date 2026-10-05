import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp

# 1. Parámetros físicos del sistema
m = 20.0   # Masa [kg]
k = 20.0   # Constante del resorte [N/m]
x0 = 1.0   # Desplazamiento inicial [m]
v0 = 0.0   # Velocidad inicial [m/s]

t_span = (0, 15)
t_eval = np.linspace(0, 15, 301)

# 2. Sistema de EDOs de primer orden [dx/dt, dv/dt]
def sistema_masa_resorte(t, y, c, m, k):
    x, v = y
    dxdt = v
    dvdt = -(c / m) * v - (k / m) * x
    return [dxdt, dvdt]

# 3. Evaluación de los tres coeficientes de amortiguamiento c
casos = [
    (5, 'c = 5 N s/m (Subamortiguado)', 'blue'),
    (40, 'c = 40 N s/m (Amortiguamiento Crítico)', 'green'),
    (200, 'c = 200 N s/m (Sobreamortiguado)', 'red')
]

plt.figure(figsize=(10, 6))

# 4. Integración numérica mediante Runge-Kutta (solve_ivp)
for c, label, color in casos:
    sol = solve_ivp(
        sistema_masa_resorte,
        t_span,
        [x0, v0],
        args=(c, m, k),
        t_eval=t_eval,
        method='RK45'
    )
    plt.plot(sol.t, sol.y[0], label=label, color=color, linewidth=2)

plt.title('Respuesta Temporal del Sistema Masa-Resorte-Amortiguador ($0 < t < 15$ s)', fontsize=13)
plt.xlabel('Tiempo $t$ [s]', fontsize=11)
plt.ylabel('Desplazamiento $x(t)$ [m]', fontsize=11)
plt.axhline(0, color='black', linestyle='--', linewidth=0.8)
plt.grid(True, linestyle=':', alpha=0.7)
plt.legend(fontsize=10)
plt.tight_layout()

# 5. Guardar la figura producida
plt.savefig('sistema_masa_resorte.png', dpi=300)
plt.show()
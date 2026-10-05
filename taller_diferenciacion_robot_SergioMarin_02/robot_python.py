#SERGIO ANDRES MARIN PATIÑO 03/10/2026

import matplotlib; matplotlib.use('Agg')       # sin display en WSL
import numpy as np, matplotlib.pyplot as plt

data = np.loadtxt("trayectoria_robot.csv", delimiter=",", skiprows=1)
t, x, y = data[:,0], data[:,1], data[:,2]

vx = np.gradient(x, t)                          # central interior + O(h) extremos (edge_order=1)
vy = np.gradient(y, t)
v  = np.sqrt(vx**2 + vy**2)
theta = np.unwrap(np.arctan2(vy, vx))
omega = np.gradient(theta, t)

for nm, arr in [("trayectoria",(x,y)),("v",(t,v)),("theta",(t,theta)),("omega",(t,omega))]:
    plt.figure()
    if nm=="trayectoria": plt.plot(x,y,'-o'); plt.axis('equal'); plt.xlabel('x [m]'); plt.ylabel('y [m]')
    else: plt.plot(*arr); plt.xlabel('t [s]'); plt.ylabel(nm)
    plt.title(nm); plt.savefig(f"py_{nm}.png", dpi=120); plt.close()

print(f"Python: v(0)={v[0]:.6f} v(25)={v[25]:.6f} v(50)={v[50]:.6f}")


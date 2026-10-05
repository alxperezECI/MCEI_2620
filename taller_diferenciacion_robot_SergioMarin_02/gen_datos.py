#SERGIO ANDRES MARIN PATIÑO 03/10/2026

import numpy as np

t = 0.2*np.arange(51)                       # 51 muestras, h=0.2, t∈[0,10]
x = 0.08*t**2 + 0.40*np.sin(0.45*t)
y = 0.50*t  + 0.30*(1 - np.cos(0.45*t))

np.savetxt("trayectoria_robot.csv", np.c_[t, x, y],
           delimiter=",", header="t,x,y", comments="", fmt="%.5f")
print("OK:", len(t), "filas, h =", t[1]-t[0])
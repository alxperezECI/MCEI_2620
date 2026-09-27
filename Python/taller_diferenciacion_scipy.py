import numpy as np
 
# --------------------------------------------------

# Lectura de los datos experimentales

# --------------------------------------------------
 
data = np.loadtxt(

    "datos_sensor.csv",

    delimiter=",",

    skiprows=1

)
 
x = data[:, 0]

y = data[:, 1]
 
N = len(x)

h = x[1] - x[0]
 
# --------------------------------------------------

# Diferencia hacia adelante

# --------------------------------------------------
 
dy_adelante = np.full(N, np.nan)
 
dy_adelante[:-1] = (

    y[1:] - y[:-1]

) / h
 
 
# --------------------------------------------------

# Diferencia central

# --------------------------------------------------
 
dy_central = np.full(N, np.nan)
 
dy_central[1:-1] = (

    y[2:] - y[:-2]

) / (2*h)
 
 
# --------------------------------------------------

# Resultados

# --------------------------------------------------
 
print(f"Numero de datos = {N}")

print(f"Paso h          = {h:.4f}\n")
 
print("x       Adelante        Central")

print("---------------------------------")
 
for i in range(N):

    print(

        f"{x[i]:.2f}    "

        f"{dy_adelante[i]:.6f}    "

        f"{dy_central[i]:.6f}"

    )
 
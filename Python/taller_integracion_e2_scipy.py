import numpy as np

from scipy.integrate import trapezoid, simpson

import time
 
 
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

# Verificacion de los datos

# --------------------------------------------------
 
equiespaciados = np.allclose(np.diff(x), h)

faltantes = np.isnan(data).any()
 
print(f"Numero de datos       = {N}")

print(f"Paso h                = {h:.4f}")

print(f"Datos equiespaciados  = {equiespaciados}")

print(f"Valores faltantes     = {faltantes}")
 
 
# --------------------------------------------------

# Regla del trapecio

# --------------------------------------------------
 
inicio = time.perf_counter()
 
I_trap = trapezoid(y, x=x)
 
fin = time.perf_counter()
 
tiempo_trap = fin - inicio
 
 
# --------------------------------------------------

# Regla de Simpson mediante SciPy

# --------------------------------------------------
 
inicio = time.perf_counter()
 
I_simp = simpson(y, x=x)
 
fin = time.perf_counter()
 
tiempo_simp = fin - inicio
 
 
# --------------------------------------------------

# Comparacion

# --------------------------------------------------
 
diferencia = abs(I_simp - I_trap)
 
diferencia_relativa = (

    diferencia / abs(I_trap)

) * 100
 
 
print("\nResultados de integracion")
 
print(f"Integral trapecio     = {I_trap:.12f}")

print(f"Integral Simpson      = {I_simp:.12f}")
 
print(f"\nDiferencia absoluta   = {diferencia:.12e}")

print(f"Diferencia relativa   = {diferencia_relativa:.6f} %")
 
print(f"\nTiempo trapecio       = {tiempo_trap:.6e} s")

print(f"Tiempo Simpson        = {tiempo_simp:.6e} s")
 
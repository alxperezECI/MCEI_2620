import numpy as np

from scipy.integrate import quad

import time
 
 
# Funcion del problema

def f(x):

    return np.exp(-0.4*x) * (1 + 0.5*np.sin(3*x))
 
 
# Primitiva analitica

def F(x):

    return (

        -2.5*np.exp(-0.4*x)

        + (0.5*np.exp(-0.4*x)/9.16)

        * (-0.4*np.sin(3*x) - 3*np.cos(3*x))

    )
 
 
# Intervalo de integracion

a = 0.0

b = 8.0
 
# Valor analitico de referencia

I_ref = F(b) - F(a)
 
print(f"Valor de referencia = {I_ref:.12f}")
 
 
# Tolerancias para scipy.integrate.quad

epsabs = 1e-10

epsrel = 1e-10
 
 
# Medicion del tiempo

inicio = time.perf_counter()
 
I_quad, error_estimado = quad(

    f,

    a,

    b,

    epsabs=epsabs,

    epsrel=epsrel

)
 
fin = time.perf_counter()
 
tiempo = fin - inicio
 
# Error respecto a la referencia analitica

error_real = abs(I_quad - I_ref)
 
 
print("\nCuadratura adaptativa SciPy")

print(f"Integral SciPy        = {I_quad:.12f}")

print(f"Error estimado        = {error_estimado:.3e}")

print(f"Error vs referencia   = {error_real:.3e}")

print(f"Tolerancia absoluta   = {epsabs:.3e}")

print(f"Tolerancia relativa   = {epsrel:.3e}")

print(f"Tiempo                = {tiempo:.6e} s")
 
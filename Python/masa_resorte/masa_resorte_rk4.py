"""Ejecutar: python Python/masa_resorte/masa_resorte_rk4.py"""
from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

M, K, TF, H = 20.0, 20.0, 15.0, 0.01
CASOS = {5: 'Subamortiguado', 40: 'Crítico', 200: 'Sobreamortiguado'}


def sistema(y, c):
    x, v = y
    return np.array([v, -(c / M) * v - (K / M) * x])


def resolver(c, h=H):
    t = np.linspace(0, TF, int(round(TF / h)) + 1)
    y = np.zeros((len(t), 2))
    y[0] = [1, 0]
    for n in range(len(t) - 1):
        dt = t[n + 1] - t[n]
        k1 = sistema(y[n], c)
        k2 = sistema(y[n] + dt * k1 / 2, c)
        k3 = sistema(y[n] + dt * k2 / 2, c)
        k4 = sistema(y[n] + dt * k3, c)
        y[n + 1] = y[n] + dt * (k1 + 2*k2 + 2*k3 + k4) / 6
    return t, y


def exacta(t, c):
    a = c / (2*M)
    w0 = np.sqrt(K/M)
    if a < w0:
        w = np.sqrt(w0*w0 - a*a)
        return np.exp(-a*t) * (np.cos(w*t) + a/w*np.sin(w*t))
    if np.isclose(a, w0):
        return (1 + a*t)*np.exp(-a*t)
    r1, r2 = -a + np.sqrt(a*a-w0*w0), -a - np.sqrt(a*a-w0*w0)
    return (-r2*np.exp(r1*t) + r1*np.exp(r2*t))/(r1-r2)


def main():
    raiz = Path(__file__).resolve().parents[2]
    datos = raiz/'datos'/'masa_resorte'
    informe = raiz/'Informe'/'masa_resorte'
    datos.mkdir(parents=True, exist_ok=True)
    informe.mkdir(parents=True, exist_ok=True)
    resultados, errores = {}, []
    fig, ax = plt.subplots(figsize=(10, 6))
    for c, nombre in CASOS.items():
        t, y = resolver(c)
        ref = exacta(t, c)
        err = abs(y[:, 0]-ref)
        tf, yf = resolver(c, H/2)
        err_fino = max(abs(yf[:, 0]-exacta(tf, c)))
        resultados[c] = y
        errores.append(f'| {c} | {max(err):.6e} | {err_fino:.6e} |')
        np.savetxt(datos/f'resultados_c{c}.csv', np.column_stack((t, y, ref, err)),
                   delimiter=',', header='t_s,x_m,v_m_s,x_exacta_m,error_absoluto_m', comments='')
        ax.plot(t, y[:, 0], label=f'c={c}: {nombre}', linewidth=2)
        print(f'c={c}: error máximo = {max(err):.6e} m')
    ax.axhline(0, color='black', linewidth=.8)
    ax.set(xlabel='Tiempo [s]', ylabel='Desplazamiento [m]',
           title='Sistema masa–resorte amortiguado · RK4', xlim=(0, TF))
    ax.grid(alpha=.3)
    ax.legend()
    fig.tight_layout()
    fig.savefig(informe/'desplazamiento_rk4.png', dpi=200)
    plt.close(fig)
    filas = []
    for instante in [0, 1, 2, 5, 10, 15]:
        i = int(round(instante/H))
        valores = ' | '.join(f'{resultados[c][i,0]:.8f}' for c in CASOS)
        filas.append(f'| {instante} | {valores} |')
    texto = r'''# Sistema masa–resorte amortiguado

## Problema

Resolver $mx''+cx'+kx=0$ con m=20 kg, k=20 N/m y
c=5, 40 y 200 N·s/m. Condiciones iniciales: x(0)=1 m y v(0)=0 m/s.
Se calcula desde 0 hasta 15 s, incluyendo los extremos.

## Desarrollo y método

Definiendo v=x', se transforma la ecuación de segundo orden:

$$x'=v,\qquad v'=-x-\frac{c}{20}v.$$

Para el estado Y=[x,v], la función F(Y)=[v,-x-(c/20)v].
Se implementa Runge–Kutta clásico de cuarto orden directamente:

$$K_1=F(Y_n),\qquad K_2=F(Y_n+hK_1/2),$$
$$K_3=F(Y_n+hK_2/2),\qquad K_4=F(Y_n+hK_3),$$
$$Y_{n+1}=Y_n+\frac{h}{6}(K_1+2K_2+2K_3+K_4).$$

Se utiliza h=0.01 s: 1500 pasos y 1501 estados.
El sistema es autónomo, por lo que F no depende explícitamente del tiempo.
El error global de RK4 es de orden cuatro para soluciones suaves y pasos estables.
Se comprueba el resultado frente a la solución exacta y con h=0.005 s.

## Clasificación del amortiguamiento

$$c_{cr}=2\sqrt{mk}=40\ \mathrm{N\,s/m}.$$

| c (N·s/m) | c/c_cr | Tipo |
|---:|---:|---|
| 5 | 0.125 | Subamortiguado |
| 40 | 1 | Crítico |
| 200 | 5 | Sobreamortiguado |

## Gráfica

![Desplazamiento](desplazamiento_rk4.png)

## Resultados

| t (s) | x con c=5 (m) | x con c=40 (m) | x con c=200 (m) |
|---:|---:|---:|---:|
'''
    texto += '\n'.join(filas)
    texto += r'''

## Validación analítica

Para c=5, a=0.125 y w=sqrt(1-a²):

$$x(t)=e^{-at}[\cos(wt)+(a/w)\sin(wt)].$$

Para c=40:

$$x(t)=(1+t)e^{-t}.$$

Para c=200, r_1=-5+sqrt(24) y r_2=-5-sqrt(24):

$$x(t)=\frac{-r_2e^{r_1t}+r_1e^{r_2t}}{r_1-r_2}.$$

Estas expresiones satisfacen las condiciones iniciales.
La tabla muestra el máximo error absoluto del desplazamiento sobre cada malla.

| c (N·s/m) | Error con h=0.01 (m) | Error con h=0.005 (m) |
|---:|---:|---:|
'''
    texto += '\n'.join(errores)
    texto += '''

## Conclusiones

1. Con c=5 se presentan oscilaciones de amplitud decreciente. Los valores
   negativos indican que la masa cruza la posición de equilibrio.
2. Con c=40 el retorno no oscila y es más rápido que con c=200 para estas
   condiciones iniciales. Un amortiguamiento mayor no garantiza un retorno
   más rápido: el caso sobreamortiguado conserva un desplazamiento importante
   al terminar los 15 segundos.
3. RK4 calcula desplazamiento y velocidad simultáneamente. La solución exacta
   y la reducción del paso permiten verificar la precisión numérica.

## Reproducción

Desde la raíz del repositorio, con NumPy y Matplotlib instalados:

```bash
python Python/masa_resorte/masa_resorte_rk4.py
```

Se generan tres CSV en datos/masa_resorte, con tiempo, desplazamiento,
velocidad, posición exacta y error absoluto. La gráfica y este informe
se regeneran en Informe/masa_resorte.

## Fuente

Enunciado del sistema masa–resorte suministrado por el profesor.
'''
    (informe/'README.md').write_text(texto, encoding='utf-8')
    print('Listo: gráfica, tres CSV e informe generados.')


if __name__ == '__main__':
    main()

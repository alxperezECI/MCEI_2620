# Taller sobre integración y diferenciación numérica

**Métodos Computacionales para Ingeniería**  
**Escuela Colombiana de Ingeniería Julio Garavito**

**Estudiantes:**  
- Jimmy Alexander Calderon Trujillo  
- Sebastian Barahona  

---

## 1. Introducción

En este taller se estudiaron diferentes métodos de integración y diferenciación numérica utilizando tres entornos de programación: GNU Octave, C/C++ con GNU Scientific Library (GSL) y Python con NumPy/SciPy.

El objetivo fue comparar el comportamiento de los métodos cuando se dispone de una función matemática conocida y cuando únicamente se cuenta con un conjunto de mediciones experimentales. Para ello se analizaron aspectos como discretización, error numérico, tolerancias, costo computacional, representación de los datos y nivel de abstracción proporcionado por las diferentes bibliotecas.

---

## 2. Ejercicio 1. Integración de una función conocida

Se consideró la función

\[
f(x)=e^{-0.4x}\left(1+0.5\sin(3x)\right),
\qquad 0\leq x\leq 8.
\]

El objetivo fue calcular numéricamente

\[
I=\int_0^8 f(x)\,dx.
\]

### 2.1 Referencia analítica

Se utilizó como referencia una primitiva de la función:

\[
F(x)=
-2.5e^{-0.4x}
+
\frac{
0.5e^{-0.4x}
\left[-0.4\sin(3x)-3\cos(3x)\right]
}{
0.4^2+3^2
}.
\]

Por tanto,

\[
I_{\mathrm{ref}}=F(8)-F(0)
\]

y se obtuvo aproximadamente:

\[
\boxed{I_{\mathrm{ref}}=2.559824508302}
\]

La integral representa el área acumulada bajo la curva de la función en el intervalo entre 0 y 8. La función combina un decaimiento exponencial con una componente oscilatoria sinusoidal. Estas oscilaciones y la curvatura de la función influyen en la precisión de las aproximaciones obtenidas mediante discretizaciones poco refinadas.

---

## 3. Regla compuesta del trapecio

La regla del trapecio fue implementada explícitamente para:

\[
n=\{10,20,50,100,500,1000\}.
\]

Los resultados obtenidos fueron:

| n | Integral aproximada | Error absoluto |
|---:|---:|---:|
| 10 | 2.494072510186 | 6.575200e-02 |
| 20 | 2.544929716212 | 1.489479e-02 |
| 50 | 2.557502776878 | 2.321731e-03 |
| 100 | 2.559246207984 | 5.783003e-04 |
| 500 | 2.559801403459 | 2.310484e-05 |
| 1000 | 2.559818732303 | 5.775999e-06 |

Los resultados fueron reproducidos en Octave, C++ y Python.

Se observa que al incrementar el número de subintervalos disminuye sistemáticamente el error. Esto es coherente con el comportamiento esperado de la regla compuesta del trapecio, cuyo error global es de orden

\[
O(h^2).
\]

---

## 4. Integración adaptativa

### 4.1 Octave

En Octave se utilizó adicionalmente la función `integral`, obteniendo aproximadamente:

\[
I=2.559824508302.
\]

El resultado coincidió con la referencia analítica dentro de la precisión numérica utilizada.

### 4.2 C++ con GSL

En C++ se utilizó `gsl_integration_qag`, con:

- tolerancia absoluta: \(10^{-10}\),
- tolerancia relativa: \(10^{-10}\),
- regla Gauss-Kronrod,
- límite de subdivisiones: 1000.

El resultado obtenido fue:

\[
I_{\mathrm{GSL}}\approx2.559824508302.
\]

La estimación interna de error reportada por GSL fue aproximadamente:

\[
2.84\times10^{-14}.
\]

### 4.3 Python con SciPy

En Python se utilizó `scipy.integrate.quad` con tolerancias absoluta y relativa de \(10^{-10}\).

Se obtuvo:

\[
I_{\mathrm{SciPy}}\approx2.559824508302
\]

con una estimación interna de error de aproximadamente:

\[
1.46\times10^{-10}.
\]

---

## 5. Análisis del ejercicio 1

### 5.1 ¿Qué ocurre con el trapecio cuando aumenta n?

Al incrementar \(n\), disminuye el tamaño de paso \(h\) y los segmentos lineales representan mejor la curvatura de la función. El error pasó de aproximadamente \(6.58\times10^{-2}\) para \(n=10\) a \(5.78\times10^{-6}\) para \(n=1000\).

### 5.2 ¿Aumentar n equivale a reducir la tolerancia de un método adaptativo?

No. En el trapecio compuesto el refinamiento se realiza de manera uniforme en todo el intervalo. En un método adaptativo, el algoritmo subdivide preferentemente las regiones en las que estima que necesita mayor resolución para cumplir una tolerancia determinada.

Por tanto, ambos procedimientos pueden aumentar la precisión, pero distribuyen el esfuerzo computacional de manera diferente.

### 5.3 ¿Una mayor precisión implica necesariamente un mayor costo?

En general, exigir una mayor precisión puede requerir más evaluaciones de la función y, por tanto, aumentar el costo computacional. Sin embargo, el costo también depende del método utilizado y de las características de la función.

En este ejercicio los tiempos de ejecución fueron muy pequeños, por lo que pequeñas diferencias entre mediciones individuales de tiempo no deben interpretarse como evidencia suficiente de superioridad de un entorno.

### 5.4 Error estimado y error respecto a la referencia

El error reportado por GSL o SciPy es una estimación interna realizada por el algoritmo adaptativo. En cambio, el error respecto a la referencia se calcula mediante:

\[
E_{\mathrm{real}}
=
|I_{\mathrm{num}}-I_{\mathrm{ref}}|.
\]

Por esta razón, ambos valores no tienen que coincidir exactamente. El primero es una estimación realizada durante el proceso numérico y el segundo requiere conocer una referencia independiente.

---

# 6. Ejercicio 2. Integración de datos experimentales

Se generaron 50 mediciones igualmente espaciadas mediante

\[
x_i=0.2i,\qquad i=0,\ldots,49.
\]

Los valores fueron almacenados en:

`datos/datos_sensor.csv`

con dos columnas:

`x,y`

Una vez generado el archivo, los análisis posteriores utilizaron únicamente las mediciones almacenadas en el CSV y no la función utilizada originalmente para generar los datos.

Se verificó:

- Número de mediciones: 50.
- Valor inicial de x: 0.0.
- Valor final de x: 9.8.
- Espaciamiento: 0.2.
- Espaciamiento uniforme.
- Ausencia de datos faltantes.

---

## 7. Integración de los datos

### 7.1 Regla del trapecio

Los tres entornos produjeron:

\[
\boxed{I_{\mathrm{trap}}=21.190846339900}
\]

La coincidencia se debe a que los tres programas implementan la misma operación sobre el mismo conjunto de mediciones.

### 7.2 Simpson y condición sobre los subintervalos

Existen 50 mediciones, lo cual produce 49 subintervalos.

La regla compuesta estándar de Simpson 1/3 requiere un número par de subintervalos. Por esta razón, en la implementación explícita se aplicó Simpson sobre las primeras 49 mediciones, correspondientes a 48 subintervalos entre 0 y 9.6, y trapecio en el último intervalo entre 9.6 y 9.8.

Se obtuvo:

\[
I_{\mathrm{Simpson},[0,9.6]}
=
20.698381760973
\]

y

\[
I_{\mathrm{trap},[9.6,9.8]}
=
0.493571183340.
\]

Por tanto,

\[
\boxed{
I_{\mathrm{mixto}}
=
21.191952944313
}
\]

En Python, `scipy.integrate.simpson` aplicado directamente al conjunto completo produjo:

\[
I_{\mathrm{Simpson,SciPy}}
=
21.192113098332.
\]

---

## 8. Integración mediante interpolación en GSL

En C++ se analizaron dos estrategias.

La primera consistió en integrar directamente los datos mediante trapecios:

\[
I_{\mathrm{trap}}
=
21.190846339900.
\]

La segunda construyó una representación interpolada mediante un spline cúbico de GSL y posteriormente integró dicha representación.

Se obtuvo:

\[
I_{\mathrm{spline}}
=
21.191888257606.
\]

La diferencia absoluta entre ambas estrategias fue:

\[
|I_{\mathrm{spline}}-I_{\mathrm{trap}}|
=
0.001041917706.
\]

La diferencia conceptual es importante: el trapecio supone segmentos lineales entre las mediciones, mientras que el spline construye una representación cúbica continua entre los puntos. Por ello los resultados son cercanos, pero no necesariamente idénticos.

---

# 9. Diferenciación numérica

Sobre las mismas mediciones se implementaron dos aproximaciones.

## 9.1 Diferencia hacia adelante

\[
f'(x_i)
\approx
\frac{f(x_{i+1})-f(x_i)}{h}.
\]

## 9.2 Diferencia central

\[
f'(x_i)
\approx
\frac{f(x_{i+1})-f(x_{i-1})}{2h}.
\]

Las implementaciones en Octave, C++ y Python produjeron los mismos valores numéricos.

| x | y | Adelante | Central |
|---:|---:|---:|---:|
| 0.0 | 2.150000 | 0.209017 | No disponible |
| 0.2 | 2.191803 | 0.085202 | 0.147110 |
| 0.4 | 2.208844 | -0.011276 | 0.036963 |
| 0.6 | 2.206589 | -0.065108 | -0.038192 |
| 0.8 | 2.193567 | -0.069089 | -0.067099 |
| 1.0 | 2.179749 | -0.025335 | -0.047212 |
| 1.2 | 2.174682 | 0.055147 | 0.014906 |
| 1.4 | 2.185712 | 0.154434 | 0.104791 |
| 1.6 | 2.216599 | 0.250881 | 0.202657 |
| 1.8 | 2.266775 | 0.322968 | 0.286924 |

---

## 10. Tratamiento de los extremos

La diferencia hacia adelante necesita el punto \(x_{i+1}\). Por esta razón, con la fórmula utilizada no puede calcularse en el último punto \(x=9.8\).

La diferencia central necesita simultáneamente los puntos \(x_{i-1}\) y \(x_{i+1}\). Por ello no se calculó en los extremos \(x=0.0\) y \(x=9.8\).

Es posible utilizar fórmulas unilaterales específicas para tratar los extremos, pero en este taller se conservaron las expresiones solicitadas para mantener la misma metodología en los tres entornos.

---

## 11. Sensibilidad de la diferenciación al ruido

La diferenciación numérica suele ser más sensible al ruido que la integración.

En una diferencia finita se restan mediciones cercanas y posteriormente se divide el resultado por un valor pequeño \(h\). Una perturbación presente en las mediciones puede, por tanto, amplificarse en la estimación de la derivada.

La integración presenta un comportamiento diferente porque acumula información sobre un intervalo. Las perturbaciones locales pueden compensarse parcialmente durante la suma, haciendo que el proceso sea generalmente menos sensible al ruido que la diferenciación.

---

# 12. Comparación de los tres entornos

| Aspecto | GNU Octave | C/C++ con GSL | Python con NumPy/SciPy |
|---|---|---|---|
| Facilidad de uso | Alta para cálculo numérico y matricial | Requiere mayor cantidad de código y manejo explícito | Alta, con sintaxis compacta |
| Control del algoritmo | Intermedio | Alto | Intermedio-alto |
| Manejo de tolerancias | Disponible en funciones adaptativas | Explícito mediante parámetros de GSL | Explícito mediante parámetros de SciPy |
| Integración de funciones | `integral` y métodos implementados manualmente | GSL y métodos implementados manualmente | `quad` y métodos implementados manualmente |
| Integración de datos | `trapz` y métodos explícitos | Trapecio explícito e interpolación GSL | `trapezoid` y `simpson` |
| Estimación de error | Depende de la función utilizada | GSL entrega estimación en cuadratura adaptativa | `quad` entrega estimación de error |
| Control de memoria y estructuras | Mayor abstracción | Mayor control por parte del programador | Alta abstracción mediante NumPy/SciPy |
| Visualización | Integrada y directa | Requiere herramientas adicionales | Directa mediante Matplotlib |
| Desarrollo del código | Relativamente corto | Más detallado y explícito | Relativamente corto |
| Tiempo de ejecución observado | Muy pequeño para este problema | Muy pequeño para este problema | Muy pequeño para este problema |

No se establece una clasificación absoluta de rendimiento a partir de los tiempos medidos, debido al pequeño tamaño del problema y a la variabilidad propia de mediciones de ejecución tan cortas.

---

# 13. Reflexión final

## 13.1 Función conocida frente a datos experimentales

Cuando se conoce una función matemática es posible evaluarla en cualquier punto del dominio y utilizar directamente métodos de cuadratura adaptativa como los disponibles en GSL o SciPy.

Cuando únicamente se dispone de mediciones experimentales, la situación cambia. La información existe solamente en los puntos medidos y es necesario decidir cómo representar el comportamiento entre ellos. Algunas alternativas son asumir segmentos lineales, aplicar reglas como trapecio o Simpson cuando las condiciones lo permiten, o construir una función interpolada como un spline.

Por esta razón, la representación de los datos forma parte del problema numérico y puede influir directamente en el resultado.

## 13.2 Discretización

La discretización transforma un problema continuo en un conjunto finito de puntos. El tamaño de paso determina cuánta información se conserva sobre el comportamiento de la función.

En el ejercicio 1 se observó directamente que una discretización más fina disminuyó el error de la regla del trapecio.

## 13.3 Error y tolerancia

El error mide la diferencia entre una aproximación y un valor de referencia cuando este se conoce.

La tolerancia, en cambio, constituye un criterio utilizado por los métodos adaptativos para decidir cuándo la aproximación interna es suficientemente precisa.

Por ello, tolerancia y error real son conceptos relacionados, pero no equivalentes.

## 13.4 Representación de los datos

En los datos experimentales no existe necesariamente una expresión matemática conocida entre dos mediciones consecutivas. Utilizar trapecios supone una representación lineal, mientras que utilizar un spline supone una representación suave mediante polinomios.

La elección de la representación debe ser coherente con la naturaleza de los datos y con el objetivo del análisis.

## 13.5 Abstracción de las bibliotecas

Las bibliotecas científicas permiten utilizar algoritmos numéricos complejos mediante interfaces relativamente sencillas.

Octave y Python proporcionan un nivel alto de abstracción que facilita el desarrollo y la visualización. GSL desde C/C++ requiere una implementación más explícita, pero permite observar con mayor claridad elementos como espacios de trabajo, tolerancias, estructuras y selección de algoritmos.

El desarrollo del taller permitió comprobar que diferentes interfaces pueden resolver el mismo problema matemático manteniendo los principios fundamentales del método numérico.

---

# 14. Conclusiones

Los métodos de integración numérica estudiados permitieron obtener resultados consistentes entre GNU Octave, C/C++ con GSL y Python con SciPy.

La regla del trapecio mostró convergencia hacia el valor de referencia a medida que aumentó el número de subintervalos. Los métodos adaptativos permitieron controlar el proceso mediante tolerancias y proporcionaron estimaciones internas del error.

Para datos experimentales, la forma de representar la información entre las mediciones resultó fundamental. El trapecio, Simpson y la interpolación mediante spline produjeron resultados próximos, aunque no idénticos, debido a las diferentes aproximaciones utilizadas entre los puntos.

En diferenciación numérica, las diferencias hacia adelante y central produjeron resultados reproducibles en los tres entornos. También se evidenció la necesidad de tratar adecuadamente los extremos y la mayor sensibilidad de la diferenciación frente a perturbaciones en los datos.

Finalmente, el taller permitió distinguir entre el método matemático y la herramienta computacional utilizada para implementarlo. Aunque Octave, GSL y SciPy presentan interfaces y niveles de abstracción diferentes, los resultados dependen principalmente de la correcta formulación del problema, la discretización, las tolerancias y la representación de los datos.

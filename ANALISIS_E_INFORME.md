# RESPUESTAS Y ANÁLISIS DEL TALLER DE INTEGRACIÓN Y DIFERENCIACIÓN NUMÉRICA

## 1. Análisis Matemático

### Expresión Analítica de Referencia: Determine si puede obtenerse una expresión analítica y, si la obtiene, úsela como referencia de lo contrario, defina una referencia numérica de alta precisión.
Existe una expresión analítica para la integral. Separando los términos y aplicando las fórmulas de integración correspondientes, se obtiene una primitiva $F(x)$. Por tanto, la referencia exacta en el intervalo de trabajo se calcula como:
$$I = F(x_{\text{8}}) - F(x_{\text{0}}) = 2.55982451$$

Este valor se toma como referencia exacta para comparar los resultados obtenidos mediante los métodos numéricos.

### Representación del Área Acumulada: 2.	Explique qué representa el área acumulada.
Es la cantidad total acumulada de la señal en la ventana [0,8] de análisis. Como $f(x)$ se interpreta como una señal-tasa, su integral representa la magnitud neta entregada en ese intervalo (por ejemplo, si $f(x)$ fuera potencia, la integral representaría la energía total).

### Características que Afectan la Aproximación: 3.	Identifique características que puedan afectar la aproximación.
* **Oscilación:** El término sin(3x) presenta oscilaciones de período corto aproximado de 2.094. Si el paso $h$ es grande, la malla representa deficientemente estas oscilaciones y aumenta el error.
* **Decaimiento / Variación:** La señal tiene mayor variación cerca de $x=0$. Una malla uniforme puede desperdiciar puntos en regiones donde la función varía poco, es decir, en puntos donde la función ya es cercana a cero.
* **Curvatura:** Las derivadas de la componente sinusoidal aumentan con la frecuencia, afectando las constantes de error de los métodos de cuadratura.
* **Suavidad:** La función es analítica y no presenta discontinuidades ni singularidades, lo que favorece la convergencia esperada de los métodos numéricos.
* **Ausencia de cambios de signo:** Como $f(x) > 0$, no existe cancelación entre áreas positivas y negativas, haciendo más estable la aproximación de la integral.

---

## 2. Preguntas de Análisis

1. **¿Qué cambia en el trapecio al aumentar $n$?**
   Al aumentar $n$, disminuye el tamaño del paso $h$ y el error de aproximación. Para el método del trapecio, el error presenta un comportamiento de orden $\mathcal{O}(h^2)$, por lo que al duplicar $n$, el error disminuye aproximadamente cuatro veces.

2. **¿Qué diferencia hay entre aumentar $n$ y reducir la tolerancia de un método adaptativo?**
   Aumentar $n$ utiliza una malla uniforme y fija previamente el número de subintervalos, sin garantizar una precisión determinada. En cambio, reducir la tolerancia de un método adaptativo establece una precisión objetivo y permite que el algoritmo determine dónde y cuánto subdividir, concentrando más evaluaciones en las regiones con mayor variación.

3. **¿Mayor precisión implica necesariamente mayor costo?**
   No necesariamente. Dentro de un mismo método, aumentar la precisión implica mayor número de evaluaciones. Sin embargo, un método más eficiente puede alcanzar mayor precisión con menos evaluaciones. Los métodos adaptativos como GSL y SciPy logran errores mucho menores que el trapecio con $n=1000$ utilizando considerablemente menos evaluaciones.

4. **Comparación del error estimado por GSL/SciPy vs. error respecto a la referencia:**
   El error estimado por GSL y SciPy es calculado internamente a partir de sus aproximaciones, sin conocer el valor exacto. El error respecto a la referencia compara el resultado numérico con el valor analítico ($2.55982451$). Ambos métodos presentan resultados extremadamente cercanos a la referencia, confirmando su alta precisión.

---

## 3. Comparación Final entre Entornos

| Criterio | Octave | C / C++ + GSL | Python + SciPy |
| :--- | :--- | :--- | :--- |
| **Facilidad de implementación** | Alta — vectorizado, sin compilación | Baja — callbacks y configuración más compleja | Alta — funciones de alto nivel |
| **Control del algoritmo** | Medio | Muy alto — mayor control de parámetros | Alto |
| **Manejo de tolerancias** | Limitado | Alto — permite definir `epsabs` y `epsrel` | Alto — `epsabs` y `epsrel` |
| **Integración de funciones** | Directa | Potente, pero más verbosa | Directa y potente |
| **Integración de datos** | Media — requiere mayor manejo en Simpson | Media — implementación manual o interpolación | Alta — funciones específicas para datos (`trapezoid`, `simpson`) |
| **Estimación del error** | Limitada — requiere comparar resultados | Alta — proporciona estimación del error | Alta — `quad` devuelve estimación del error |
| **Tiempo de ejecución** | Medio | Alto — código compilado | Alto |
| **Visualización** | Alta — herramientas integradas | Baja — requiere herramientas externas | Alta — integración con herramientas de graficación |

---

## 4. Reflexión y Evocación

### Cambio de estrategia de $f(x)$ a datos experimentales $(x_i, y_i)$
* **Evaluación en puntos arbitrarios:** La imposibilidad de evaluar la función en nodos arbitrarios impide el uso de cuadraturas adaptativas. El cálculo queda restringido al empleo de reglas de malla fija sobre la muestra disponible.
* **Control del paso $h$:** Al estar determinado por el instrumento de medición, se pierde la capacidad de refinar la malla, efectuar estudios de convergencia o controlar directamente el error.
* **Referencia y visibilidad del error:** La ausencia de una solución analítica elimina el cálculo del error exacto. La validación se limita a la comparación de discrepancias entre distintos métodos.
* **Suavidad y ruido:** Sin garantía de analiticidad en $f(x)$, las cotas teóricas de error ($\mathcal{O}(h^2), \mathcal{O}(h^4)$) pierden validez estricta. El ruido experimental degrada la precisión, afectando en mayor medida a la diferenciación que a la integración.

### Conceptos Fundamentales
* **Discretización:** El paso $h$ determina el error dentro del régimen asintótico. Si la resolución es insuficiente, el orden teórico del método no llega a manifestarse.
* **Error:** Es crucial diferenciar el error absoluto del error estimado *a posteriori*. Se comprueba que el orden del método ($\mathcal{O}(h^4)$ frente a $\mathcal{O}(h^2)$) es más eficaz para reducir el error que el incremento masivo de puntos.
* **Tolerancia:** Modifica la dinámica de trabajo; en lugar de prefijar $n$, se establece la precisión requerida (`epsabs`/`epsrel`) para que el algoritmo determine la densidad de evaluación.
* **Abstracción de bibliotecas:** Las herramientas de alto nivel simplifican el desarrollo, aunque pueden ocultar comportamientos internos. Mientras `scipy.integrate.simpson` gestionó el borde impar automáticamente, las implementaciones manuales en Octave y C generaron discrepancias sin emitir advertencias, demostrando la necesidad de comprender el funcionamiento interno de los algoritmos.
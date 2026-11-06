# Taller de Diferenciación Numérica - Robot de Cinemática Diferencial

## Pregunta de la Parte 2

**Compare con Octave y explique qué operación de NumPy reemplaza las diferencias explícitas y cómo trata los extremos.**

La operación `np.gradient` reemplaza las diferencias finitas explícitas. En Octave se tuvo que programar manualmente las diferencias centradas para los puntos interiores y las unilaterales para los extremos; en Python, `np.gradient` hace todo eso internamente en una sola línea.

Para los extremos, `np.gradient` usa diferencias unilaterales de primer orden: hacia adelante en el primer punto y hacia atrás en el último. En Octave se replicó ese comportamiento escribiendo esas dos condiciones a mano. Por eso los valores obtenidos coinciden.

---

## Pregunta de la Parte 3

**Explique por qué los datos tabulados requieren una estrategia distinta a `gsl_deriv_central`.**

`gsl_deriv_central` está diseñado para derivar una función evaluable f(x). Internamente evalúa f en x+h y en x-h y aplica la fórmula centrada. Pero en este taller no se tiene una función x(t) continua; se tiene una tabla de valores (t_i, x_i, y_i). No es posible evaluar x(t) en puntos intermedios porque no se conoce la función subyacente.

Por eso en C++ se implementa las diferencias directamente sobre los arreglos. GSL sí sería útil si se tuviera la función explícita, pero para datos tabulados no aplica.

---

## Tabla comparativa

| Criterio | Octave | Python | C/C++ GSL |
|---|---|---|---|
| Carga de datos | `dlmread` | `np.loadtxt` | `ifstream` + `std::stod` |
| Cálculo de ẋ, ẏ | diferencias explícitas | `np.gradient` | ciclos manuales |
| Cálculo de v | `sqrt(vx.^2 + vy.^2)` | `np.sqrt` | `std::sqrt` |
| Cálculo de θ | `atan2` | `np.arctan2` | `std::atan2` |
| Cálculo de ω | diferencias explícitas | `np.gradient` | diferencias manuales |
| Manejo de arreglos | indexación 1-based | vectorizado | `std::vector` |
| Facilidad de implementación | alta | alta | media/baja |
| Control sobre el algoritmo | medio | medio | alto |
| Tiempo de ejecución | medio | rápido | muy rápido |

---

## Preguntas de análisis

**1. ¿Qué diferencia existe entre calcular la velocidad lineal a partir de ẋ, ẏ y calcularla directamente desde diferencias de posición?**

Calcular v a partir de ẋ, ẏ da la velocidad instantánea y permite obtener θ con `atan2`. Calcularla directamente desde diferencias de posición solo da rapidez promedio y pierde la dirección, por lo que no se podría obtener ω.

**2. ¿Por qué un error pequeño en ẋ, ẏ puede producir un efecto mayor al calcular ω?**

Porque θ = atan2(ẏ, ẋ) es sensible cuando ẋ o ẏ son pequeños o hay ruido. Y ω es la derivada de θ, así que derivar amplifica ese ruido.

**3. ¿Qué ocurre si se reduce h manteniendo un nivel de ruido fijo en las posiciones?**

El error de truncamiento baja, pero el error por ruido crece como ruido/h. Existe un h óptimo; si es muy pequeño, la derivada se vuelve ruidosa.

**4. ¿Por qué ω requiere especial cuidado con el desenvolvimiento de θ?**

Porque atan2 devuelve ángulos en (-π, π]. Al cruzar el límite aparece un salto artificial de 2π. Sin unwrap, al diferenciar esos saltos generan picos falsos en ω.

---

## Conclusiones

1. Los tres entornos (Octave, Python y C++) dieron resultados numéricamente equivalentes (v entre 0.542613 y 1.581951, ω entre -0.186657 y -0.023407), lo que confirma que el método es correcto. La diferencia está en la herramienta: NumPy automatiza con `np.gradient`, mientras que Octave y C++ requieren implementarlo explícitamente, dando más control pero más trabajo manual.

2. El unwrap de θ es obligatorio antes de calcular ω. Sin él, los saltos de 2π producidos por `atan2` aparecen como discontinuidades y generan valores falsos de velocidad angular, sin importar el entorno usado.

---

## Reflexión

**1. Reconstrucción del procedimiento.**

El procedimiento para pasar de (x, y, t) a v y ω es: primero se derivan x(t) y y(t) respecto al tiempo para obtener ẋ y ẏ. Con esas derivadas se calcula la rapidez lineal v = sqrt(ẋ² + ẏ²) y la orientación θ = atan2(ẏ, ẋ). Luego se aplica unwrap a θ para eliminar los saltos de 2π que introduce atan2. Finalmente se deriva θ respecto al tiempo para obtener ω.

El error numérico puede introducirse en dos puntos: (a) al derivar x e y, porque la diferenciación amplifica el ruido en los datos; (b) al derivar θ, porque además hereda los errores previos y es especialmente sensible cuando ẋ o ẏ son pequeños.

**2. Transferencia del aprendizaje.**

Con datos reales de un robot con otra frecuencia de muestreo y ruido de sensores, conservaría el esquema general: derivar posición → calcular v y θ → unwrap → derivar θ para ω. Modificaría la elección del paso h y posiblemente aplicaría un filtro (por ejemplo Savitzky-Golay o un filtro pasa-bajos) antes de derivar, porque los datos reales son ruidosos. Para seleccionar el método de diferenciación, compararía el error de truncamiento contra el error por ruido: si el ruido es alto, conviene un esquema más robusto o suavizar primero; si los datos son suaves, la diferencia centrada es la mejor opción.
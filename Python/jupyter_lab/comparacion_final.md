# Comparación final entre entornos



| Criterio | GNU Octave | C/C++ + GSL | Python + SciPy |
|---|---|---|---|
| **Facilidad de implementación** | Alta. Sintaxis matemática natural, sin gestión de memoria. | Media/baja. Requiere compilar, manejar punteros, `gsl_function` y workspaces. | Alta. Pocas líneas, sintaxis clara, sin compilación. |
| **Control del algoritmo** | Medio. Se elige la función, pero hay menos control sobre parámetros internos. | Alto. Se controla tolerancia, límite de subdivisiones, regla Gauss–Kronrod (`key`) y workspace. | Medio. Tolerancias y `limit` expuestos, pero sin control sobre la regla interna. |
| **Manejo de tolerancias** | Expuesto (`quad` acepta `tol`). | Expuesto y detallado (`epsabs`, `epsrel`, `limit`). | Expuesto (`epsabs`, `epsrel`, `limit`). |
| **Integración de funciones** | `quad` (adaptativo) e `integral`. | `gsl_integration_qag`, `qags`, `qagi` según el tipo de intervalo. | `scipy.integrate.quad` (QUADPACK). |
| **Integración de datos discretos** | `trapz` (vectorizado). Simpson debe implementarse manualmente. | Interpolación con `gsl_spline` + `gsl_spline_eval_integ`, o trapecio manual. | `trapezoid` y `simpson` en `scipy.integrate`. |
| **Estimación del error** | `quad` no devuelve error estimado de forma estándar. | `gsl_integration_qag` devuelve `result` y `error` por punteros. | `quad` devuelve `(result, error)` en una tupla. |
| **Tiempo de ejecución** | Intermedio. Bueno para prototipos, más lento que C++ en bucles. | Más rápido en cómputo intensivo. Código compilado, optimizable con `-O2`/`-O3`. | Intermedio. NumPy/SciPy vectorizados son rápidos; bucles Python puros son lentos. |
| **Visualización** | Nativa con `plot`, `fill` y `print` a PNG. | No nativa. Requiere librerías externas. | Excelente con `matplotlib`. |
| **Prototipado** | Muy rápido. Ideal para explorar y graficar. | Lento. Cada cambio exige recompilar. | Muy rápido. Ideal para prototipar y comparar métodos. |
| **Reproducibilidad / portabilidad** | Script único, fácil de compartir. Requiere Octave instalado. | Requiere compilador y GSL. Mayor fricción para reproducir. | `requirements.txt` reconstruye el entorno completo. |
| **Curva de aprendizaje** | Baja para quien conoce MATLAB. | Alta: punteros, gestión de memoria, compilación. | Baja/media: sintaxis simple, mucha documentación. |

---

## Síntesis por criterio

### Facilidad de implementación
Octave y Python permiten escribir una integral adaptativa en una línea. GSL exige armar `gsl_function`, `workspace`, pasar punteros a `result` y `error`, y liberar memoria al final.

### Control del algoritmo
GSL es el entorno que más detalles expone: regla interna (`key` de 15 a 61 puntos Gauss–Kronrod), límite de subdivisiones y estrategia de refinamiento. Octave y SciPy ocultan esos detalles detrás de una interfaz de alto nivel.

### Integración de datos discretos
Python + SciPy ofrece `trapezoid` y `simpson` listos para usar. En Octave solo existe `trapz` (Simpson debe implementarse). En C++ hay que manejar interpolación o trapecio explícito.

### Estimación del error
GSL y SciPy devuelven `(result, error)` de forma directa. Octave `quad` no devuelve estimación estándar; hay que recurrir a la diferencia entre métodos o al error del integrador.

### Tiempo de ejecución
C/C++ con GSL es el más rápido en cómputo intensivo. Para problemas pequeños como el Ejercicio 2 (50 datos), la diferencia es de microsegundos y no es determinante.

### Visualización
Python con `matplotlib` produce gráficas de calidad de publicación. Octave tiene visualización funcional. GSL no incluye visualización.

### Prototipado
Octave y Python permiten probar una idea en segundos. C++ obliga a compilar y depurar en cada iteración.

### Reproducibilidad
Python + SciPy ofrece la mejor reproducibilidad: `requirements.txt` captura versiones exactas y `pip install -r requirements.txt` reconstruye el entorno. GSL y Octave dependen de versiones del sistema.

---

## Recomendación de uso según el contexto

| Contexto | Entorno recomendado |
|---|---|
| Explorar un método nuevo, comparar varios | Python o Octave |
| Prototipar rápido con visualización | Python |
| Producción, alto rendimiento | C/C++ + GSL |
| Cálculo con muchas evaluaciones de función | C/C++ + GSL |
| Trabajo académico con informe y gráficas | Python |
| Interfaz directa con hardware o sistemas embebidos | C/C++ + GSL |
| Análisis interactivo de datos experimentales | Python (NumPy + SciPy) |

---

## Conclusión general

Los tres entornos son **matemáticamente equivalentes**: con las mismas tolerancias y los mismos datos producen los mismos resultados numéricos.

Las diferencias son de **ergonomía, control y contexto**:

- **Python + SciPy** → mejor para prototipar, visualizar y reproducir.
- **GNU Octave** → muy cercano a Python en ergonomía; adecuado si ya se domina MATLAB.
- **C/C++ + GSL** → mejor para producción, alto rendimiento y control fino del algoritmo.

La elección depende del objetivo: explorar → Python; enseñar → Octave; producir → C++.
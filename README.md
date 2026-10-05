# Taller de Diferenciación Numérica - Cinemática Diferencial de un Robot Móvil

## 7. Comparación de Resultados

| Criterio | Octave | Python | C/C++ + GSL |
| :--- | :--- | :--- | :--- |
| **Carga de datos** | `dlmread('trayectoria_robot.csv', ',', 1, 0)` de forma nativa e inmediata. | `np.loadtxt('trayectoria_robot.csv', delimiter=',', skiprows=1)` muy limpia e intuitiva. | Lectura manual mediante flujos `std::ifstream`, `std::stringstream` y conversión `std::stod`. |
| **Cálculo de $\dot{x}, \dot{y}$** | Indexación por rangos vectoriales: `(x(3:N) - x(1:N-2)) / (2*h)`. | Función nativa optimizada: `np.gradient(x, t)`. | Bucle `for` con la fórmula explícita de diferencias centradas $O(h^2)$. |
| **Cálculo de $v$** | Vectorizado elemento a elemento: `sqrt(vx.^2 + vy.^2)`. | Vectorizado de alto rendimiento: `np.sqrt(vx**2 + vy**2)`. | Bucle `for` element-by-element con `std::sqrt`. |
| **Cálculo de $\theta$** | `atan2(vy, vx)` aplicado directamente sobre vectores. | `np.arctan2(vy, vx)` vectorizado sobre `ndarrays`. | Bucle `for` elemento por elemento con `std::atan2`. |
| **Cálculo de $\omega$** | Función `unwrap(theta)` seguida de diferencia central vectorial. | Función `np.unwrap(theta)` seguida de `np.gradient(theta_unwrap, t)`. | Algoritmo manual de `unwrap` con bucle `while` ($> \pi$, $< -\pi$) + bucle `for` de diferencia central. |
| **Manejo de arreglos** | Matrices de punto flotante de doble precisión estilo MATLAB/Octave. | Estructuras de datos contiguas `numpy.ndarray`. | Contenedores dinámicos de la STL (`std::vector<double>`). |
| **Facilidad de implementación** | Muy alta: Sintaxis matemática directa ideal para prototipado rápido. | Muy alta: Expresiva, limpia y con ecosistema amplio de librerías científico-técnicas. | Media: Requiere gestión explícita de E/S de archivos, memoria y bucles. |
| **Control sobre el algoritmo** | Medio: Las funciones vectorizadas están abstraídas por el intérprete de Octave. | Medio: Las funciones numéricas están abstraídas en rutinas compiladas en C de NumPy. | Total: Control completo sobre cada iteración, puntero, memoria y operación matemática. |
| **Tiempo de ejecución** | Intérprete adecuado para scripts pequeños, con sobrecosto respecto a C++. | Muy rápido debido a las rutinas subyacentes compiladas en C de NumPy. | El más rápido y eficiente (código compilado nativo de máquina). |

---

## 8. Preguntas de Análisis

1. **¿Qué diferencia existe entre calcular la velocidad lineal a partir de $\dot{x}, \dot{y}$ y calcularla directamente desde diferencias de posición?**
   * **Respuesta:** El cálculo $v = \sqrt{\dot{x}^2 + \dot{y}^2}$ obtiene la magnitud instantánea del vector velocidad en cada eje a partir de sus derivadas temporales tangenciales. En cambio, calcular la velocidad directamente como $\frac{\Delta d}{\Delta t} = \frac{\sqrt{(x_{i+1}-x_i)^2 + (y_{i+1}-y_i)^2}}{h}$ aproxima la longitud de la secante (cuerda) entre dos puntos muestreados. En trayectorias curvas, la cuerda siempre es menor que la longitud real del arco recorrido, lo que provoca una subestimación sistemática de la velocidad lineal en presencia de curvatura.

2. **¿Por qué un error pequeño en $\dot{x}, \dot{y}$ puede producir un efecto mayor al calcular $\omega$?**
   * **Respuesta:** Porque el cálculo de $\omega$ implica un proceso de **doble diferenciación no lineal**. Primero se calcula la orientación mediante la función trascendente $\theta = \text{atan2}(\dot{y}, \dot{x})$, la cual amplifica las variaciones relativas de las componentes $v_x$ y $v_y$. Posteriormente, al derivar $\theta$ respecto al tiempo para obtener $\omega = \dot{\theta}$, cualquier pequeña perturbación o ruido en las posiciones/velocidades originales se divide entre el paso de tiempo $h$, sufriendo un efecto de amplificación de alta frecuencia.

3. **¿Qué ocurre si se reduce $h$ manteniendo un nivel de ruido fijo en las posiciones?**
   * **Respuesta:** Ocurre la degradación del cálculo por **propagación y explosión del ruido numérico**. El error total en la diferenciación numérica consta de dos componentes: el error de truncamiento del método (que decrece como $O(h^2)$) y el error derivado del ruido de medición $\epsilon$ (que crece como $O(\epsilon / h)$). Si el ruido de los sensores $\epsilon$ es constante y $h \to 0$, el término $\frac{\epsilon}{h} \to \infty$, haciendo que la derivada numérica sea dominada totalmente por el ruido y pierda todo significado físico.

4. **¿Por qué $\omega$ requiere especial cuidado con el desenvolvimiento de $\theta$?**
   * **Respuesta:** La función $\text{atan2}$ devuelve ángulos acotados estrictamente en el intervalo $[-\pi, \pi]$. Cuando el vehículo gira o cambia de cuadrante pasando la frontera de $\pi$ rad a $-\pi$ rad, la señal de ángulo sufre un salto discontinuo artificial de $2\pi$ rad entre dos muestras consecutivas. Si se calcula la diferencia finita sobre esta discontinuidad sin aplicar un desenvolvimiento angular (`unwrap`), el algoritmo interpreta una velocidad angular espuria de $\omega \approx \frac{-2\pi}{h}$, produciendo picos (*spikes*) gigantescos e irrealizables que distorsionan completamente la cinemática.

---

## 9. Producto Esperado

### Conclusiones sobre Método y Entorno Computacional
1. **Conclusión sobre el Método Matemático:** El esquema de diferencias centradas $O(h^2)$ ofrece un balance óptimo entre precisión y simplicidad para datos tabulados equiespaciados, requiriendo un tratamiento riguroso en fronteras mediante diferencias unilaterales $O(h)$ y un procedimiento previo de desenvolvimiento de fase (`unwrap`) indispensable para evitar artefactos en la velocidad angular.
2. **Conclusión sobre los Entornos Computacionales:** La elección del entorno depende del ciclo de desarrollo: GNU Octave y Python (NumPy) son superiores para el prototipado rápido, análisis interactivo y procesamiento vectorizado debido a su alta abstracción y expresividad; por su parte, C++ garantiza el mejor rendimiento en tiempo real y mínimo consumo de memoria, ideal para su despliegue embarcado en sistemas embebidos de robótica móvil (ROS 2 / micro-ROS).

---

## 10. Reflexión

### 1. Reconstrucción del Procedimiento
Sin consultar el código, el procedimiento cinemático diferencial completo para pasar de $(x_i, y_i, t_i)$ a $v_i$ y $\omega_i$ comprende la siguiente secuencia matemática:

1. **Obtención de datos y validación del paso temporal:** Verificar que el muestreo es equiespaciado determinando $h = t_{i+1} - t_i$[cite: 1].
2. **Diferenciación posicional ($v_x, v_y$):**
   * Puntos interiores ($i = 1, \dots, N-2$): Se aplica la diferencia centrada de segundo orden:
     $$\dot{x}_i = \frac{x_{i+1} - x_{i-1}}{2h}, \quad \dot{y}_i = \frac{y_{i+1} - y_{i-1}}{2h}$$
   * Puntos extremos ($i = 0$ e $i = N-1$): Se aplican diferencias unilaterales de primer orden hacia adelante y hacia atrás respectivamente.
3. **Cálculo de la velocidad lineal ($v$):**
   $$v_i = \sqrt{\dot{x}_i^2 + \dot{y}_i^2}$$
4. **Cálculo de la orientación instantánea ($\theta$):**
   $$\theta_i = \text{atan2}(\dot{y}_i, \dot{x}_i)$$
5. **Desenvolvimiento angular (`unwrap`):** Se recorre el vector $\theta$ de forma secuencial corrigiendo cualquier salto discreto mayor a $\pi$ o menor a $-\pi$ mediante la adición o sustracción de múltiplos de $2\pi$.
6. **Diferenciación angular ($\omega$):** Se aplican nuevamente diferencias centradas sobre el ángulo desenvuelto $\theta^{\text{unwrap}}$:
   $$\omega_i = \frac{\theta^{\text{unwrap}}_{i+1} - \theta^{\text{unwrap}}_{i-1}}{2h}$$

* **Fuentes de error numérico:** Error de truncamiento de la serie de Taylor ($O(h^2)$ en el interior y $O(h)$ en las fronteras), errores de redondeo de punto flotante en la función $\text{atan2}$ y propagación del ruido posicional amplificado por $h$.

### 2. Transferencia del Aprendizaje
Al trabajar con datos reales provenientes de un robot físico provisto de ruido de sensores (encoders, IMU, GPS/LiDAR) y frecuencias de muestreo variables:

* **Decisiones que conservaría:** La secuencia lógica del modelo cinemático ($v_x, v_y \to v \to \theta \to \text{unwrap} \to \omega$) y el uso obligatorio del desenvolvimiento de fase (`unwrap`) antes de la derivación angular.
* **Decisiones que modificaría:** Sustituiría las diferencias finitas puras sobre datos crudos, aplicando previamente un **filtro de suavizado** (como un filtro de Savitzky-Golay, *splines* de suavizado o un Filtro de Kalman Extendido - EKF) para atenuar el ruido de medición.
* **Criterio para seleccionar el método de diferenciación:** Para baja perturbación y alta frecuencia de muestreo, diferencias centradas $O(h^2)$ o $O(h^4)$ son óptimas. Con ruido estocástico alto de sensores, el criterio fundamental es emplear **diferenciación basada en modelos o filtros polinomiales/estocásticos** ajustando la ventana de suavizado según la relación señal-ruido.
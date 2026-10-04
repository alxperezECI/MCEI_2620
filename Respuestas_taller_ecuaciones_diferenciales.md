# Ecuaciones diferenciales
## Sistema masa-resorte-amortiguador
 
### Comparación de resultados
El problema se resolvió mediante el método de Runge-Kutta de cuarto orden (RK4), utilizando un paso de tiempo de 0.01 s y un intervalo de simulación entre 0 y 15 s.
 
Se utilizaron los parámetros m = 20 kg y k = 20 N/m, con una posición inicial de 1 m y velocidad inicial igual a cero.
 
| Coeficiente c [N·s/m] | Comportamiento | Octave x(15) [m] | Python x(15) [m] |
| 5 | Subamortiguado | -0.089791 | -0.089791 |
| 40 | Crítico | 0.000005 | 0.000005 |
| 200 | Sobreamortiguado | 0.222006 | 0.222006 |
 
Los resultados obtenidos en Octave y Python coinciden para los tres valores del coeficiente de amortiguamiento, lo cual permite comprobar que la implementación del método RK4 fue consistente en ambos entornos.
 
Para c = 5 N·s/m se observa un comportamiento oscilatorio cuya amplitud disminuye progresivamente debido al amortiguamiento. Para c = 40 N·s/m el sistema retorna al equilibrio sin presentar oscilaciones y alcanza rápidamente un desplazamiento cercano a cero. Para c = 200 N·s/m tampoco se presentan oscilaciones, pero el retorno a la posición de equilibrio es más lento.
 
### Comparación entre Octave y Python
| Criterio | Octave | Python |
| Método numérico | RK4 | RK4 |
| Paso de tiempo | 0.01 s | 0.01 s |
| Número de pasos | 1500 | 1500 |
| Implementación | RK4 programado explícitamente | RK4 programado explícitamente |
| Manejo de datos | Vectores y matrices de Octave | Arreglos de NumPy |
| Generación de gráfica | Funciones gráficas de Octave | Matplotlib |
| Tiempo de ejecución obtenido | 0.541221 s | 0.301408 s |
| Resultados | Coinciden con Python | Coinciden con Octave |
 
Los dos programas utilizaron las mismas condiciones iniciales, el mismo paso de tiempo y el mismo método numérico. Por esta razón, las diferencias entre ambos corresponden principalmente al entorno de programación y no al procedimiento matemático utilizado.

## Conclusiones
1. El método de Runge-Kutta de cuarto orden permitió resolver numéricamente el sistema masa-resorte-amortiguador y obtener resultados consistentes en Octave y Python. Para las mismas condiciones iniciales y el mismo paso de tiempo, ambos programas produjeron los mismos valores de desplazamiento al finalizar los 15 segundos de simulación.
 
2. El coeficiente de amortiguamiento influye directamente en la respuesta del sistema. Con un amortiguamiento bajo se presentan oscilaciones que disminuyen progresivamente, mientras que al aumentar el amortiguamiento el sistema deja de oscilar. Sin embargo, un amortiguamiento muy alto produce un retorno más lento hacia la posición de equilibrio.
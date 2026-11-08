# Taller de reducción de dimensionalidad mediante PCA

## Implementación
Se implementó PCA en Python con NumPy y Matplotlib para analizar diez mediciones de temperatura, corriente y vibración de un motor eléctrico.

## Ejecución
python pca_motor.py

## Resultados
### 1. Estandarización
| Variable         | Media    | Desviación estándar |
| Temperatura (°C) | 55.5000  | 5.016639            |
| Corriente (A)    | 5.9000   | 0.533333            |
| Vibración (mm/s) | 3.0800   | 0.637356            |

Se estandarizaron las variables para evitar diferencias de escala.

### 2. Matriz de covarianza
| Variable    | Temperatura | Corriente | Vibración |
| Temperatura | 1.000000    | 0.996683  | 0.997343  |
| Corriente   | 0.996683    | 1.000000  | 0.996958  |
| Vibración   | 0.997343    | 0.996958  | 1.000000  |
Los valores positivos cercanos a 1 indican una correlación muy fuerte entre las variables.

### 3. Autovalores y autovectores
| Variable    | PC1      | PC2       | PC3       |
| Temperatura | 0.577357 | -0.548700 | -0.604638 |
| Corriente   | 0.577283 | 0.798017  | -0.172953 |
| Vibración   | 0.577410 | -0.249192 | 0.777496  |
La suma de autovalores coincide con la traza de la matriz de covarianza: 3.000000.

### 4. Varianza explicada
| Componente | Autovalor | Individual  | Acumulada    |
| PC1        | 2.993990  | 99.799654 % | 99.799654 %  |
| PC2        | 0.003388  | 0.112937 %  | 99.912590 %  |
| PC3        | 0.002622  | 0.087410 %  | 100.000000 % |

Varianza explicada: resultados/varianza_explicada.png
PC1 concentra el 99.80 % de la variabilidad. Con PC2 se conserva el 99.91 %.

### 5. Proyección PC1-PC2
Proyección PCA: resultados/proyeccion_pca.png
Las muestras 1 y 10 están en extremos opuestos de PC1. Las muestras 5 y 6 se ubican cerca del centro.

### 6. Recomendación técnica
Se recomienda conservar dos componentes para visualización, manteniendo el 99.91 % de la variabilidad. Para diagnóstico de fallas deben conservarse también las mediciones originales.

## 4. Preguntas de reflexión
**1. ¿Qué problema aparecería si se aplica PCA directamente a las variables originales sin estandarizarlas, teniendo unidades °C, A y mm/s?**
La temperatura dominaría el análisis debido a su mayor varianza numérica, haciendo que las componentes principales dependan de las escalas de medición y no de la relación entre las variables.

**2. Si PC1 y PC2 explican un porcentaje alto de la varianza, ¿podría perderse información útil para detectar una falla incipiente? Explique.**
Sí. Aunque PC1 y PC2 conservan el 99.91 % de la varianza, el 0.09 % descartado podría contener pequeñas variaciones asociadas a fallas incipientes. Por ello, se recomienda conservar las mediciones originales para el diagnóstico.

## Conclusiones
El PCA confirmó una fuerte correlación entre las tres variables. La reducción a dos componentes conserva el 99.91 % de la variabilidad, siendo adecuada para visualización y análisis exploratorio.
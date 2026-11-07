# Taller de reducción de dimensionalidad mediante PCA

Métodos computacionales en ingeniería (MCEI). Implementado en Octave: `pca\_motor.m`.

Presentado Por: Laura Camila Andrade y Angel Fabian Pedreros Almeida

## 1\. Datos y estandarización

Se construyó la matriz X (10 × 3) con temperatura T (°C), corriente I (A) y vibración V (mm/s), y se estandarizó cada columna: z = (x − media) / desviación estándar muestral (N−1).

||T (°C)|I (A)|V (mm/s)|
|-|-|-|-|
|Media|55.5000|5.9000|3.0800|
|Desv. estándar|5.0166|0.5333|0.6374|

## 2\. Matriz de covarianza S (de las variables estandarizadas)

||T|I|V|
|-|-|-|-|
|**T**|1.0000|0.9967|0.9973|
|**I**|0.9967|1.0000|0.9970|
|**V**|0.9973|0.9970|1.0000|

La diagonal vale 1 porque cada variable estandarizada tiene varianza 1. Todos los términos fuera de la diagonal son positivos y cercanos a 0.997: temperatura, corriente y vibración crecen juntas y tienen una correlación lineal casi perfecta, osea aportan información muy redundante.

## 3\. Autovalores y autovectores

||PC1|PC2|PC3|
|-|-|-|-|
|Autovalor|2.9940|0.003388|0.002622|
|Carga en T|0.5774|0.5487|0.6046|
|Carga en I|0.5773|-0.7980|0.1730|
|Carga en V|0.5774|0.2492|-0.7775|

La suma de autovalores es 3.0000 y coincide con la traza de S (3). PC1 tiene cargas casi iguales y positivas, así que representa el comportamiento conjunto de las tres variables. PC2 y PC3 mezclan signos lo que describen diferencias entre variables.

## 4\. Varianza explicada

|Componente|Individual (%)|Acumulada (%)|
|-|-|-|
|PC1|99.80|99.80|
|PC2|0.11|99.91|
|PC3|0.09|100.00|

## 5\. Proyección PC1-PC2

!\[Proyección de las 10 muestras sobre PC1-PC2](pca\_pc1\_pc2.png)

Las muestras se ordenan a lo largo de PC1 casi en el mismo orden de la numeración, reflejando el aumento conjunto de las tres variables. Las muestras 1 y 10 son las más alejadas (extremos); 2 y 3, y 9 y 10, son las más próximas entre sí. La dispersión en PC2 es de solo ±0.1 y PC2 explica 0.11 % de la varianza, por lo que esas diferencias no indican observaciones diferentes

## 6\. Recomendación

Se recomienda **conservar dos componentes** en lugar de las tres variables originales. PC1 + PC2 conservan 99.91 % de la variabilidad, y las tres variables son casi redundantes (correlaciones ≈ 0.997). Dos componentes permiten visualizar las ventanas de operación en un plano y reducen el costo de análisis. Incluso PC1 sola conserva 99.80 %, pero PC2 se mantiene para graficar y para verificar desviaciones respecto a la tendencia.

## 7\. Preguntas de reflexión

**1. ¿Qué problema aparecería si se aplica PCA sin estandarizar?**
PCA maximiza la varianza, y la varianza depende de las unidades. T (°C) tiene varianza ≈ 25, mientras que I (A) ≈ 0.28 y V (mm/s) ≈ 0.41. Sin estandarizar, PC1 quedaría dominada casi por completo por T (≈ 97 % de la varianza total) solo por la escala numérica, y no por su importancia física. Cambiar de unidades (por ejemplo, °C a K, o mm/s a m/s) cambiaría las componentes.

**2. Si PC1 y PC2 explican un porcentaje alto, ¿se podría perder información útil para detectar una falla incipiente?**
Si, varianza alta no es lo mismo que información útil. Una falla incipiente puede aparecer como un cambio pequeño o como un daño de la relación habitual entre variables (por ejemplo, vibración subiendo sin que suba la corriente). Ese patrón vive en las direcciones de poca varianza (PC2, PC3), justo las que se descartan al reducir. Por eso, para detección de fallas conviene revisar también los componentes descartados, por ejemplo con el error de reconstrucción.


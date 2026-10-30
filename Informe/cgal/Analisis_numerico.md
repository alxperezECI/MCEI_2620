# Análisis de precisión y robustez numérica

Autores: Jimmy Alexander Calderón Trujillo y Sebastián Barahona.

## Experimento

Se estudiaron los puntos A=(0,0), B=(1,1) y C=(2,2+epsilon),
con epsilon igual a 10^-8, 10^-12, 10^-15, 10^-16, 10^-17 y 10^-20.

El determinante de orientación, en aritmética exacta, es:

    D = (Bx-Ax)(Cy-Ay) - (By-Ay)(Cx-Ax)
      = 1*(2+epsilon) - 1*2
      = epsilon

Por tanto, para cualquier epsilon positivo, los puntos no son
colineales y su orientación es un giro a la izquierda.

## Variantes comparadas

1. EPICK: predicados exactos sobre coordenadas representadas como double.
2. EPECK desde double: kernel exacto que recibe la coordenada
   previamente calculada mediante 2.0 + epsilon en punto flotante.
3. EPECK racional: epsilon se construye dividiendo exactamente
   la unidad entre diez de manera repetida; la suma también es exacta.

## Resultados observados

| epsilon | EPICK | EPECK desde double | EPECK racional |
|---|---|---|---|
| 10^-8 | Izquierda | Izquierda | Izquierda |
| 10^-12 | Izquierda | Izquierda | Izquierda |
| 10^-15 | Izquierda | Izquierda | Izquierda |
| 10^-16 | Colineales | Colineales | Izquierda |
| 10^-17 | Colineales | Colineales | Izquierda |
| 10^-20 | Colineales | Colineales | Izquierda |

En los tres últimos casos, la coordenada calculada como double
fue exactamente 2 y el determinante calculado fue 0.

En IEEE-754 binary64, la separación entre 2 y el siguiente número
representable mayor es 2^-51, aproximadamente 4.44e-16.
Con redondeo al más cercano, los desplazamientos positivos ensayados
de 1e-16 o menores desaparecen al sumarlos a 2.

Este umbral depende de la magnitud de las coordenadas y del cálculo;
no significa que double sea incapaz de representar el número 1e-16.

## Interpretación

Un predicado exacto clasifica correctamente la geometría de las
coordenadas que recibe. No recupera información perdida al preparar
esas coordenadas.

Por eso EPECK desde double también clasifica como colineales los
casos donde C ya fue redondeado a (2,2). La conversión posterior
a un tipo exacto conserva el valor redondeado, no el valor ideal.

La variante racional mantiene epsilon positivo y obtiene el giro
a la izquierda para todos los valores estudiados.

Este experimento demuestra pérdida de información al construir
la entrada en punto flotante. No demuestra un fallo de EPICK ni
un error de evaluación de su predicado de orientación.

## Implicación para el taller

Para calcular la envolvente convexa y la conectividad de Delaunay
sobre las coordenadas double suministradas, EPICK permite tomar
decisiones geométricas robustas respecto de esos valores.

Cuando una aplicación necesita conservar exactamente nuevas
coordenadas construidas, por ejemplo intersecciones utilizadas
en operaciones posteriores, puede requerir EPECK y una preparación
de entradas que no pierda antes la información necesaria.

La exactitud aritmética tampoco elimina errores de medición ni
incertidumbres físicas de las ubicaciones de los sensores.

## Reproducción

Desde la raíz del repositorio:

    cmake -S C_C++/cgal -B C_C++/cgal/build -DCMAKE_BUILD_TYPE=Release
    cmake --build C_C++/cgal/build -j 2
    C_C++/cgal/build/precision

Código: C_C++/cgal/precision.cpp.
Resultados: datos/cgal/precision.csv.

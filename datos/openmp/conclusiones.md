# Conclusiones de la práctica OpenMP

Se calculó la integral de sin(x) entre 0 y pi con un millón
de subintervalos y nueve mediciones por configuración.

1. Ambos métodos aproximaron correctamente el valor exacto 2.
   El error fue aproximadamente 1.65e-12 para trapecio y menor
   que 1.4e-14 para Simpson.
2. Dos hilos obtuvieron la menor mediana: 0.017634 s para trapecio
   y 0.018399 s para Simpson. Las aceleraciones fueron 1.24 y 1.31,
   con reducciones de tiempo aproximadas de 19.3% y 23.9%.
3. Cuatro hilos no mejoraron los tiempos de dos en esta ejecución.
   Más hilos no garantizan mayor rapidez: influyen la coordinación,
   los recursos disponibles y la carga del computador.
4. La mejora observada con un solo hilo no implica trabajo simultáneo.
   Puede deberse a diferencias del código compilado y variabilidad temporal.
5. reduction evita condiciones de carrera en las sumas.
   El orden de suma puede cambiar las últimas cifras por redondeo.
6. Estos resultados corresponden a este equipo, tamaño del problema
   y conjunto de mediciones; no constituyen una ventaja universal.

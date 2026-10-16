# Practica de OpenMP

Basada en la guia OpenMP del profesor Alexander Perez, MCEI_M.

Desde la raiz del repositorio:

```bash
bash C_C++/openmp/ejecutar.sh
```

Requiere GCC con OpenMP y herramientas de Ubuntu. No necesita Python.

## Objetivo

Calcular integral de sin(x) entre 0 y pi, cuyo valor exacto es 2,
mediante Simpson 1/3 y trapecio, con un millon de subintervalos.
Simpson requiere n par; el programa fija n=1000000.

Trapecio: I = h[(f(a)+f(b))/2 + suma de puntos interiores].
Simpson: I = (h/3)[f(a)+f(b)+4*suma_impar+2*suma_par].

## Paralelizacion

Cada evaluacion de sin(x_i) es independiente. El indice de bucle y las
variables declaradas dentro son privados. Los limites y el paso son compartidos
solo para lectura. reduction(+:pares,impares) crea acumuladores privados y combina
sus resultados al final, evitando condiciones de carrera. schedule(static)
reparte las iteraciones en bloques; cada iteracion tiene un costo similar.

Se utiliza una region parallel con un for interno, equivalente en reparto del
bucle a parallel for, para registrar tambien el numero real de hilos del equipo.
En trapecio se mantienen dos sumas de peso identico, equivalentes a la suma unica
de la guia. Esto permite reutilizar el calculo y comparar versiones que hacen
el mismo trabajo. Los extremos se evaluan fuera del bucle.

## Medicion

Se comparan una version realmente secuencial y versiones OpenMP con 1, 2 y 4
hilos solicitados. Se registra tambien el numero real de hilos. Se desactiva el
ajuste dinamico. Cada configuracion se calienta y se mide 9 veces con
omp_get_wtime; se rota el orden. Se presenta la mediana. Se incluyen los costos
de la region paralela y la reduccion; se excluyen compilacion, archivos e impresion.

Speedup = mediana_secuencial / mediana_paralela.
Eficiencia = speedup / numero_real_de_hilos.

Se guardan en datos/openmp: resultados.csv, tiempos_repeticiones.csv,
entorno.txt y salida.txt. Las mediciones corresponden al computador que ejecuta
el programa; cerrar aplicaciones pesadas ayuda a reducir variabilidad.

## Interpretacion

Un speedup mayor que 1 indica aceleracion; menor que 1 indica que la version
paralela tardo mas. No se garantiza que cuatro hilos sean cuatro veces mas
rapidos: influyen coordinacion, procesadores fisicos/logicos, frecuencia y carga.

Ambos metodos deben aproximar 2. Se comprueba un error absoluto menor que 1e-8
para esta malla. Simpson tiene orden teorico cuatro y trapecio dos para funciones
suaves; con una malla muy fina, el redondeo puede dominar. Cambiar el orden de
sumacion en paralelo puede alterar las ultimas cifras, sin implicar una carrera.

## Conclusiones pendientes de las mediciones locales

El metodo matematico se conserva al repartir las evaluaciones independientes.
La reduccion protege las sumas compartidas. La configuracion mas rapida y la
aceleracion obtenida deben determinarse con resultados.csv, no suponerse.

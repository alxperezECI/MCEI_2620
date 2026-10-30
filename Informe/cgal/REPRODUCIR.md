# Reproducción del taller

Ejecutar desde la raíz del repositorio en Ubuntu/WSL.

## Dependencias

    sudo apt update
    sudo apt install -y g++ cmake libcgal-dev libgmp-dev libmpfr-dev python3-venv

Si no existe un entorno virtual, crearlo:

    python3 -m venv .venv

Activarlo e instalar las bibliotecas de visualización:

    source .venv/bin/activate
    python -m pip install numpy matplotlib

Las versiones utilizadas se registran en datos/cgal/entorno.txt.
La instalación sin versiones fijadas puede proporcionar versiones
diferentes; consultar ese archivo para comparar entornos.

## Compilación

    cmake -S C_C++/cgal -B C_C++/cgal/build -DCMAKE_BUILD_TYPE=Release
    cmake --build C_C++/cgal/build -j 2

## Experimentos aleatorios

    python Python/cgal/experimentar.py

Se generan 12 combinaciones de tamaño y semilla.
Cada combinación tiene una ejecución previa y cinco mediciones.
Los resultados agregados están en resumen_experimentos.csv y las
mediciones individuales en mediciones.csv, dentro de datos/cgal.

Los CSV geométricos de cada combinación corresponden a la última
ejecución y se comprueba su igualdad entre repeticiones.

## Casos especiales

    set -o pipefail
    C_C++/cgal/build/casos_especiales | tee datos/cgal/casos_especiales.csv

La columna verificado debe valer 1 para los tres casos.

## Precisión numérica

    C_C++/cgal/build/precision | tee datos/cgal/precision.csv

## Sensores del profesor

El CSV original debe estar en:

    datos/cgal/ubicaciones_100_sensores_xy.csv

Ejecutar:

    C_C++/cgal/build/sensores 100 0 datos/cgal/sensores_profesor

El argumento 0 ocupa la posición de semilla, pero no se utiliza
para generar datos: este programa lee el CSV.
Rechaza identificadores repetidos, posiciones duplicadas y
coordenadas no finitas.

## Gráficas

    for n in 100 500 1000 5000; do
        python Python/cgal/graficar.py "n${n}_s42" || break
    done
    python Python/cgal/graficar.py sensores_profesor
    python Python/cgal/graficar_resultados.py

Las imágenes se guardan en Informe/cgal/figuras.

## Tabla del informe

    python Python/cgal/documentar_resultados.py

Este script actualiza Resultados.md a partir de los CSV.
Si se modifican parámetros o datos, también deben revisarse las
conclusiones narrativas, que no se recalculan automáticamente.

## Advertencias de reproducción

Una nueva ejecución sobrescribe los archivos de resultados de
los mismos casos. Los tiempos pueden variar entre ejecuciones.

Los archivos de salida permiten revisar resultados sin ejecutar
el programa. El directorio build está excluido de Git.

La reproducibilidad exacta de los archivos se comprobó en el mismo
entorno; no se garantiza entre plataformas, compiladores o versiones.

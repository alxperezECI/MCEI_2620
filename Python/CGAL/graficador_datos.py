import matplotlib.pyplot as plt
import matplotlib.pyplot as plt
import sys
def graficar_datos_cgal(nombre_archivo):
    puntos_x, puntos_y = [], [] # Vectores para almacenar las coordenadas de los puntos
    hull_x, hull_y = [], [] # Vectores para almacenar las coordenadas de la envolvente convexa
    aristas = [] # Vector para almacenar la cantidad de aristas de la triangulación de Delaunay

    seccion_actual = None

    # Leer el archivo
    try:
        with open(nombre_archivo, 'r') as f:
            for linea in f: 
                linea = linea.strip()
                if not linea: continue

                # Detectar en qué sección de datos estamos para dividir correctamente los mismos
                if linea.startswith('# points:'):
                    seccion_actual = 'puntos'
                    continue
                elif linea.startswith('# convex_hull:'):
                    seccion_actual = 'hull'
                    continue
                elif linea.startswith('# delaunay_edges:'):
                    seccion_actual = 'aristas'
                    continue
                # Se ignora el resumen de texto inicial
                elif linea.startswith('Puntos') or linea.startswith('Vertices') or linea.startswith('Aristas') or linea.startswith('Caras'):
                    continue

                # Extraer las coordenadas
                partes = linea.split()
                if seccion_actual == 'puntos' and len(partes) == 2:
                    puntos_x.append(float(partes[0]))
                    puntos_y.append(float(partes[1]))
                elif seccion_actual == 'hull' and len(partes) == 2:
                    hull_x.append(float(partes[0]))
                    hull_y.append(float(partes[1]))
                elif seccion_actual == 'aristas' and len(partes) == 4:
                    aristas.append((float(partes[0]), float(partes[1]), float(partes[2]), float(partes[3])))
    except FileNotFoundError:
        print(f"Error: No se pudo encontrar el archivo '{nombre_archivo}'.")
        return

    # Cerrar el polígono de la envolvente conectando el último punto con el primero
    if hull_x:
        hull_x.append(hull_x[0])
        hull_y.append(hull_y[0])

    # Calcular la cantidad de puntos en la frontera restando el punto duplicado
    puntos_frontera = len(hull_x) - 1
    print(f"Puntos en la frontera de la envolvente: {puntos_frontera}")
    print(f"Puntos en el interior: {len(puntos_x) - puntos_frontera}")

    # Configurar la gráfica
    plt.figure(figsize=(10, 10))

    # Se grafican las aristas de la triangulación de Delaunay
    for x1, y1, x2, y2 in aristas:
        plt.plot([x1, x2], [y1, y2], color='lightgray', linewidth=0.8, zorder=1)

    # Se grafica la envolvente convexa
    plt.plot(hull_x, hull_y, color='red', linewidth=2, label='Envolvente Convexa', zorder=2)

    # Se grafica la nube de puntos 
    plt.scatter(puntos_x, puntos_y, color='blue', s=10, label='Nube de Puntos', zorder=3)

    plt.title('Triangulación de Delaunay y Envolvente Convexa')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.legend()
    plt.axis('equal') # Se mantiene la proporción geométrica 1 a 1
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.show()

def main():
    # Toma el archivo desde la consola, o usa uno por defecto si no se especifica
    if len(sys.argv) > 1:
        archivo = sys.argv[1]
    else:
        archivo = '100_puntos_semilla_42.txt'
    
    print(f"Generando gráfica")
    graficar_datos_cgal(archivo)


main()
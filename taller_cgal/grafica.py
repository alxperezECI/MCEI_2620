import matplotlib.pyplot as plt
import sys

archivo = sys.argv[1] if len(sys.argv) > 1 else "resultados_sensores.txt"

puntos_x, puntos_y = [], []
hull_x, hull_y = [], []
aristas = []
seccion = None

with open(archivo, "r") as f:
    for linea in f:
        linea = linea.strip()
        if not linea: continue
        
        if linea.startswith("# points"): seccion = "points"; continue
        elif linea.startswith("# convex_hull"): seccion = "hull"; continue
        elif linea.startswith("# delaunay_edges"): seccion = "edges"; continue
            
        datos = linea.split()
        if seccion == "points":
            puntos_x.append(float(datos[0])); puntos_y.append(float(datos[1]))
        elif seccion == "hull":
            hull_x.append(float(datos[0])); hull_y.append(float(datos[1]))
        elif seccion == "edges":
            aristas.append((float(datos[0]), float(datos[1]), float(datos[2]), float(datos[3])))

plt.figure(figsize=(10, 8))
for x1, y1, x2, y2 in aristas:
    plt.plot([x1, x2], [y1, y2], 'r-', linewidth=0.5, alpha=0.6)
plt.scatter(puntos_x, puntos_y, s=15, c='blue', zorder=5, label='Sensores')
if hull_x:
    hull_x.append(hull_x[0]); hull_y.append(hull_y[0])
    plt.plot(hull_x, hull_y, 'g-', linewidth=2, label='Envolvente Convexa')

plt.title("Red de 100 Sensores: Envolvente y Delaunay")
plt.xlabel("Coordenada X (m)")
plt.ylabel("Coordenada Y (m)")
plt.legend()
plt.grid(True, linestyle='--', alpha=0.5)

# Guardar la imagen
plt.savefig('grafica_sensores.png', dpi=300, bbox_inches='tight')

plt.show()

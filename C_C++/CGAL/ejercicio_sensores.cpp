#include <iterator>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Se utiliza un Kernel que garantiza la exactitud de los predicados, es decir, de las decisiones geométricas, pero permite aproximaciones
// en las construcciones, como el cálculo de nuevas coordenadas
// Da un equilibrio entre confiabilidad y rendimiento
using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = Kernel::Point_2;

// Triangulación de Delaunay:
// Esta estructura permite conectar los puntos que representan los sensores mediante triángulos. Sus aristas permiten identificar qué sensores son
// vecinos y cuáles tienen una relación de proximidad entre sí.
// Estas conexiones pueden utilizarse como base para organizar una red de comunicación entre los dispositivos
using Delaunay = CGAL::Delaunay_triangulation_2<Kernel>;

// Adaptadores para extraer el Diagrama de Voronoi a partir de Delaunay
// El diagrama de Voronoi se obtiene a partir de la triangulación de Delaunay.
// Cada región de Voronoi corresponde a un sensor y está formada por los
// puntos del espacio que se encuentran más cerca de ese sensor que de los demás.
// Los adaptadores permiten transformar la información de Delaunay para construir y consultar las regiones de Voronoi
using Voronoi_traits = CGAL::Delaunay_triangulation_adaptation_traits_2<Delaunay>;
using Voronoi_policy = CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<Delaunay>;
using Voronoi = CGAL::Voronoi_diagram_2<Delaunay, Voronoi_traits, Voronoi_policy>;

int main() {
    // Se carga un archivo CSV que contiene la identificación y las coordenadas
    // X e Y de los 100 sensores
    std::string filename = "ubicaciones_100_sensores_xy.csv";
    std::ifstream file(filename);

    // Se comprueba que el archivo exista
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return 1;
    }

    // Vector que almacena las posiciones de los sensores como puntos 2D
    std::vector<Point> puntos_sensores;
    std::string line;

    // Se omite la primera línea del archivo porque contiene los encabezados
    std::getline(file, line);

    // Se recorren las demás líneas del archivo para obtener las coordenadas de cada sensor
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, x_str, y_str;

        // Se separan los datos de cada sensor utilizando la coma como delimitador
        std::getline(ss, id, ',');
        std::getline(ss, x_str, ',');
        std::getline(ss, y_str, ',');

        // Se verifica que las coordenadas no estén vacías y se convierten de texto a números para crear un punto geométrico
        if (!x_str.empty() && !y_str.empty()) {
            puntos_sensores.emplace_back(std::stod(x_str), std::stod(y_str));
        }
    }

    // Si existen posiciones repetidas, CGAL puede representarlas como un único vértice al construir la triangulación
    std::cout << "Se cargaron exitosamente " << puntos_sensores.size() << " sensores.\n\n";


    // Triangulación de Delaunay (enlaces de comunicación)
    // Se crea la triangulación y se insertan las posiciones de todos los sensores
    // CGAL conecta los puntos formando triángulos. Las aristas representan relaciones de proximidad entre sensores
    Delaunay dt;
    dt.insert(puntos_sensores.begin(), puntos_sensores.end());

    std::cout << "Análisis de topología (Delaunay)" << std::endl;

    // Número de vértices de la triangulación.
    // Cada vértice representa una posición distinta de un sensor. Este valor permite comprobar cuántos sensores quedaron representados
    // en la estructura geométrica
    std::cout << "Nodos en la red: " << dt.number_of_vertices() << std::endl;

    // Se cuenta el número de aristas de la triangulación.
    // Cada arista conecta dos sensores vecinos y representa una posible relación de proximidad que podría utilizarse para establecer comunicación.
    // Las aristas infinitas asociadas al exterior de la triangulación no se cuentan
    std::cout << "Enlaces de comunicación (aristas): " << std::distance(dt.finite_edges_begin(), dt.finite_edges_end()) << std::endl;

    // Visualizar las primeras 5 rutas de comunicación generadas
    // Se recorren las aristas de la triangulación para mostrar algunos ejemplos de conexiones entre sensores vecinos.
    // Estas conexiones son formas para organizar la comunicación, aunque no garantizan que los dispositivos puedan comunicarse,
    // para ello también sería necesario considerar el alcance de sus transmisores
    std::cout << "\nEjemplo de rutas vecinas establecidas:" << std::endl;
    int edge_count = 0;
    for (auto edge = dt.finite_edges_begin(); edge != dt.finite_edges_end() && edge_count < 5; ++edge, ++edge_count) {
        // Cada arista se representa mediante una cara triangular y el índice
        // del vértice opuesto a dicha arista
        auto face = edge->first;
        int index = edge->second;

        // Se obtienen los dos extremos de la arista
        // Estos puntos corresponden a las posiciones de los sensores conectados
        Point p1 = face->vertex((index + 1) % 3)->point();
        Point p2 = face->vertex((index + 2) % 3)->point();

        // Se muestran las coordenadas de los dos sensores que forman cada conexión
        std::cout << " -> Enlace de (" << p1.x() << ", " << p1.y() 
                  << ") a (" << p2.x() << ", " << p2.y() << ")\n";
    }

    // Diagrama de Voronoi (distribución espacial)
    // Se construye el diagrama de Voronoi utilizando la triangulación de Delaunay.
    // Se obtiene una región de influencia para cada sensor. Cada región contiene las posiciones del espacio que están más cerca
    // del sensor correspondiente que de cualquier otro sensor
    Voronoi vd(dt);

    std::cout << "\nAnálisis de Cobertura Espacial (Voronoi)" << std::endl;

    // Se obtiene el número de regiones o caras del diagrama de Voronoi.
    // Cada cara corresponde a una posición de sensor.
    // Si los 100 sensores tienen coordenadas diferentes, se esperan 100 regiones
    std::cout << "Zonas de influencia exclusivas: " << vd.number_of_faces() << std::endl;

    // Las caras (zonas) pueden ser acotadas (polígonos cerrados) o no acotadas (infinitas hacia los bordes).
    // Las regiones acotadas suelen corresponder a sensores ubicados en zonas interiores
    // de la distribución, mientras que las no acotadas corresponden a sensores situados en el contorno de la envolvente convexa de los puntos.
    // Esta clasificación permite conocer cómo se distribuyen espacialmente los sensores
    int zonas_acotadas = 0;
    int zonas_infinitas = 0;
    for (auto face = vd.faces_begin(); face != vd.faces_end(); ++face) {
        // Si la región no tiene límites cerrados, se clasifica como no acotada
        if (face->is_unbounded()) {
            zonas_infinitas++;
        } else {
            // Si la región está completamente delimitada, se clasifica como acotada
            zonas_acotadas++;
        }
    }

    // Se muestran los resultados del análisis de las regiones de Voronoi.
    // Las zonas acotadas permiten realizar posteriormente cálculos de área.
    // Las zonas muy grandes podrían indicar una separación grande entre sensores,
    // mientras que las zonas pequeñas pueden indicar que hay mucha concentración de dispositivos.
    // Este código únicamente cuenta las regiones, no calcula sus áreas ni identifica directamente cuáles son grandes o pequeñas
    std::cout << "Zonas centrales (polígonos cerrados donde se puede medir el área): " << zonas_acotadas << std::endl;
    std::cout << "Zonas perimetrales (se extienden hacia el exterior de la región): " << zonas_infinitas << std::endl;

    // Delaunay permite analizar la proximidad entre sensores y definir posibles conexiones para organizar la red de comunicación.
    // Voronoi permite estudiar las zonas de influencia de cada sensor e identificar posibles áreas con pocos o demasiados dispositivos.

    // Si un sensor deja de funcionar, la triangulación podría recalcularse sin ese puntopara estudiar nuevas conexiones entre los sensores restantes

    return 0;
}
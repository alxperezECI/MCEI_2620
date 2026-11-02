#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <iterator>
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <chrono> // Para medir el tiempo de ejecución

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = Kernel::Point_2;
using Delaunay = CGAL::Delaunay_triangulation_2<Kernel>;

int main(int argc, char* argv[])
{
    // Numero de puntos y semilla opcionales desde la linea de comandos.
    const std::size_t n = (argc > 1) ? static_cast<std::size_t>(std::stoul(argv[1])) : 100;
    const unsigned int seed = (argc > 2) ? static_cast<unsigned int>(std::stoul(argv[2])) : 42u;

    // Generador reproducible de coordenadas en [0, 100].
    std::mt19937 generator(seed); // Se utiliza un generador de números aleatorios Mersenne Twister con una semilla fija para obtener resultados reproducibles
    std::uniform_real_distribution<double> distribution(0.0, 100.0); // Se define una distribución uniforme para generar coordenadas aleatorias en el rango [0, 100].

    std::vector<Point> points;
    points.reserve(n);

    // ciclo para generar n puntos aleatorios en el rango [0, 100] para ambas coordenadas
    for (std::size_t i = 0; i < n; ++i) {
        points.emplace_back(distribution(generator), distribution(generator));
    }

    // Calculo de la envolvente convexa.
    auto start_hull = std::chrono::high_resolution_clock::now(); // Se empieza a medir el tiempo de ejecución de la envolvente convexa
    std::vector<Point> hull; // Vector para almacenar los vértices de la envolvente convexa
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(hull));
    auto end_hull = std::chrono::high_resolution_clock::now(); // Se termina de medir el tiempo de ejecución de la envolvente
    auto duration_hull = std::chrono::duration_cast<std::chrono::microseconds>(end_hull - start_hull).count(); // Se calcula la duración en microsegundos de la envolvente convexa

    // Construccion de la triangulacion de Delaunay.
    Delaunay dt; // Se crea un objeto de la clase Delaunay para almacenar la triangulación de los puntos generados
    auto start_delaunay = std::chrono::high_resolution_clock::now(); // Se empieza a medir el tiempo de ejecución de la triangulación de Delaunay
    dt.insert(points.begin(), points.end());  // Se insertan los puntos generados en la triangulación de Delaunay
    auto end_delaunay = std::chrono::high_resolution_clock::now(); // Se termina de medir el tiempo de ejecución de la triangulación de Delaunay
    auto duration_delaunay = std::chrono::duration_cast<std::chrono::microseconds>(end_delaunay - start_delaunay).count(); // Se calcula la duración en microsegundos de la triangulación de Delaunay

    // Resumen de resultados.
    std::cout << "Puntos generados: " << points.size() << '\n';
    std::cout << "Vertices de la envolvente: " << hull.size() << '\n';
    std::cout << "Vertices distintos en Delaunay: " << dt.number_of_vertices() << '\n';
    std::cout << "Aristas finitas de Delaunay: " << std::distance(dt.finite_edges_begin(), dt.finite_edges_end()) << '\n';
    std::cout << "Caras finitas de Delaunay: " << dt.number_of_faces() << '\n';
    std::cout << "Tiempo Envolvente Convexa: " << duration_hull << " microsegundos\n";
    std::cout << "Tiempo Triangulacion Delaunay: " << duration_delaunay << " microsegundos\n";

    // Exportar puntos originales.
    std::cout << "\n# points: x y\n";
    for (const auto& p : points) {
        std::cout << p.x() << " " << p.y() << '\n';
    }

    // Exportar vertices de la envolvente en orden ciclico.
    std::cout << "\n# convex_hull: x y\n";
    for (const auto& p : hull) {
        std::cout << p.x() << " " << p.y() << '\n';
    }

    // Exportar aristas finitas de la triangulacion.
    std::cout << "\n# delaunay_edges: x1 y1 x2 y2\n";
    for (auto edge = dt.finite_edges_begin(); edge != dt.finite_edges_end(); ++edge) {
        auto face = edge->first;
        int index = edge->second;
        Point p = face->vertex((index + 1) % 3)->point();
        Point q = face->vertex((index + 2) % 3)->point();
        std::cout << p.x() << " " << p.y() << " " << q.x() << " " << q.y() << '\n';
    }

    return 0;
}
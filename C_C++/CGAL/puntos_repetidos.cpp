#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <iterator>
#include <iostream>
#include <vector>
#include <chrono> // Para medir el tiempo de ejecución

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel; // Definición del kernel de CGAL para geometría computacional
using Point = Kernel::Point_2; // Definición del tipo de punto 2D utilizando el kernel definido anteriormente
using Delaunay = CGAL::Delaunay_triangulation_2<Kernel>; // Definición del tipo de triangulación de Delaunay utilizando el kernel definido anteriormente

int main(int argc, char* argv[])
{
    // Número de puntos a generar, con valor por defecto de 100 si no se proporciona un argumento
    const std::size_t n = (argc > 1) ? static_cast<std::size_t>(std::stoul(argv[1])) : 100; // Al no ser aleatorio, no requiere semilla para el generador de números aleatorios

    std::vector<Point> points; // Vector para almacenar los puntos generados
    points.reserve(n); // Reservar espacio para n puntos en el vector para evitar realocaciones innecesarias

    //Este es el experimento 1, que consiste en generar n puntos repetidos en la misma coordenada (50.0, 50.0)

    // ciclo para generar n puntos repetidos en la misma coordenada (50.0, 50.0)
    for (std::size_t i = 0; i < n; ++i) {
        points.emplace_back(50.0, 50.0); 
    }

    // Calculo de la envolvente Convexa
    auto start_hull = std::chrono::high_resolution_clock::now(); // Se empieza a medir el tiempo de ejecución de la envolvente convexa
    
    std::vector<Point> hull; // Vector para almacenar los puntos de la envolvente convexa
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(hull)); // Se calcula la envolvente convexa de los puntos generados y se almacenan en el vector hull
    
    auto end_hull = std::chrono::high_resolution_clock::now(); // Se termina de medir el tiempo de ejecución de la envolvente convexa
    auto duration_hull = std::chrono::duration_cast<std::chrono::microseconds>(end_hull - start_hull).count(); // Se calcula la duración de la ejecución de la envolvente en microsegundos

    // Triangulación de Delaunay
    Delaunay dt; // Se crea un objeto de la clase Delaunay para almacenar la triangulación de Delaunay de los puntos generados
    
    auto start_delaunay = std::chrono::high_resolution_clock::now(); // Se empieza a medir el tiempo de ejecución de la triangulación de Delaunay
    
    dt.insert(points.begin(), points.end()); // Se insertan los puntos generados en la triangulación
    
    auto end_delaunay = std::chrono::high_resolution_clock::now(); // Se termina de medir el tiempo de ejecución de la triangulación
    auto duration_delaunay = std::chrono::duration_cast<std::chrono::microseconds>(end_delaunay - start_delaunay).count(); // Se calcula la duración de la ejecución de la triangulación en microsegundos

    // Resultados
    std::cout << "Experimento 1: Puntos Repetidos\n";
    std::cout << "Puntos generados duplicados: " << points.size() << '\n';
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
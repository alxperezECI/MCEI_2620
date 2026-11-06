#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <algorithm>
#include <iostream>
#include <fstream>  // Para leer archivos
#include <sstream>  // Para separar por comas
#include <string>
#include <vector>
#include <iterator> // Para std::distance

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = Kernel::Point_2;
using Delaunay = CGAL::Delaunay_triangulation_2<Kernel>;

int main(int argc, char* argv[]) {
    // Verificar que se pasó el nombre del archivo por consola
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo.csv>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << '\n';
        return 1;
    }

    std::vector<Point> points;
    std::string line;
    
    // Ignorar la primera línea (cabecera: sensor_id,x_m,y_m)
    std::getline(file, line);

    // Leer línea por línea
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, x_str, y_str;
        
        // Separar por comas
        std::getline(ss, id, ',');
        std::getline(ss, x_str, ',');
        std::getline(ss, y_str, ',');
        
        // Convertir a double y agregar a los puntos
        if (!x_str.empty() && !y_str.empty()) {
            double x = std::stod(x_str);
            double y = std::stod(y_str);
            points.emplace_back(x, y);
        }
    }
    file.close();

    // Calculo de la envolvente convexa.
    std::vector<Point> hull;
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(hull));

    // Construccion de la triangulacion de Delaunay.
    Delaunay dt;
    dt.insert(points.begin(), points.end());

    // Resumen de resultados.
    std::cout << "Puntos cargados (sensores): " << points.size() << '\n';
    std::cout << "Vertices de la envolvente: " << hull.size() << '\n';
    std::cout << "Vertices distintos en Delaunay: " << dt.number_of_vertices() << '\n';
    
    auto num_edges = std::distance(dt.finite_edges_begin(), dt.finite_edges_end());
    std::cout << "Aristas finitas de Delaunay: " << num_edges << '\n';
    
    std::cout << "Caras finitas de Delaunay: " << dt.number_of_faces() << '\n';

    // Exportar puntos originales.
    std::cout << "\n# points: x y\n";
    for (const auto& p : points) {
        std::cout << p.x() << ' ' << p.y() << '\n';
    }

    // Exportar vertices de la envolvente.
    std::cout << "\n# convex_hull: x y\n";
    for (const auto& p : hull) {
        std::cout << p.x() << ' ' << p.y() << '\n';
    }

    // Exportar aristas finitas.
    std::cout << "\n# delaunay_edges: x1 y1 x2 y2\n";
    for (auto edge = dt.finite_edges_begin(); edge != dt.finite_edges_end(); ++edge) {
        auto face = edge->first;
        int index = edge->second;
        Point p = face->vertex((index + 1) % 3)->point();
        Point q = face->vertex((index + 2) % 3)->point();
        std::cout << p.x() << ' ' << p.y() << ' ' << q.x() << ' ' << q.y() << '\n';
    }

    return 0;
}
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = K::Point_2;
using Delaunay = CGAL::Delaunay_triangulation_2<K>;

void estudiar(const std::string& nombre,
              const std::vector<Point>& puntos,
              int dimension_esperada, int vertices_esperados,
              int hull_esperado, int aristas_esperadas,
              int caras_esperadas) {
    std::vector<Point> hull;
    CGAL::convex_hull_2(
        puntos.begin(), puntos.end(), std::back_inserter(hull));

    Delaunay dt;
    dt.insert(puntos.begin(), puntos.end());

    const auto v = static_cast<long long>(dt.number_of_vertices());
    const auto e = std::distance(
        dt.finite_edges_begin(), dt.finite_edges_end());
    const auto f = std::distance(
        dt.finite_faces_begin(), dt.finite_faces_end());

    const bool correcta =
        dt.is_valid() &&
        dt.dimension() == dimension_esperada &&
        v == vertices_esperados &&
        static_cast<int>(hull.size()) == hull_esperado &&
        e == aristas_esperadas &&
        f == caras_esperadas &&
        v - e + f == 1;

    std::cout << nombre << ',' << puntos.size() << ','
              << dt.dimension() << ',' << hull.size() << ','
              << v << ',' << e << ',' << f << ','
              << v - e + f << ',' << correcta << '\n';

    if (!correcta)
        throw std::runtime_error("Fallo en el caso " + nombre);
}

int main() {
    try {
        std::cout << "caso,puntos_entrada,dimension,vertices_hull,"
                     "vertices_distintos,aristas_finitas,caras_finitas,"
                     "euler,verificado\n";

        estudiar("repetidos",
            {{0,0}, {2,0}, {2,2}, {0,2}, {1,1},
             {0,0}, {2,2}, {1,1}},
            2, 5, 4, 8, 4);

        estudiar("colineales",
            {{0,0}, {1,1}, {2,2}, {3,3}, {4,4}},
            1, 5, 2, 4, 0);

        estudiar("cocirculares",
            {{0,0}, {2,0}, {2,2}, {0,2}},
            2, 4, 4, 5, 2);

        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}

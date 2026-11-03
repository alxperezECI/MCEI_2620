#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <vector>
#include <random>
#include <iterator>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <filesystem>
 
using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = Kernel::Point_2;
using Delaunay = CGAL::Delaunay_triangulation_2<Kernel>;
 
int main(int argc, char* argv[]) {
    try {
        const std::size_t n = (argc > 1)
            ? std::stoul(argv[1]) : 100;
        const unsigned int seed = (argc > 2)
            ? static_cast<unsigned int>(std::stoul(argv[2])) : 42u;
        if (n < 3) {
            std::cerr << "Se requieren al menos 3 puntos.\n";
            return 1;
        }

        std::mt19937 generator(seed);
        std::uniform_real_distribution<double> distribution(0.0, 100.0);
        std::vector<Point> points;
        points.reserve(n);
 
        for (std::size_t i = 0; i < n; ++i) {
            double x = distribution(generator);
            double y = distribution(generator);
            points.emplace_back(x, y);
        }
 
        auto inicio = std::chrono::steady_clock::now();
 
        // Calcular envolvente convexa

        std::vector<Point> hull;
        CGAL::convex_hull_2(
            points.begin(),
            points.end(),
            std::back_inserter(hull)
        );
 
        auto fin_hull = std::chrono::steady_clock::now();
 
        // Calcular triangulacion de Delaunay
        Delaunay dt;

        dt.insert(points.begin(), points.end());
 
        auto fin_delaunay = std::chrono::steady_clock::now();

        // Exportar resultados geometricos para visualizacion
        std::filesystem::create_directories("resultados");
 
        // Guardar coordenadas de todos los puntos
        std::ofstream archivo_puntos("resultados/puntos.csv");
        archivo_puntos << "x,y\n";
        archivo_puntos << std::setprecision(17);
 
        for (const auto& p : points) {
            archivo_puntos << p.x() << "," << p.y() << "\n";
        }
        archivo_puntos.close();
 
        // Guardar vertices de la envolvente convexa
        std::ofstream archivo_hull("resultados/convex_hull.csv");
        archivo_hull << "x,y\n";
        archivo_hull << std::setprecision(17);
 
        for (const auto& p : hull) {
            archivo_hull << p.x() << "," << p.y() << "\n";
        }
        archivo_hull.close();
 
        // Guardar aristas finitas de Delaunay
        std::ofstream archivo_aristas("resultados/delaunay.csv");
        archivo_aristas << "x1,y1,x2,y2\n";
        archivo_aristas << std::setprecision(17);
 
        for (auto it = dt.finite_edges_begin();
             it != dt.finite_edges_end(); ++it) {
 
            auto segmento = dt.segment(*it);
 
            archivo_aristas
<< segmento.source().x() << ","
<< segmento.source().y() << ","
<< segmento.target().x() << ","
<< segmento.target().y() << "\n";
        }
        archivo_aristas.close();
 
        double tiempo_hull =
            std::chrono::duration<double, std::milli>(
                fin_hull - inicio
            ).count();
 
        double tiempo_delaunay =
            std::chrono::duration<double, std::milli>(
                fin_delaunay - fin_hull
            ).count();
 
        std::cout << std::fixed << std::setprecision(4);
 
        std::cout << "\n=== RESULTADOS CGAL ===\n";

        std::cout << "Semilla: " << seed << "\n";

        std::cout << "Puntos generados: " << points.size() << "\n";

        std::cout << "Vertices envolvente convexa: "
<< hull.size() << "\n";

        std::cout << "Vertices Delaunay: "
<< dt.number_of_vertices() << "\n";

        std::cout << "Aristas Delaunay: "
<< std::distance(
                 dt.finite_edges_begin(),
                 dt.finite_edges_end()
             ) << "\n";

        std::cout << "Caras finitas Delaunay: "
<< dt.number_of_faces() << "\n";
 
        std::cout << "Tiempo envolvente (ms): "
<< tiempo_hull << "\n";

        std::cout << "Tiempo Delaunay (ms): "
<< tiempo_delaunay << "\n";
 
        double proporcion =

            100.0 * hull.size() / points.size();
 
        std::cout << "Puntos en frontera (%): "
<< proporcion << "\n";
 
        return 0;

    }

    catch (const std::exception& e) {

        std::cerr << "Error: " << e.what() << "\n";

        return 1;

    }

}
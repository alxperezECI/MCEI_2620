#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <stdexcept>
 
using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;

using Point = Kernel::Point_2;

using Delaunay = CGAL::Delaunay_triangulation_2<Kernel>;
 
int main() {

    try {

        std::ifstream archivo("datos/ubicaciones_100_sensores_xy.csv");
 
        if (!archivo.is_open()) {

            throw std::runtime_error(

                "No se pudo abrir el archivo de sensores."

            );

        }
 
        std::vector<Point> puntos;

        std::string linea;
 
        // Omitir encabezado

        std::getline(archivo, linea);
 
        while (std::getline(archivo, linea)) {

            if (linea.empty()) continue;
 
            std::stringstream ss(linea);

            std::string id, sx, sy;
 
            std::getline(ss, id, ',');

            std::getline(ss, sx, ',');

            std::getline(ss, sy, ',');
 
            puntos.emplace_back(std::stod(sx), std::stod(sy));

        }
 
        archivo.close();
 
        if (puntos.size() < 3) {

            throw std::runtime_error("Se requieren al menos 3 sensores.");

        }
 
        // Calcular envolvente convexa

        auto inicio = std::chrono::steady_clock::now();
 
        std::vector<Point> hull;

        CGAL::convex_hull_2(

            puntos.begin(),

            puntos.end(),

            std::back_inserter(hull)

        );
 
        auto fin_hull = std::chrono::steady_clock::now();
 
        // Calcular triangulacion de Delaunay

        Delaunay dt;

        dt.insert(puntos.begin(), puntos.end());
 
        auto fin_delaunay = std::chrono::steady_clock::now();
 
        // Crear carpeta independiente para sensores

        std::filesystem::create_directories("resultados/sensores");
 
        const std::string ruta = "resultados/sensores/";
 
        // Exportar puntos

        std::ofstream archivo_puntos(ruta + "puntos.csv");

        archivo_puntos << "x,y\n";

        archivo_puntos << std::setprecision(17);
 
        for (const auto& p : puntos) {

            archivo_puntos << p.x() << "," << p.y() << "\n";

        }
 
        // Exportar envolvente convexa

        std::ofstream archivo_hull(ruta + "convex_hull.csv");

        archivo_hull << "x,y\n";

        archivo_hull << std::setprecision(17);
 
        for (const auto& p : hull) {

            archivo_hull << p.x() << "," << p.y() << "\n";

        }
 
        // Exportar aristas de Delaunay

        std::ofstream archivo_aristas(ruta + "delaunay.csv");

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
 
        // Mostrar resultados

        double tiempo_hull =

            std::chrono::duration<double, std::milli>(

                fin_hull - inicio

            ).count();
 
        double tiempo_delaunay =

            std::chrono::duration<double, std::milli>(

                fin_delaunay - fin_hull

            ).count();
 
        std::cout << std::fixed << std::setprecision(4);
 
        std::cout << "\n=== RESULTADOS SENSORES CGAL ===\n";

        std::cout << "Sensores cargados: " << puntos.size() << "\n";

        std::cout << "Vertices envolvente convexa: " << hull.size() << "\n";

        std::cout << "Vertices Delaunay: " << dt.number_of_vertices() << "\n";

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
 
        std::cout << "\nArchivos guardados en: "
<< ruta << "\n";
 
        return 0;
 
    } catch (const std::exception& e) {

        std::cerr << "Error: " << e.what() << "\n";

        return 1;

    }

}

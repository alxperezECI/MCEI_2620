#include <sstream>
#include <set>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = K::Point_2;
using Delaunay = CGAL::Delaunay_triangulation_2<K>;
using Clock = std::chrono::steady_clock;

int main(int argc, char* argv[]) {
    try {
        const int n = argc > 1 ? std::stoi(argv[1]) : 100;
        const int seed = argc > 2 ? std::stoi(argv[2]) : 42;
        if (n < 3 || seed < 0)
            throw std::invalid_argument("Use n >= 3 y semilla >= 0.");

        const std::filesystem::path carpeta =
            argc > 3 ? argv[3] : "datos/cgal/prueba";
        std::filesystem::create_directories(carpeta);


        std::ifstream entrada("datos/cgal/ubicaciones_100_sensores_xy.csv");
        if (!entrada)
            throw std::runtime_error("No se pudo abrir el CSV de sensores.");

        std::string linea;
        std::getline(entrada, linea);
        if (!linea.empty() && linea.back() == '\r')
            linea.pop_back();
        if (linea != "sensor_id,x_m,y_m")
            throw std::runtime_error("Encabezado inesperado en el CSV.");

        std::vector<Point> puntos;
        std::vector<std::string> ids;
        std::set<std::string> ids_unicos;
        std::set<std::pair<double, double>> posiciones;

        while (std::getline(entrada, linea)) {
            if (!linea.empty() && linea.back() == '\r')
                linea.pop_back();
            if (linea.empty()) continue;

            std::istringstream fila(linea);
            std::string id, sx, sy;
            std::getline(fila, id, ',');
            std::getline(fila, sx, ',');
            std::getline(fila, sy);

            std::size_t nx, ny;
            double x = std::stod(sx, &nx);
            double y = std::stod(sy, &ny);
            if (nx != sx.size() || ny != sy.size() ||
                !std::isfinite(x) || !std::isfinite(y))
                throw std::runtime_error("Coordenada invalida.");

            if (id.empty() || !ids_unicos.insert(id).second)
                throw std::runtime_error("Identificador vacio o repetido.");
            if (!posiciones.insert({x, y}).second)
                throw std::runtime_error("Dos sensores comparten posicion.");

            ids.push_back(id);
            puntos.emplace_back(x, y);
        }

        if (puntos.size() != static_cast<std::size_t>(n))
            throw std::runtime_error("Cantidad de sensores distinta de n.");

        std::ofstream identificadores(carpeta / "sensores_identificados.csv");
        if (!identificadores)
            throw std::runtime_error("No se pudo guardar la identificacion.");

        identificadores << std::setprecision(17) << "sensor_id,x_m,y_m\n";
        for (std::size_t i = 0; i < puntos.size(); ++i)
            identificadores << ids[i] << ',' << puntos[i].x()
                            << ',' << puntos[i].y() << '\n';

        std::vector<Point> envolvente;
        const auto t0 = Clock::now();
        CGAL::convex_hull_2(
            puntos.begin(), puntos.end(),
            std::back_inserter(envolvente)
        );
        const auto t1 = Clock::now();

        Delaunay dt;
        dt.insert(puntos.begin(), puntos.end());
        const auto t2 = Clock::now();

        const double tiempo_hull =
            std::chrono::duration<double, std::milli>(t1 - t0).count();
        const double tiempo_dt =
            std::chrono::duration<double, std::milli>(t2 - t1).count();

        const auto vertices = dt.number_of_vertices();
        const auto aristas = std::distance(
            dt.finite_edges_begin(), dt.finite_edges_end());
        const auto caras = std::distance(
            dt.finite_faces_begin(), dt.finite_faces_end());

        const bool valida = dt.is_valid();
        const long long euler = static_cast<long long>(vertices)
                              - aristas + caras;

        std::ofstream fp(carpeta / "puntos.csv");
        std::ofstream fh(carpeta / "envolvente.csv");
        std::ofstream fa(carpeta / "aristas.csv");
        std::ofstream fr(carpeta / "resumen.csv");
        if (!fp || !fh || !fa || !fr)
            throw std::runtime_error("No se pudieron crear los CSV.");

        fp << std::setprecision(17) << "x,y\n";
        fh << std::setprecision(17) << "x,y\n";
        fa << std::setprecision(17) << "x1,y1,x2,y2\n";

        for (const auto& p : puntos)
            fp << p.x() << ',' << p.y() << '\n';

        for (const auto& p : envolvente)
            fh << p.x() << ',' << p.y() << '\n';

        for (auto e = dt.finite_edges_begin();
             e != dt.finite_edges_end(); ++e) {
            const auto segmento = dt.segment(*e);
            const auto& p = segmento.source();
            const auto& q = segmento.target();
            fa << p.x() << ',' << p.y() << ','
               << q.x() << ',' << q.y() << '\n';
        }

        fr << "n,semilla_no_aplica,dimension,vertices_hull,vertices,"
              "aristas_finitas,caras_finitas,hull_ms,delaunay_ms,"
              "valida,euler\n";
        fr << std::setprecision(17)
           << n << ',' << seed << ',' << dt.dimension() << ','
           << envolvente.size() << ',' << vertices << ','
           << aristas << ',' << caras << ','
           << tiempo_hull << ',' << tiempo_dt << ','
           << valida << ',' << euler << '\n';

        std::cout << "Sensores leidos: " << n
                  << "\nVertices de la envolvente: " << envolvente.size()
                  << "\nVertices distintos: " << vertices
                  << "\nAristas finitas: " << aristas
                  << "\nCaras finitas: " << caras
                  << "\nTriangulacion valida: " << std::boolalpha << valida
                  << "\nV - E + F (sin cara exterior): " << euler
                  << "\nTiempo envolvente: " << tiempo_hull << " ms"
                  << "\nTiempo Delaunay: " << tiempo_dt << " ms"
                  << "\nArchivos guardados en: " << carpeta << '\n';

        return valida && euler == 1 ? 0 : 1;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}

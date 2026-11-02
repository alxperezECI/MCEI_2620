#include <random>
#include <cmath>
#include <chrono>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iterator>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point  = Kernel::Point_2;
using Delaunay = CGAL::Delaunay_triangulation_2<Kernel>;

void analizar(const std::vector<Point>& pts)
{
    auto t0 = std::chrono::steady_clock::now();

    std::vector<Point> contorno;
    CGAL::convex_hull_2(pts.begin(), pts.end(), std::back_inserter(contorno));

    Delaunay triang;
    triang.insert(pts.begin(), pts.end());

    auto t1 = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    std::size_t aristas = std::distance(triang.finite_edges_begin(), triang.finite_edges_end());

    std::cout << "n=" << pts.size()
              << "  hull=" << contorno.size()
              << "  V=" << triang.number_of_vertices()
              << "  E=" << aristas
              << "  F=" << triang.number_of_faces()
              << "  tiempo(ms)=" << ms << '\n';
}

int main()
{
    std::ifstream archivo("/home/paula/MCEI_2620/ubicaciones_100_sensores_xy.csv");
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo\n";
        return 1;
    }

    std::string linea;
    std::getline(archivo, linea);   // descarta el encabezado

    std::vector<Point> points;

    // Lee cada sensor: S001,63.943,2.501
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string id, sx, sy;

        std::getline(ss, id, ',');   // S001 (no se usa)
        std::getline(ss, sx, ',');   // "63.943" como texto
        std::getline(ss, sy, ',');   // "2.501" como texto

        double x = std::stod(sx);    // texto -> número
        double y = std::stod(sy);

        points.emplace_back(x, y);   // agrega el punto a la lista
    }

        std::vector<Point> hull;
        CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(hull));

    std::cout << "Vertices de la envolvente: " << hull.size() << '\n';

        std::cout << "\nVertices de la envolvente (x, y):\n";
    for (const auto& p : hull) {
        std::cout << p.x() << ' ' << p.y() << '\n';
    }


        for (std::size_t n : {100, 500, 1000, 5000}) 
        {
        for (unsigned int semilla : {1, 2, 3, 42, 2026}) 
        {
        std::mt19937 generador(semilla);
        std::uniform_real_distribution<double> distribucion(0.0, 100.0);

        std::vector<Point> aleatorios;
        for (std::size_t i = 0; i < n; ++i) {
            double x = distribucion(generador);
            double y = distribucion(generador);
            aleatorios.emplace_back(x, y);
        }

        analizar(aleatorios);
        }
    }

    Delaunay dt;
    dt.insert(points.begin(), points.end());

    std::size_t aristas = std::distance(dt.finite_edges_begin(), dt.finite_edges_end());

    std::cout << "\nVertices de Delaunay: " << dt.number_of_vertices() << '\n';
    std::cout << "Aristas de Delaunay: " << aristas << '\n';
    std::cout << "Caras (triangulos) de Delaunay: " << dt.number_of_faces() << '\n';

    std::cout << "Puntos leidos: " << points.size() << '\n';

        std::ofstream fpuntos("puntos.txt");
    for (const auto& p : points) {
        fpuntos << p.x() << ' ' << p.y() << '\n';
    }
    std::ofstream fhull("hull.txt");
    for (const auto& p : hull) {
        fhull << p.x() << ' ' << p.y() << '\n';
    }
    std::ofstream faristas("aristas.txt");
    for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e) {
        auto cara = e->first;
        int i = e->second;
        Point p = cara->vertex((i + 1) % 3)->point();
        Point q = cara->vertex((i + 2) % 3)->point();
        faristas << p.x() << ' ' << p.y() << ' ' << q.x() << ' ' << q.y() << '\n';
    }
    analizar(points);

        std::vector<Point> repetidos = points;
    repetidos.insert(repetidos.end(), points.begin(), points.end());

    std::cout << "\n--- Puntos repetidos ---\n";
    analizar(repetidos);

        std::vector<Point> colineales;
    for (int i = 0; i < 100; ++i) 
    {
        colineales.emplace_back(i, 2 * i);  // puntos colineales: y = 2x
    }

    std::cout << "\n--- Puntos colineales ---\n";
    analizar(colineales);

        std::vector<Point> malla;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            malla.emplace_back(i, j);
        }
    }

    std::cout << "\n--- Malla 10x10 (cocirculares) ---\n";
    analizar(malla);


        std::vector<Point> casi;
    for (int i = 0; i < 100; ++i) {
        double d = (i % 2 == 0) ? 1e-9 : 0;
        casi.emplace_back(i, i + d);
    }

    std::cout << "\n--- Casi colineales ---\n";
    analizar(casi);

        std::cout << "\n--- Robustez: A=(0,0) B=(1,1) C=(2,2+eps) ---\n";
    for (double eps : {1e-1, 1e-8, 1e-15, 4.4e-16, 2.2e-16, 1e-17}) {
        Point A(0, 0), B(1, 1), C(2, 2 + eps);
        auto o = CGAL::orientation(A, B, C);
        double det = (B.x() - A.x()) * (C.y() - A.y()) - (B.y() - A.y()) * (C.x() - A.x());

        std::cout << "eps=" << eps << "  det_ingenuo=" << det << "  CGAL: ";
        if (o == CGAL::COLLINEAR)       std::cout << "COLINEAL\n";
        else if (o == CGAL::LEFT_TURN)  std::cout << "IZQUIERDA\n";
        else                            std::cout << "DERECHA\n";
    }

        std::cout << "\n--- Cuadricula 256x256 alrededor de (0.5, 0.5) ---\n";
    const double u = std::ldexp(1.0, -53);
    Point q(12, 12), r(24, 24);
    int diferencias = 0, colineales = 0;

    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            Point p(0.5 + i * u, 0.5 + j * u);

            double det = (q.x() - p.x()) * (r.y() - p.y())
                       - (q.y() - p.y()) * (r.x() - p.x());
            int signo_ingenuo = (det > 0) - (det < 0);
            int signo_cgal = static_cast<int>(CGAL::orientation(p, q, r));

            if (signo_cgal == 0) ++colineales;
            if (signo_ingenuo != signo_cgal) ++diferencias;
        }
    }

    std::cout << "casos=65536  colineales exactos=" << colineales
              << "  diferencias=" << diferencias << '\n';

    return 0;
}

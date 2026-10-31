#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = Kernel::Point_2;
using Delaunay = CGAL::Delaunay_triangulation_2<Kernel>;

// Lee un CSV "id,x,y" con encabezado; ignora la primera columna.
std::vector<Point> leer_csv(const std::string &ruta)
{
    std::vector<Point> pts;
    std::ifstream f(ruta);
    if (!f)
    {
        std::cerr << "No se pudo abrir " << ruta << "\n";
        return pts;
    }
    std::string linea;
    std::getline(f, linea); // descarta encabezado
    while (std::getline(f, linea))
    {
        if (linea.empty())
            continue;
        std::stringstream ss(linea);
        std::string id, sx, sy;
        std::getline(ss, id, ',');
        std::getline(ss, sx, ',');
        std::getline(ss, sy, ',');
        pts.emplace_back(std::stod(sx), std::stod(sy));
    }
    return pts;
}

int main(int argc, char *argv[])
{
    std::vector<Point> points;

    if (argc > 1 && std::string(argv[1]).find(".csv") != std::string::npos)
    {
        points = leer_csv(argv[1]);
    }
    else
    {
        const std::size_t n = (argc > 1) ? std::stoul(argv[1]) : 100;
        const unsigned seed = (argc > 2) ? std::stoul(argv[2]) : 42u;
        std::mt19937 gen(seed);
        std::uniform_real_distribution<double> dist(0.0, 100.0);
        points.reserve(n);
        for (std::size_t i = 0; i < n; ++i)
            points.emplace_back(dist(gen), dist(gen));
    }

    // Envolvente convexa
    std::vector<Point> hull;
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(hull));

    // Triangulación de Delaunay
    Delaunay dt;
    dt.insert(points.begin(), points.end());

    // Resumen
    std::size_t num_edges = 0;
    for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e)
        ++num_edges;

    std::cout << "Puntos generados: " << points.size() << '\n';
    std::cout << "Vertices de la envolvente: " << hull.size() << '\n';
    std::cout << "Vertices distintos Delaunay: " << dt.number_of_vertices() << '\n';
    std::cout << "Aristas finitas Delaunay: " << num_edges << '\n';
    std::cout << "Caras finitas Delaunay: " << dt.number_of_faces() << '\n';

    // Exportar para graficar en Python
    std::ofstream fh("hull.csv");
    fh << "x,y\n";
    for (const auto &p : hull)
        fh << p.x() << ',' << p.y() << '\n';

    std::ofstream fe("delaunay_edges.csv");
    fe << "x1,y1,x2,y2\n";
    for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e)
    {
        auto fc = e->first;
        int i = e->second;
        Point p = fc->vertex((i + 1) % 3)->point();
        Point q = fc->vertex((i + 2) % 3)->point();
        fe << p.x() << ',' << p.y() << ',' << q.x() << ',' << q.y() << '\n';
    }
    std::ofstream fp("puntos.csv");
    fp << "x,y\n";
    for (const auto &p : points)
        fp << p.x() << ',' << p.y() << '\n';

    return 0;
}
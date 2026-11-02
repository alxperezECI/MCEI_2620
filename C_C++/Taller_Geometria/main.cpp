#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <vector>

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = Kernel::Point_2;
using Delaunay = CGAL::Delaunay_triangulation_2<Kernel>;
using Clock = std::chrono::steady_clock;

static double ms(Clock::time_point a, Clock::time_point b)
{
    return std::chrono::duration<double, std::milli>(b - a).count();
}

// Uso: ./taller_cgal [n=100] [seed=42] [modo=random] [archivo_salida]
// modo: random | duplicates | collinear | cocircular
int main(int argc, char *argv[])
{
    const std::size_t n = (argc > 1) ? std::stoul(argv[1]) : 100;
    const unsigned int seed = (argc > 2) ? std::stoul(argv[2]) : 42u;
    const std::string mode = (argc > 3) ? argv[3] : "random";
    const std::string outfile = (argc > 4) ? argv[4] : "";

    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> U(0.0, 100.0);
    std::uniform_real_distribution<double> ang(0.0, 2.0 * M_PI);

    std::vector<Point> points;
    points.reserve(n);

    if (mode == "random")
    {
        for (std::size_t i = 0; i < n; ++i)
            points.emplace_back(U(gen), U(gen));
    }
    else if (mode == "duplicates")
    {
        // La mitad de los puntos se repite exactamente.
        for (std::size_t i = 0; i < n; ++i)
        {
            if (i >= n / 2 && i > 0)
                points.push_back(points[i - n / 2]);
            else
                points.emplace_back(U(gen), U(gen));
        }
    }
    else if (mode == "collinear")
    {
        // Puntos exactamente colineales sobre y = 2x + 1 (enteros, sin error de redondeo).
        for (std::size_t i = 0; i < n; ++i)
            points.emplace_back(static_cast<double>(i), 2.0 * static_cast<double>(i) + 1.0);
    }
    else if (mode == "cocircular")
    {
        // Puntos sobre una circunferencia (cocirculares salvo redondeo).
        for (std::size_t i = 0; i < n; ++i)
        {
            double t = ang(gen);
            points.emplace_back(50.0 + 40.0 * std::cos(t), 50.0 + 40.0 * std::sin(t));
        }
    }
    else if (mode == "grid")
    {
        // Rejilla regular: muchos cuartetos exactamente cocirculares y colineales.
        std::size_t s = static_cast<std::size_t>(std::ceil(std::sqrt(static_cast<double>(n))));
        for (std::size_t i = 0; i < n; ++i)
            points.emplace_back(static_cast<double>(i % s), static_cast<double>(i / s));
    }
    else
    {
        std::cerr << "modo desconocido: " << mode << '\n';
        return 1;
    }

    auto t0 = Clock::now();
    std::vector<Point> hull;
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(hull));
    auto t1 = Clock::now();

    Delaunay dt;
    dt.insert(points.begin(), points.end());
    auto t2 = Clock::now();

    const std::size_t V = dt.number_of_vertices();
    const std::size_t F = dt.number_of_faces();
    const std::size_t E = std::distance(dt.finite_edges_begin(), dt.finite_edges_end());
    const std::size_t h = hull.size();
    // Euler (triangulacion plana, V-E+F = 1 contando solo caras finitas)
    const long euler = static_cast<long>(V) - static_cast<long>(E) + static_cast<long>(F);

    // Una unica linea CSV con el resumen
    std::cout << "mode,n,seed,V,hull,E,F,euler,t_hull_ms,t_delaunay_ms\n"
              << mode << ',' << n << ',' << seed << ',' << V << ',' << h << ',' << E << ','
              << F << ',' << euler << ',' << ms(t0, t1) << ',' << ms(t1, t2) << '\n';

    if (!outfile.empty())
    {
        std::ofstream out(outfile);
        out << "# points: x y\n";
        for (const auto &p : points)
            out << p.x() << ' ' << p.y() << '\n';
        out << "\n# convex_hull: x y\n";
        for (const auto &p : hull)
            out << p.x() << ' ' << p.y() << '\n';
        out << "\n# delaunay_edges: x1 y1 x2 y2\n";
        for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e)
        {
            auto f = e->first;
            int i = e->second;
            Point p = f->vertex((i + 1) % 3)->point();
            Point q = f->vertex((i + 2) % 3)->point();
            out << p.x() << ' ' << p.y() << ' ' << q.x() << ' ' << q.y() << '\n';
        }
    }
    return 0;
}
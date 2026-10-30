#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iomanip>
#include <iostream>

using KI = CGAL::Exact_predicates_inexact_constructions_kernel;
using KE = CGAL::Exact_predicates_exact_constructions_kernel;

const char* nombre(CGAL::Orientation orientacion) {
    if (orientacion == CGAL::LEFT_TURN) return "izquierda";
    if (orientacion == CGAL::RIGHT_TURN) return "derecha";
    return "colineales";
}

int main() {
    std::cout << std::setprecision(17);
    std::cout << "exponente,epsilon_double,cy_double,"
                 "determinante_double,orientacion_epick,"
                 "epeck_desde_double,epeck_racional\n";

    for (int exponente : {8, 12, 15, 16, 17, 20}) {
        double epsilon = 1.0;
        KE::FT epsilon_exacto(1);

        for (int i = 0; i < exponente; ++i) {
            epsilon /= 10.0;
            epsilon_exacto /= KE::FT(10);
        }

        const double cy = 2.0 + epsilon;
        const double determinante = cy - 2.0;

        const KI::Point_2 a(0, 0), b(1, 1), c(2, cy);
        const KE::Point_2 ae(0, 0), be(1, 1);
        const KE::Point_2 c_desde_double(2, cy);
        const KE::Point_2 c_exacto(
            KE::FT(2), KE::FT(2) + epsilon_exacto);

        std::cout << exponente << ',' << epsilon << ','
                  << cy << ',' << determinante << ','
                  << nombre(CGAL::orientation(a, b, c)) << ','
                  << nombre(CGAL::orientation(ae, be, c_desde_double))
                  << ','
                  << nombre(CGAL::orientation(ae, be, c_exacto))
                  << '\n';
    }
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

#include <gsl/gsl_spline.h>
#include <gsl/gsl_errno.h>

int main()
{
    // ========================================================
    // TALLER DE INTEGRACION Y DIFERENCIACION NUMERICA
    // Ejercicio 2 - Datos experimentales
    // C++ con GSL
    //
    // Jimmy Alexander Calderon Trujillo
    // Sebastian Barahona
    // ========================================================

    std::vector<double> x;
    std::vector<double> y;

    // --------------------------------------------------------
    // 1. LECTURA DEL ARCHIVO CSV
    // --------------------------------------------------------

    std::ifstream archivo("datos/datos_sensor.csv");

    if (!archivo.is_open())
    {
        std::cerr << "Error: no se pudo abrir datos_sensor.csv\n";
        return 1;
    }

    std::string linea;

    // Ignorar encabezado: x,y
    std::getline(archivo, linea);

    while (std::getline(archivo, linea))
    {
        std::stringstream ss(linea);
        std::string valor_x;
        std::string valor_y;

        std::getline(ss, valor_x, ',');
        std::getline(ss, valor_y, ',');

        x.push_back(std::stod(valor_x));
        y.push_back(std::stod(valor_y));
    }

    archivo.close();

    const std::size_t N = x.size();

    std::cout << std::fixed << std::setprecision(12);

    std::cout << "EJERCICIO 2 - DATOS EXPERIMENTALES\n";
    std::cout << "---------------------------------------------\n";
    std::cout << "Numero de mediciones : " << N << "\n";
    std::cout << "x inicial            : " << x.front() << "\n";
    std::cout << "x final              : " << x.back() << "\n";

    // --------------------------------------------------------
    // 2. VERIFICACION DEL ESPACIAMIENTO
    // --------------------------------------------------------

    double h = x[1] - x[0];
    bool uniforme = true;

    for (std::size_t i = 1; i < N - 1; ++i)
    {
        double hi = x[i + 1] - x[i];

        if (std::abs(hi - h) > 1e-12)
        {
            uniforme = false;
            break;
        }
    }

    std::cout << "Espaciado h          : " << h << "\n";
    std::cout << "Espaciado uniforme   : "
              << (uniforme ? "SI" : "NO") << "\n";

    // --------------------------------------------------------
    // 3. TRAPECIO DIRECTAMENTE SOBRE LOS DATOS
    // --------------------------------------------------------

    double I_trap = 0.0;

    for (std::size_t i = 0; i < N - 1; ++i)
    {
        I_trap +=
            (x[i + 1] - x[i]) *
            (y[i] + y[i + 1]) / 2.0;
    }

    std::cout << "\nINTEGRACION NUMERICA\n";
    std::cout << "---------------------------------------------\n";
    std::cout << "Trapecio directo     : "
              << I_trap << "\n";

    // --------------------------------------------------------
    // 4. INTERPOLACION CUBICA CON GSL
    // --------------------------------------------------------
    //
    // Los datos discretos se convierten en una representacion
    // continua mediante un spline cubico.
    // Luego GSL integra esa representacion interpolada.

    gsl_interp_accel* acelerador =
        gsl_interp_accel_alloc();

    gsl_spline* spline =
        gsl_spline_alloc(gsl_interp_cspline, N);

    gsl_spline_init(
        spline,
        x.data(),
        y.data(),
        N
    );

    double I_spline =
        gsl_spline_eval_integ(
            spline,
            x.front(),
            x.back(),
            acelerador
        );

    std::cout << "Spline cubico GSL    : "
              << I_spline << "\n";

    std::cout << "Diferencia absoluta  : "
              << std::abs(I_spline - I_trap) << "\n";

    // --------------------------------------------------------
    // 5. LIBERAR MEMORIA
    // --------------------------------------------------------

    gsl_spline_free(spline);
    gsl_interp_accel_free(acelerador);

    return 0;
}

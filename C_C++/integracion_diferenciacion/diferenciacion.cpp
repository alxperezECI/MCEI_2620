#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>

// ============================================================
// TALLER DE INTEGRACION Y DIFERENCIACION NUMERICA
// Diferenciacion numerica de datos experimentales
// C++
//
// Jimmy Alexander Calderon Trujillo
// Sebastian Barahona
// ============================================================

int main()
{
    // --------------------------------------------------------
    // 1. LECTURA DE LOS DATOS EXPERIMENTALES
    // --------------------------------------------------------

    std::vector<double> x;
    std::vector<double> y;

    std::ifstream archivo("datos/datos_sensor.csv");

    if (!archivo.is_open())
    {
        std::cerr << "Error: no se pudo abrir datos_sensor.csv\n";
        return 1;
    }

    std::string linea;

    // Ignorar encabezado x,y
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

    if (N < 3)
    {
        std::cerr << "Error: se requieren al menos 3 mediciones.\n";
        return 1;
    }

    // --------------------------------------------------------
    // 2. VERIFICACION DEL ESPACIAMIENTO
    // --------------------------------------------------------

    const double h = x[1] - x[0];

    bool uniforme = true;

    for (std::size_t i = 1; i < N - 1; ++i)
    {
        const double hi = x[i + 1] - x[i];

        if (std::abs(hi - h) > 1e-12)
        {
            uniforme = false;
            break;
        }
    }

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "DIFERENCIACION NUMERICA - C++\n";
    std::cout << "---------------------------------------------\n";
    std::cout << "Numero de mediciones : " << N << "\n";
    std::cout << "Espaciado h          : " << h << "\n";
    std::cout << "Espaciado uniforme   : "
              << (uniforme ? "SI" : "NO") << "\n";

    // --------------------------------------------------------
    // 3. DIFERENCIA HACIA ADELANTE
    // --------------------------------------------------------
    //
    // f'(xi) ~= [f(xi+1) - f(xi)] / h
    //
    // No puede calcularse en el ultimo punto porque no existe
    // una medicion posterior.
    // --------------------------------------------------------

    const double NaN =
        std::numeric_limits<double>::quiet_NaN();

    std::vector<double> derivada_adelante(N, NaN);

    for (std::size_t i = 0; i < N - 1; ++i)
    {
        derivada_adelante[i] =
            (y[i + 1] - y[i]) / h;
    }

    // --------------------------------------------------------
    // 4. DIFERENCIA CENTRAL
    // --------------------------------------------------------
    //
    // f'(xi) ~= [f(xi+1) - f(xi-1)] / (2h)
    //
    // Solo se calcula en los puntos interiores.
    // --------------------------------------------------------

    std::vector<double> derivada_central(N, NaN);

    for (std::size_t i = 1; i < N - 1; ++i)
    {
        derivada_central[i] =
            (y[i + 1] - y[i - 1]) / (2.0 * h);
    }

    // --------------------------------------------------------
    // 5. MOSTRAR LOS PRIMEROS RESULTADOS
    // --------------------------------------------------------

    std::cout << "\nPRIMEROS RESULTADOS\n";
    std::cout
        << "-------------------------------------------------------------\n";
    std::cout
        << "   x          y        Adelante       Central\n";
    std::cout
        << "-------------------------------------------------------------\n";

    for (std::size_t i = 0; i < 10; ++i)
    {
        std::cout
            << std::setw(5) << x[i]
            << "   "
            << std::setw(10) << y[i]
            << "   ";

        if (std::isnan(derivada_adelante[i]))
            std::cout << std::setw(10) << "NaN";
        else
            std::cout
                << std::setw(10)
                << derivada_adelante[i];

        std::cout << "   ";

        if (std::isnan(derivada_central[i]))
            std::cout << std::setw(10) << "NaN";
        else
            std::cout
                << std::setw(10)
                << derivada_central[i];

        std::cout << "\n";
    }

    // --------------------------------------------------------
    // 6. TRATAMIENTO DE LOS EXTREMOS
    // --------------------------------------------------------

    std::cout << "\nTRATAMIENTO DE LOS EXTREMOS\n";
    std::cout << "---------------------------------------------\n";

    std::cout
        << "Diferencia hacia adelante: no se calcula en x = "
        << x.back() << "\n";

    std::cout
        << "Diferencia central: no se calcula en x = "
        << x.front()
        << " ni x = "
        << x.back()
        << "\n";

    return 0;
}

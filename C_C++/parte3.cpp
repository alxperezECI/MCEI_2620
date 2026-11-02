#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <chrono>

// Función para corregir saltos de 2*pi en la orientación (unwrap manual)
void unwrap(std::vector<double> &theta)
{
    size_t n = theta.size();
    for (size_t i = 1; i < n; ++i)
    {
        double diff = theta[i] - theta[i - 1];
        while (diff > M_PI)
        {
            theta[i] -= 2.0 * M_PI;
            diff = theta[i] - theta[i - 1];
        }
        while (diff < -M_PI)
        {
            theta[i] += 2.0 * M_PI;
            diff = theta[i] - theta[i - 1];
        }
    }
}

int main()
{
    // 1. Leer el archivo CSV
    std::string ruta_csv = "/home/paula/MCEI_2620/Python/trayectoria_robot.csv";
    std::ifstream file(ruta_csv);

    if (!file.is_open())
    {
        std::cerr << "Error: No se pudo abrir el archivo CSV." << std::endl;
        return 1;
    }

    std::vector<double> t, x, y;
    std::string line;

    // Saltar la primera línea (encabezado t,x,y)
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string val_t, val_x, val_y;

        if (std::getline(ss, val_t, ',') &&
            std::getline(ss, val_x, ',') &&
            std::getline(ss, val_y, ','))
        {
            t.push_back(std::stod(val_t));
            x.push_back(std::stod(val_x));
            y.push_back(std::stod(val_y));
        }
    }
    file.close();

    size_t N = t.size();
    double h = t[1] - t[0]; // Paso de tiempo h = 0.2s

    std::vector<double> vx(N), vy(N), v(N), theta(N), omega(N);

    // =========================================================================
    // INICIO MEDICIÓN DE TIEMPO DE CÁLCULO NUMÉRICO
    // =========================================================================
    auto t_inicio = std::chrono::high_resolution_clock::now();

    // 2. Diferencias finitas centrales para los puntos internos
    for (size_t i = 1; i < N - 1; ++i)
    {
        vx[i] = (x[i + 1] - x[i - 1]) / (2.0 * h);
        vy[i] = (y[i + 1] - y[i - 1]) / (2.0 * h);
    }

    // Extremos (Diferencias unilaterales)
    vx[0] = (x[1] - x[0]) / h;
    vy[0] = (y[1] - y[0]) / h;
    vx[N - 1] = (x[N - 1] - x[N - 2]) / h;
    vy[N - 1] = (y[N - 1] - y[N - 2]) / h;

    // 3. Velocidad lineal y orientación
    for (size_t i = 0; i < N; ++i)
    {
        v[i] = std::sqrt(vx[i] * vx[i] + vy[i] * vy[i]);
        theta[i] = std::atan2(vy[i], vx[i]);
    }

    // 4. Desenvolvimiento angular
    unwrap(theta);

    // 5. Velocidad angular
    for (size_t i = 1; i < N - 1; ++i)
    {
        omega[i] = (theta[i + 1] - theta[i - 1]) / (2.0 * h);
    }
    omega[0] = (theta[1] - theta[0]) / h;
    omega[N - 1] = (theta[N - 1] - theta[N - 2]) / h;

    auto t_fin = std::chrono::high_resolution_clock::now();
    // =========================================================================
    // FIN MEDICIÓN DE TIEMPO
    // =========================================================================

    std::chrono::duration<double, std::milli> tiempo_cpp_ms = t_fin - t_inicio;

    // 6. Guardar los resultados en resultados_cpp.csv
    std::ofstream out_file("resultados_cpp.csv");
    out_file << "t,x,y,v,theta,omega\n";
    for (size_t i = 0; i < N; ++i)
    {
        out_file << t[i] << "," << x[i] << "," << y[i] << ","
                 << v[i] << "," << theta[i] << "," << omega[i] << "\n";
    }
    out_file.close();

    // Mostrar primeras muestras
    std::cout << "--- PRIMERAS 5 MUESTRAS CALCULADAS EN C++ ---\n";
    std::cout << std::setw(6) << "t[s]" << " | "
              << std::setw(8) << "v[m/s]" << " | "
              << std::setw(10) << "theta[rad]" << " | "
              << std::setw(12) << "omega[rad/s]" << "\n";
    std::cout << "--------------------------------------------\n";
    for (size_t i = 0; i < 5; ++i)
    {
        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(6) << t[i] << " | "
                  << std::setw(8) << v[i] << " | "
                  << std::setw(10) << theta[i] << " | "
                  << std::setw(12) << omega[i] << "\n";
    }

    std::cout << "\n--------------------------------------------\n";
    std::cout << "Tiempo de ejecución de cálculo numérico en C++: "
              << std::fixed << std::setprecision(6) << tiempo_cpp_ms.count() << " ms\n";
    std::cout << "--------------------------------------------\n";

    std::cout << "\n¡Cálculo en C++ finalizado! Resultados guardados en 'resultados_cpp.csv'.\n";

    return 0;
}
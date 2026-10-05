#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>

int main() {
    // 1. Abrir y leer el archivo CSV generado previamente
    std::ifstream file("trayectoria_robot.csv");
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo trayectoria_robot.csv" << std::endl;
        return 1;
    }

    std::string line;
    std::getline(file, line); // Omitir el encabezado ("t,x,y")

    std::vector<double> t, x, y;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string st, sx, sy;
        if (std::getline(ss, st, ',') && std::getline(ss, sx, ',') && std::getline(ss, sy, ',')) {
            t.push_back(std::stod(st));
            x.push_back(std::stod(sx));
            y.push_back(std::stod(sy));
        }
    }
    file.close();

    size_t N = t.size();
    if (N < 3) {
        std::cerr << "Error: Datos insuficientes en el archivo." << std::endl;
        return 1;
    }

    double h = t[1] - t[0]; // Paso temporal h = 0.2 s

    std::vector<double> vx(N), vy(N), v(N), theta(N), omega(N);

    // 2. Cálculo de vx y vy mediante diferencias centrales
    for (size_t i = 1; i < N - 1; ++i) {
        vx[i] = (x[i + 1] - x[i - 1]) / (2.0 * h);
        vy[i] = (y[i + 1] - y[i - 1]) / (2.0 * h);
    }
    // Extremos (diferencias unilaterales adelante/atrás)
    vx[0] = (x[1] - x[0]) / h;
    vy[0] = (y[1] - y[0]) / h;
    vx[N - 1] = (x[N - 1] - x[N - 2]) / h;
    vy[N - 1] = (y[N - 1] - y[N - 2]) / h;

    // 3. Velocidad lineal v e inclinación theta
    for (size_t i = 0; i < N; ++i) {
        v[i] = std::sqrt(vx[i] * vx[i] + vy[i] * vy[i]);
        theta[i] = std::atan2(vy[i], vx[i]);
    }

    // 4. Desenvolvimiento angular manual (unwrap)
    for (size_t i = 1; i < N; ++i) {
        double diff = theta[i] - theta[i - 1];
        while (diff > M_PI) { theta[i] -= 2.0 * M_PI; diff -= 2.0 * M_PI; }
        while (diff < -M_PI) { theta[i] += 2.0 * M_PI; diff += 2.0 * M_PI; }
    }

    // 5. Velocidad angular omega mediante diferencias centrales
    for (size_t i = 1; i < N - 1; ++i) {
        omega[i] = (theta[i + 1] - theta[i - 1]) / (2.0 * h);
    }
    omega[0] = (theta[1] - theta[0]) / h;
    omega[N - 1] = (theta[N - 1] - theta[N - 2]) / h;

    // 6. Imprimir los primeros resultados en consola para verificación
    std::cout << "=== RESULTADOS C++ (Primeras 5 muestras) ===" << std::endl;
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "t[s]\tv[m/s]\ttheta[rad]\tomega[rad/s]" << std::endl;
    for (size_t i = 0; i < 5; ++i) {
        std::cout << t[i] << "\t" << v[i] << "\t" << theta[i] << "\t\t" << omega[i] << std::endl;
    }

    std::cout << "\nProceso finalizado correctamente en C++ para " << N << " muestras." << std::endl;
    return 0;
}
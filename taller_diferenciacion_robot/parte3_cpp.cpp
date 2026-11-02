#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    std::ifstream fin("trayectoria_robot.csv");
    if (!fin.is_open()) {
        std::cerr << "No se pudo abrir trayectoria_robot.csv\n";
        return 1;
    }

    std::string line;
    std::getline(fin, line); 

    std::vector<double> t, x, y;
    while (std::getline(fin, line)) {
        std::stringstream ss(line);
        std::string campo;
        double ti, xi, yi;

        std::getline(ss, campo, ','); ti = std::stod(campo);
        std::getline(ss, campo, ','); xi = std::stod(campo);
        std::getline(ss, campo, ','); yi = std::stod(campo);

        t.push_back(ti);
        x.push_back(xi);
        y.push_back(yi);
    }
    fin.close();

    int N = t.size();
    std::cout << "Muestras leidas: " << N << "\n";

    std::vector<double> vx(N), vy(N), v(N), theta(N), theta_u(N), omega(N);

    for (int i = 1; i < N - 1; ++i) {
        vx[i] = (x[i+1] - x[i-1]) / (t[i+1] - t[i-1]);
        vy[i] = (y[i+1] - y[i-1]) / (t[i+1] - t[i-1]);
    }
    vx[0]   = (x[1] - x[0]) / (t[1] - t[0]);
    vx[N-1] = (x[N-1] - x[N-2]) / (t[N-1] - t[N-2]);
    vy[0]   = (y[1] - y[0]) / (t[1] - t[0]);
    vy[N-1] = (y[N-1] - y[N-2]) / (t[N-1] - t[N-2]);

    for (int i = 0; i < N; ++i) {
        v[i]     = std::sqrt(vx[i]*vx[i] + vy[i]*vy[i]);
        theta[i] = std::atan2(vy[i], vx[i]);
    }

    theta_u[0] = theta[0];
    for (int i = 1; i < N; ++i) {
        double d = theta[i] - theta_u[i-1];
        while (d >  M_PI) { theta[i] -= 2*M_PI; d = theta[i] - theta_u[i-1]; }
        while (d < -M_PI) { theta[i] += 2*M_PI; d = theta[i] - theta_u[i-1]; }
        theta_u[i] = theta[i];
    }

    for (int i = 1; i < N - 1; ++i) {
        omega[i] = (theta_u[i+1] - theta_u[i-1]) / (t[i+1] - t[i-1]);
    }
    omega[0]   = (theta_u[1] - theta_u[0]) / (t[1] - t[0]);
    omega[N-1] = (theta_u[N-1] - theta_u[N-2]) / (t[N-1] - t[N-2]);

    std::ofstream fout("resultados/parte3_cpp_resultados.csv");
    fout << "t,x,y,vx,vy,v,theta,theta_u,omega\n";
    for (int i = 0; i < N; ++i) {
        fout << t[i]     << ","
             << x[i]     << ","
             << y[i]     << ","
             << vx[i]    << ","
             << vy[i]    << ","
             << v[i]     << ","
             << theta[i] << ","
             << theta_u[i] << ","
             << omega[i] << "\n";
    }
    fout.close();

    double vmin = *std::min_element(v.begin(), v.end());
    double vmax = *std::max_element(v.begin(), v.end());
    double wmin = *std::min_element(omega.begin(), omega.end());
    double wmax = *std::max_element(omega.begin(), omega.end());

    std::cout << "Archivo resultados/parte3_cpp_resultados.csv generado.\n";
    std::cout << "v:     min = " << vmin << ", max = " << vmax << "\n";
    std::cout << "omega: min = " << wmin << ", max = " << wmax << "\n";

    return 0;
}
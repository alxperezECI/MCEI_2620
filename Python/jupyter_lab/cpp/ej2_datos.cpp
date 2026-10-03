#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>

using namespace std;

int main() {
    string path = "/home/lunam/MCEI_2620/Python/jupyter_lab/datos/datos_sensor.csv";
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "No se pudo abrir: " << path << endl;
        return 1;
    }

    vector<double> x, y;
    string line;
    getline(file, line);  // saltar encabezado
    while (getline(file, line)) {
        stringstream ss(line);
        string vx, vy;
        getline(ss, vx, ',');
        getline(ss, vy, ',');
        x.push_back(stod(vx));
        y.push_back(stod(vy));
    }
    file.close();

    int n = x.size();
    double h = x[1] - x[0];

    cout << "Numero de datos:  " << n << "\n";
    cout << "x va de:          " << x.front() << " a " << x.back() << "\n";
    cout << fixed << setprecision(6);
    cout << "Paso h:           " << h << "\n\n";

    auto t0 = chrono::high_resolution_clock::now();
    double I_trap = h * (y.front() / 2.0 + y.back() / 2.0);
    for (int i = 1; i < n - 1; ++i) I_trap += h * y[i];
    auto t1 = chrono::high_resolution_clock::now();

    double t_trap = chrono::duration<double>(t1 - t0).count();

    auto t2 = chrono::high_resolution_clock::now();

    gsl_interp_accel *acc = gsl_interp_accel_alloc();
    gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, n);

    gsl_spline_init(spline, x.data(), y.data(), n);

    double I_interp = gsl_spline_eval_integ(spline, x.front(), x.back(), acc);

    gsl_spline_free(spline);
    gsl_interp_accel_free(acc);

    auto t3 = chrono::high_resolution_clock::now();
    double t_interp = chrono::duration<double>(t3 - t2).count();

    cout << fixed << setprecision(10);
    cout << "Trapecio explicito:  I = " << I_trap   << "   (t = " << t_trap   << " s)\n";
    cout << "GSL interpolacion:   I = " << I_interp << "   (t = " << t_interp << " s)\n\n";

    cout << scientific << setprecision(3);
    cout << "|trapecio - interp| = " << fabs(I_trap - I_interp) << "\n";

    return 0;
}
#include <iostream>

#include <fstream>

#include <sstream>

#include <vector>

#include <cmath>

#include <chrono>

#include <iomanip>
 
using namespace std;
 
int main() {
 
    // Taller de diferenciacion numerica

    // Cinematica diferencial de un robot

    // Parte 3 - C/C++
 
    auto inicio = chrono::high_resolution_clock::now();
 
    // Abrir archivo CSV

    ifstream archivo("../trayectoria_robot.csv");
 
    if (!archivo.is_open()) {

        cerr << "Error: no se pudo abrir trayectoria_robot.csv" << endl;

        return 1;

    }
 
    vector<double> t, x, y;
 
    string linea;
 
    // Ignorar encabezado

    getline(archivo, linea);
 
    // Leer datos

    while (getline(archivo, linea)) {
 
        stringstream ss(linea);

        string valor;
 
        getline(ss, valor, ',');

        t.push_back(stod(valor));
 
        getline(ss, valor, ',');

        x.push_back(stod(valor));
 
        getline(ss, valor, ',');

        y.push_back(stod(valor));

    }
 
    archivo.close();
 
    int N = t.size();
 
    vector<double> vx(N, 0.0);

    vector<double> vy(N, 0.0);

    vector<double> v(N, 0.0);

    vector<double> theta(N, 0.0);

    vector<double> omega(N, 0.0);
 
    // -------------------------------------

    // Derivadas de posicion

    // -------------------------------------
 
    // Diferencia hacia adelante en el primer punto

    vx[0] = (x[1] - x[0]) / (t[1] - t[0]);

    vy[0] = (y[1] - y[0]) / (t[1] - t[0]);
 
    // Diferencias centrales

    for (int i = 1; i < N - 1; i++) {
 
        vx[i] = (x[i+1] - x[i-1]) /

                (t[i+1] - t[i-1]);
 
        vy[i] = (y[i+1] - y[i-1]) /

                (t[i+1] - t[i-1]);

    }
 
    // Diferencia hacia atras en el ultimo punto

    vx[N-1] = (x[N-1] - x[N-2]) /

              (t[N-1] - t[N-2]);
 
    vy[N-1] = (y[N-1] - y[N-2]) /

              (t[N-1] - t[N-2]);
 
    // -------------------------------------

    // Velocidad lineal y orientacion

    // -------------------------------------
 
    for (int i = 0; i < N; i++) {
 
        v[i] = sqrt(vx[i]*vx[i] + vy[i]*vy[i]);
 
        theta[i] = atan2(vy[i], vx[i]);

    }
 
    // -------------------------------------

    // Desenvolvimiento angular (unwrap)

    // -------------------------------------
 
    const double PI = acos(-1.0);
 
    for (int i = 1; i < N; i++) {
 
        double diferencia = theta[i] - theta[i-1];
 
        if (diferencia > PI) {
 
            for (int j = i; j < N; j++)

                theta[j] -= 2.0 * PI;
 
        } else if (diferencia < -PI) {
 
            for (int j = i; j < N; j++)

                theta[j] += 2.0 * PI;

        }

    }
 
    // -------------------------------------

    // Velocidad angular

    // -------------------------------------
 
    // Diferencia hacia adelante

    omega[0] = (theta[1] - theta[0]) /

               (t[1] - t[0]);
 
    // Diferencias centrales

    for (int i = 1; i < N - 1; i++) {
 
        omega[i] = (theta[i+1] - theta[i-1]) /

                   (t[i+1] - t[i-1]);

    }
 
    // Diferencia hacia atras

    omega[N-1] = (theta[N-1] - theta[N-2]) /

                 (t[N-1] - t[N-2]);
 
    auto fin = chrono::high_resolution_clock::now();
 
    chrono::duration<double> tiempo = fin - inicio;
 
    // -------------------------------------

    // Resultados

    // -------------------------------------
 
    cout << fixed << setprecision(6);
 
    cout << "Numero de muestras: " << N << endl;

    cout << "Paso temporal h: "
<< t[1] - t[0] << " s" << endl;
 
    cout << "Velocidad inicial: "
<< v[0] << " m/s" << endl;
 
    cout << "Velocidad final: "
<< v[N-1] << " m/s" << endl;
 
    cout << "Omega inicial: "
<< omega[0] << " rad/s" << endl;
 
    cout << "Omega final: "
<< omega[N-1] << " rad/s" << endl;
 
    cout << scientific;

    cout << "Tiempo de ejecucion: "
<< tiempo.count() << " s" << endl;
 
    // -------------------------------------

    // Guardar resultados

    // -------------------------------------
 
    ofstream salida("resultados_robot_cpp.csv");
 
    salida << "t,x,y,vx,vy,v,theta,omega\n";
 
    salida << fixed << setprecision(10);
 
    for (int i = 0; i < N; i++) {
 
        salida
<< t[i] << ","
<< x[i] << ","
<< y[i] << ","
<< vx[i] << ","
<< vy[i] << ","
<< v[i] << ","
<< theta[i] << ","
<< omega[i] << "\n";

    }
 
    salida.close();
 
    cout << "Resultados guardados en resultados_robot_cpp.csv"
<< endl;
 
    return 0;

}
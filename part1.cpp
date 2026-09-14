#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

int main() {
    ifstream archivo("datos_convertidor_realista.csv");
    if (!archivo) {
        cout << "No se pudo abrir el archivo CSV" << endl;
        return 1;
    }

    string linea;
    getline(archivo, linea);

    double V[200], I[200], T[200], P[200];
    int n = 0;

    while (getline(archivo, linea)) {
        // Reemplazar comas por espacios
        for (int k = 0; k < linea.size(); k++) {
            if (linea[k] == ',') linea[k] = ' ';
        }
        stringstream ss(linea);
        ss >> V[n] >> I[n] >> T[n] >> P[n];
        n++;
    }
    archivo.close();

    cout << "Numero de datos: " << n << endl;

    MatrixXd X(n, 4);
    VectorXd y(n);

    for (int i = 0; i < n; i++) {
        X(i, 0) = 1.0;
        X(i, 1) = V[i];
        X(i, 2) = I[i];
        X(i, 3) = T[i];
        y(i) = P[i];
    }

    VectorXd beta = (X.transpose() * X).inverse() * X.transpose() * y;

    cout << "\n=== Coeficientes del modelo ===" << endl;
    cout << "beta0 = " << beta(0) << endl;
    cout << "beta1 (V) = " << beta(1) << endl;
    cout << "beta2 (I) = " << beta(2) << endl;
    cout << "beta3 (T) = " << beta(3) << endl;

    VectorXd yhat = X * beta;
    VectorXd r = y - yhat;

    double SSE = 0.0;
    for (int i = 0; i < n; i++) {
        SSE = SSE + r(i) * r(i);
    }

    double MSE = SSE / n;
    double RMSE = sqrt(MSE);

    cout << "\n=== Residuales y error cuadratico medio ===" << endl;
    cout << "SSE = " << SSE << endl;
    cout << "MSE = " << MSE << endl;
    cout << "RMSE = " << RMSE << endl;

    cout << "\nPrimeros 5 residuales:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "  y = " << y(i) << "  yhat = " << yhat(i)
             << "  r = " << r(i) << endl;
    }

    double sumaV = 0, sumaI = 0, sumaT = 0, sumaP = 0;
    for (int i = 0; i < n; i++) {
        sumaV = sumaV + V[i];
        sumaI = sumaI + I[i];
        sumaT = sumaT + T[i];
        sumaP = sumaP + P[i];
    }
    double mV = sumaV / n;
    double mI = sumaI / n;
    double mT = sumaT / n;
    double mP = sumaP / n;

    double varV = 0, varI = 0, varT = 0, varP = 0;
    for (int i = 0; i < n; i++) {
        varV = varV + (V[i] - mV) * (V[i] - mV);
        varI = varI + (I[i] - mI) * (I[i] - mI);
        varT = varT + (T[i] - mT) * (T[i] - mT);
        varP = varP + (P[i] - mP) * (P[i] - mP);
    }
    double sV = sqrt(varV / (n - 1));
    double sI = sqrt(varI / (n - 1));
    double sT = sqrt(varT / (n - 1));
    double sP = sqrt(varP / (n - 1));

    double b1_std = beta(1) * sV / sP;
    double b2_std = beta(2) * sI / sP;
    double b3_std = beta(3) * sT / sP;

    cout << "\n=== Influencia relativa (coeficientes estandarizados) ===" << endl;
    cout << "V: " << b1_std << endl;
    cout << "I: " << b2_std << endl;
    cout << "T: " << b3_std << endl;

    JacobiSVD<MatrixXd> svd(X);
    VectorXd s = svd.singularValues();
    double condX = s(0) / s(3);

    cout << "\n=== Numero de condicion ===" << endl;
    cout << "cond(X) = " << condX << endl;
    cout << "cond(X'X) = " << condX * condX << endl;

    return 0;
}
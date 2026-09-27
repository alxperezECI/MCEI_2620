#include <iostream>

#include <fstream>

#include <sstream>

#include <vector>

#include <string>

#include <limits>

#include <iomanip>
 
int main()

{

    std::vector<double> x;

    std::vector<double> y;
 
    // Leer datos experimentales

    std::ifstream archivo("../Python/datos_sensor.csv");
 
    if (!archivo.is_open())

    {

        std::cerr << "Error: no se pudo abrir datos_sensor.csv\n";

        return 1;

    }
 
    std::string linea;
 
    // Ignorar encabezado

    std::getline(archivo, linea);
 
    while (std::getline(archivo, linea))

    {

        std::stringstream ss(linea);

        std::string sx, sy;
 
        std::getline(ss, sx, ',');

        std::getline(ss, sy, ',');
 
        x.push_back(std::stod(sx));

        y.push_back(std::stod(sy));

    }
 
    archivo.close();
 
    int N = x.size();

    double h = x[1] - x[0];
 
    // Crear vectores para las derivadas

    double NaN = std::numeric_limits<double>::quiet_NaN();
 
    std::vector<double> dy_adelante(N, NaN);

    std::vector<double> dy_central(N, NaN);
 
    // Diferencia hacia adelante

    for (int i = 0; i < N - 1; i++)

    {

        dy_adelante[i] =

            (y[i + 1] - y[i]) / h;

    }
 
    // Diferencia central

    for (int i = 1; i < N - 1; i++)

    {

        dy_central[i] =

            (y[i + 1] - y[i - 1]) / (2.0 * h);

    }
 
    // Mostrar resultados

    std::cout << "Numero de datos = " << N << "\n";

    std::cout << "Paso h          = "
<< std::fixed << std::setprecision(4)
<< h << "\n\n";
 
    std::cout << "x\tAdelante\tCentral\n";

    std::cout << "---------------------------------\n";
 
    for (int i = 0; i < N; i++)

    {

        std::cout
<< std::fixed << std::setprecision(2)
<< x[i] << "\t"
<< std::setprecision(6)
<< dy_adelante[i] << "\t"
<< dy_central[i] << "\n";

    }
 
    return 0;

}
 
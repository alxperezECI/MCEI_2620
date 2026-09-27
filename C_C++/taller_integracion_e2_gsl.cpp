#include <iostream>

#include <fstream>

#include <sstream>

#include <vector>

#include <string>

#include <cmath>

#include <iomanip>
 
#include <gsl/gsl_spline.h>

#include <gsl/gsl_errno.h>
 
int main()

{

    std::vector<double> x;

    std::vector<double> y;
 
    // El CSV se encuentra en la carpeta Python

    std::ifstream archivo("../Python/datos_sensor.csv");
 
    if (!archivo.is_open())

    {

        std::cerr << "Error: no se pudo abrir datos_sensor.csv\n";

        return 1;

    }
 
    std::string linea;
 
    // Ignorar encabezado x,y

    std::getline(archivo, linea);
 
    // Leer datos

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
 
    const int N = x.size();
 
    std::cout << "Numero de datos = " << N << "\n";
 
    // -------------------------------------------------

    // Verificacion del espaciamiento

    // -------------------------------------------------
 
    double h = x[1] - x[0];

    bool equiespaciados = true;
 
    for (int i = 1; i < N - 1; i++)

    {

        double hi = x[i + 1] - x[i];
 
        if (std::abs(hi - h) > 1e-12)

        {

            equiespaciados = false;

        }

    }
 
    std::cout << std::fixed << std::setprecision(4);

    std::cout << "Paso h          = " << h << "\n";

    std::cout << "Equiespaciados  = "
<< (equiespaciados ? "Si" : "No") << "\n\n";
 
    // -------------------------------------------------

    // 1. Trapecio compuesto explicito

    // -------------------------------------------------
 
    double suma =

        0.5 * y.front()

        + 0.5 * y.back();
 
    for (int i = 1; i < N - 1; i++)

    {

        suma += y[i];

    }
 
    double I_trap = h * suma;
 
    std::cout << std::setprecision(12);

    std::cout << "Integral por trapecio = "
<< I_trap << "\n";
 
    // -------------------------------------------------

    // 2. Interpolacion mediante GSL

    // -------------------------------------------------
 
    gsl_interp_accel *acc =

        gsl_interp_accel_alloc();
 
    gsl_spline *spline =

        gsl_spline_alloc(gsl_interp_cspline, N);
 
    gsl_spline_init(

        spline,

        x.data(),

        y.data(),

        N

    );
 
    // Integrar el spline interpolado

    double I_spline =

        gsl_spline_eval_integ(

            spline,

            x.front(),

            x.back(),

            acc

        );
 
    std::cout << "Integral spline GSL   = "
<< I_spline << "\n";
 
    std::cout << "Diferencia            = "
<< std::abs(I_spline - I_trap)
<< "\n";
 
    // Liberar memoria

    gsl_spline_free(spline);

    gsl_interp_accel_free(acc);
 
    return 0;

}
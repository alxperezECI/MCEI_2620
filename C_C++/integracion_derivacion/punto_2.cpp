#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <gsl/gsl_spline.h>

int main() {
    // Vectores donde se almacenarán los datos del archivo CSV
    std::vector<double> x, y;

    // Abrir el archivo que contiene los datos
    std::ifstream archivo("datos_sensor.csv");
    std::string linea;

    // Comprobar si el archivo se abrió correctamente
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo datos_sensor.csv" << std::endl;
        return 1;
    }

    // El archivo comienza con una línea de encabezados ("x,y")
    // La primera línea tiene encabezado, por lo que se lee y se descarta 
    std::getline(archivo, linea);

    // Leer el archivo hasta llegar al final
    while (std::getline(archivo, linea)) {

        std::stringstream ss(linea);

        std::string valor_x, valor_y;

        
        if (std::getline(ss, valor_x, ',') && std::getline(ss, valor_y, ',')) {

            x.push_back(std::stod(valor_x));
            y.push_back(std::stod(valor_y));
        }
    }

    // Cerrar el archivo después de terminar la lectura
    archivo.close();
    
    // Obtener la cantidad de datos que fueron cargados
    int n = x.size();

    // Si no se encontró ningún dato, terminar el programa
    if (n == 0) return 1;

    // Regla del trapecio:

    // Variable donde se acumulará la aproximación de la integral
    double I_trap = 0.0;

    // Recorrer todos los pares de puntos consecutivos
    // Cada par de puntos forma un trapecio cuya área se suma al resultado total
    for (int i = 0; i < n - 1; ++i) {

        double h = x[i+1] - x[i];

        I_trap += (h / 2.0) * (y[i] + y[i+1]);
    }


    // Integración interpolada (spline cúbico) con GSL:

    // El spline cúbico analiza el punto anterior y el punto siguiente para calcular con qué pendiente venía la curva
    // Después, une los puntos con una curva suave
    //Esto permite que la transición en cada punto sea suave y continua

    // Reservar memoria para el acelerador de interpolación
    // Este objeto ayuda a GSL a realizar las búsquedas necesarias dentro de los datos
    gsl_interp_accel *acc = gsl_interp_accel_alloc();

    // Reservar memoria para el spline cúbico
    
    // gsl_interp_cspline indica que se utilizará una interpolación con splines cúbicos. En vez de aproximar
    // el área con un trapecio, GSL construye una curva suave que pasa por los datos

    gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, n);

    // Inicializar el spline utilizando todos los datos cargados
    // GSL utiliza los puntos x,y para construir la curva interpolada
    gsl_spline_init(spline, x.data(), y.data(), n);

    // Calcular la integral de la curva interpolada
    // entre el primer y el último valor de x

    // A diferencia del método del trapecio, aquí no se suman
    // las áreas de cada intervalo. GSL utiliza el spline que construyó y calcula su integral
    double I_gsl = gsl_spline_eval_integ(spline, x.front(), x.back(), acc);


    
    std::cout << std::setprecision(8);

    // Mostrar la cantidad de datos que fueron procesados
    std::cout << "Cantidad de datos procesados: " << n << std::endl;

    // Mostrar la integral con la regla del trapecio
    std::cout << "Integral (Trapecio directo) : " << I_trap << std::endl;

    // Mostrar la integral con el spline cúbico por GSL
    std::cout << "Integral (Spline GSL)       : " << I_gsl << std::endl;

    // Liberar la memoria utilizada por el spline
    gsl_spline_free(spline);

    // Liberar la memoria utilizada por el acelerador de interpolación
    gsl_interp_accel_free(acc);

    return 0;
}

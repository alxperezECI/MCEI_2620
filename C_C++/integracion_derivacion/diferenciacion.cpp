#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

int main() {
    // Vectores donde se almacenarán los datos del archivo CSV
    std::vector<double> x, y;

    // Abrir el archivo que contiene los datos
    std::ifstream archivo("datos_sensor.csv");
    std::string linea;


    // Comprobar si el archivo se abrió correctamente
    if (archivo.is_open()) {

        // El archivo comienza con una línea de encabezados ("x,y")
        // La primera línea tiene encabezado, por lo que se lee y se descarta 
        std::getline(archivo, linea);

        // Leer el archivo hasta llegar al final
        while (std::getline(archivo, linea)) {

            std::stringstream ss(linea);

            std::string val_x, val_y;

        
            if (std::getline(ss, val_x, ',') && std::getline(ss, val_y, ',')) {

                x.push_back(std::stod(val_x));
                y.push_back(std::stod(val_y));
            }
        }

        // Cerrar el archivo después de terminar la lectura
        archivo.close();

    } else {

        // Si el archivo no pudo abrirse, mostrar un mensaje de error
        std::cerr << "Error al abrir el archivo." << std::endl;
        return 1;
    }

    // Obtener la cantidad de datos que fueron cargados
    int n = x.size();

    // Se necesitan al menos tres puntos para poder calcular las diferencias centrales
    if (n < 3) return 1;


    // El código supone que los datos están igualmente espaciados
    double h = x[1] - x[0];

    // Crear vectores para almacenar las aproximaciones de la derivada:
    
    // dy_adelante: derivada calculada con diferencias hacia adelante
    // dy_central:  derivada calculada con diferencias centrales
    std::vector<double> dy_adelante(n, 0.0);
    std::vector<double> dy_central(n, 0.0);

    // Diferencia hacia adelante:

    // La diferencia hacia adelante utiliza el punto actual y el siguiente para aproximar la derivada:
    
    // f'(x_i) ≈ [f(x_{i+1}) - f(x_i)] / h
    
    // Se puede aplicar desde el primer punto hasta el penúltimo, porque el último punto no tiene un punto posterior
    for (int i = 0; i < n - 1; ++i) {
        dy_adelante[i] = (y[i+1] - y[i]) / h;
    }

    // Para resolver el problema del último punto se utiliza una diferencia hacia atrás:
    
    // f'(x_{n-1}) ≈ [f(x_{n-1}) - f(x_{n-2})] / h
    dy_adelante[n-1] = (y[n-1] - y[n-2]) / h;

    // Diferencia central:

    // La diferencia central utiliza un punto a cada lado del punto donde se desea calcular la derivada:
    
    // f'(x_i) ≈ [f(x_{i+1}) - f(x_{i-1})] / (2h)
    
    // Solo puede utilizarse en los puntos interiores, porque son los únicos que tienen un punto anterior y uno posterior
    for (int i = 1; i < n - 1; ++i) {
        dy_central[i] = (y[i+1] - y[i-1]) / (2.0 * h);
    }

    // En el primer punto no existe un punto anterior, para ello se utiliza una diferencia hacia adelante
    dy_central[0] = (y[1] - y[0]) / h;

    // En el último punto no existe un punto posterior, para ello se utiliza una diferencia hacia atrás
    dy_central[n-1] = (y[n-1] - y[n-2]) / h;

    // Salida:

    // - std::fixed: utilizar notación decimal
    // - std::setprecision(6): mostrar seis cifras después del punto
    std::cout << std::fixed << std::setprecision(6);
    

    // std::setw(5) y std::setw(15) establecen el ancho de cada columna
    // std::left hace que el contenido se alinee hacia la izquierda
    // std::right hace que el contenido se alinee hacia la derecha
    
    // Esto permite que todos los resultados queden organizados
   
    std::cout << std::setw(5)  << std::left  << "i" 
              << std::setw(12) << std::right << "x"
              << std::setw(12) << std::right << "y"
              << std::setw(15) << std::right << "dy (Adelante)" 
              << std::setw(15) << std::right << "dy (Central)" << std::endl;


    std::cout << "-----------------------------------------------------------" << std::endl;
    
   
    // - i: índice del dato
    // - dy (Adelante): derivada calculada con diferencia hacia adelante
    // - dy (Central): derivada calculada con diferencia central
    
    // Se utilizan nuevamente anchos fijos también
    for (int i = 0; i < n; ++i) {
        std::cout << std::setw(5)  << std::left  << i 
                  << std::setw(12) << std::right << x[i]
                  << std::setw(12) << std::right << y[i]
                  << std::setw(15) << std::right << dy_adelante[i] 
                  << std::setw(15) << std::right << dy_central[i] << std::endl;
    }

    return 0;
}
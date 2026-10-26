#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>
#include <chrono> // Librería necesaria para medir el tiempo

// Función para realizar el desenvolvimiento angular (unwrap)
// La función evita los saltos de +/-2*pi que pueden aparecer cuando se calcula el ángulo con atan2()
void unwrap(std::vector<double>& theta) {
    // Se comienza desde el segundo elemento para comparar cada ángulo con el ángulo calculado anteriormente
    for (size_t i = 1; i < theta.size(); ++i) {

        // Diferencia entre el ángulo actual y el anterior
        double diff = theta[i] - theta[i-1];

        // Si la diferencia es mayor que pi, significa que probablemente ocurrió un salto de -pi a pi
        // Se resta 2*pi para mantener la continuidad del ángulo
        while (diff > M_PI) {
            theta[i] -= 2 * M_PI;
            diff = theta[i] - theta[i-1];
        }

        // Si la diferencia es menor que -pi, significa que probablemente ocurrió un salto de pi a -pi
        // Se suma 2*pi para mantener la continuidad del ángulo
        while (diff < -M_PI) {
            theta[i] += 2 * M_PI;
            diff = theta[i] - theta[i-1];
        }
    }
}

int main() {
    // Iniciar el cronómetro antes de la carga de datos
    auto start_time = std::chrono::high_resolution_clock::now();

    // Vectores donde se almacenarán los datos del archivo:
    // t: tiempo
    // x: posición en el eje X
    // y: posición en el eje Y
    std::vector<double> t, x, y;

    // Se abre el archivo CSV que contiene la trayectoria del robot
    std::ifstream file("trayectoria_robot.csv");

    // Variables para leer cada línea y dato del CSV
    std::string line, word;

    // Leer el archivo CSV
    if (file.is_open()) {

        // Se lee y descarta la primera línea (encabezado del archivo)
        std::getline(file, line);

        // Se recorren todas las líneas restantes del archivo
        while (std::getline(file, line)) {

            // Se utiliza un stringstream para separar los datos de cada fila utilizando la coma como delimitador
            std::stringstream ss(line);

            // Se lee el tiempo y se convierte de texto a double
            std::getline(ss, word, ',');
            t.push_back(std::stod(word));

            // Se lee la posición x y se convierte a double
            std::getline(ss, word, ',');
            x.push_back(std::stod(word));

            // Se lee la posición y y se convierte a double
            std::getline(ss, word, ',');
            y.push_back(std::stod(word));
        }

        // Se cierra el archivo después de terminar la lectura
        file.close();

    } else {
        // Si el archivo no pudo abrirse, se muestra un mensaje de error
        std::cerr << "Error al abrir el archivo 'trayectoria_robot.csv'."
                  << std::endl;

        return 1;
    }

    // Número total de datos de la trayectoria
    size_t N = t.size();

    // Se crean los vectores que almacenarán:
    // vx, vy: componentes de la velocidad
    // v: velocidad lineal
    // theta: orientación del robot
    // w: velocidad angular
    
    // Todos se inicializan en cero
    std::vector<double> vx(N, 0.0), vy(N, 0.0), v(N, 0.0),
                       theta(N, 0.0), w(N, 0.0);

    // Cálculo de vx y vy con diferencias centrales

    // Las diferencias centrales permiten aproximar la derivada de la posición respecto al tiempo:
    
    // vx = dx/dt
    // vy = dy/dt
    
    // Para un punto interior se utilizan los datos anterior y posterior al punto actual
    for (size_t i = 1; i < N - 1; ++i) {

        // Aproximación de la velocidad en X mediante diferencia central
        vx[i] = (x[i+1] - x[i-1]) / (t[i+1] - t[i-1]);

        // Aproximación de la velocidad en Y mediante diferencia central
        vy[i] = (y[i+1] - y[i-1]) / (t[i+1] - t[i-1]);
    }

    // Aproximación en los extremos
    
    // En el primer y último punto no es posible utilizar diferencias centrales porque no existen datos a ambos lados
    // Por eso se utiliza una diferencia hacia adelante en el primer punto y una diferencia hacia atrás en el último

    // Velocidad en X en el primer instante
    vx[0] = (x[1] - x[0]) / (t[1] - t[0]);

    // Velocidad en Y en el primer instante
    vy[0] = (y[1] - y[0]) / (t[1] - t[0]);

    // Velocidad en X en el último instante
    vx[N-1] = (x[N-1] - x[N-2]) / (t[N-1] - t[N-2]);

    // Velocidad en Y en el último instante
    vy[N-1] = (y[N-1] - y[N-2]) / (t[N-1] - t[N-2]);

    // Cálculo de la velocidad lineal (v) y orientación (theta)
    for (size_t i = 0; i < N; ++i) {

        // La velocidad lineal corresponde a la magnitud del vector velocidad formado por sus componentes vx y vy:
        
        // v = sqrt(vx^2 + vy^2)
        
        // El resultado representa la rapidez del robot en m/s
        v[i] = std::sqrt(vx[i]*vx[i] + vy[i]*vy[i]);

        // atan2 permite calcular el ángulo de orientación del vector velocidad con respecto al eje X
        
        // El resultado se obtiene en radianes y tiene en cuenta correctamente el cuadrante en el que se encuentra el vector
        theta[i] = std::atan2(vy[i], vx[i]);
    }


    // atan2() entrega normalmente valores entre -pi y pi
    // Cuando el robot continúa girando y el ángulo supera estos límites, puede aparecer un salto brusco, por ejemplo: 3.13 rad a -3.14 rad
    
    // El unwrap() corrige estos saltos para obtener una evolución continua del ángulo

    unwrap(theta);

    // La velocidad angular es la derivada de la orientación:
    
    // w = d(theta)/dt
    
    // Se utiliza una diferencia central para los puntos interiores de la trayectoria
    for (size_t i = 1; i < N - 1; ++i) {

        // Aproximación de la velocidad angular con diferencia central
        w[i] = (theta[i+1] - theta[i-1]) /
               (t[i+1] - t[i-1]);
    }

   
    // Al igual que con vx y vy, en los extremos no se puede utilizar una diferencia central, por lo que se utilizan diferencias
    // hacia adelante y hacia atrás

    // Velocidad angular en el primer instante
    w[0] = (theta[1] - theta[0]) / (t[1] - t[0]);

    // Velocidad angular en el último instante
    w[N-1] = (theta[N-1] - theta[N-2]) /
             (t[N-1] - t[N-2]);

    // Detener el cronómetro antes de imprimir los resultados y calcular la duración
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Tiempo de los calculos (incluyendo carga de datos): " << elapsed.count() << " segundos\n\n";

    // Imprimir los resultados en la consola
  
    std::cout << std::left
              << std::setw(10) << "t(s)"
              << std::setw(15) << "v(m/s)"
              << std::setw(15) << "theta(rad)"
              << std::setw(15) << "w(rad/s)" << "\n";

    std::cout << std::string(55, '-') << "\n";


    for (size_t i = 0; i < N; ++i) {

        std::cout << std::left

                  << std::setw(10)
                  << std::fixed
                  << std::setprecision(2)
                  << t[i]

                  << std::setw(15)
                  << std::fixed
                  << std::setprecision(4)
                  << v[i]

                  << std::setw(15)
                  << std::fixed
                  << std::setprecision(4)
                  << theta[i]

                  << std::setw(15)
                  << std::fixed
                  << std::setprecision(4)
                  << w[i]

                  << "\n";
    }

    return 0;
}
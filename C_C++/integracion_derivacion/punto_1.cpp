#include <iostream>
#include <iomanip>
#include <cmath>
#include <gsl/gsl_integration.h>

// Definición de la función que se desea integrar
double f(double x, void * params) {
    return exp(-0.4 * x) * (1.0 + 0.5 * sin(3.0 * x));
}

// Implementación manual de la regla del trapecio

// El intervalo [a,b] se divide en n subintervalos de igual tamaño
// La integral se aproxima sumando las áreas de los trapecios formados
double regla_trapecio(double a, double b, int n) {

    // Calcula el ancho de cada subintervalo
    double h = (b - a) / n;

    // Los extremos del intervalo tienen un peso de 1/2 en la suma
    double suma = (f(a, nullptr) + f(b, nullptr)) / 2.0;
    
    // Se recorren los puntos interiores del intervalo, estos valores tienen peso completo en la suma
    for (int i = 1; i < n; ++i) {
        suma += f(a + i * h, nullptr);
    }

    // Multiplicar la suma por h
    return suma * h;
}

int main() {

    // Límites inferior y superior del intervalo de integración
    double a = 0.0, b = 8.0;
    
    // Valor de referencia para el cálculo del error (valor analítico del primer punto)
    const double REFERENCIA = 2.559824508;
    

    // Cuadratura adaptativa con GSL
    
    // Se reserva memoria para que GSL pueda administrar las subdivisiones del intervalo durante la integración adaptativa
    // El número 1000 indica la cantidad máxima de subdivisiones disponibles para el proceso
    gsl_integration_workspace * workspace = gsl_integration_workspace_alloc(1000);

    
    // - resultado_gsl: aproximación numérica de la integral
    // - error_gsl: estimación del error realizada por GSL
    double resultado_gsl, error_gsl;
    
    // GSL utiliza gsl_function para recibir la función que se desea integrar
    gsl_function F;

    // Se indica que la función utilizada será f()
    F.function = &f;

    // La función no necesita parámetros adicionales, por lo que se utiliza nullptr
    F.params = nullptr;
    
    // Tolerancia absoluta permitida por el método
    // El algoritmo intenta que el error absoluto estimado sea menor que este valor
    double epsabs = 1e-6;

    // Tolerancia relativa permitida
    // Permite controlar el error en relación con el tamaño del valor calculado
    double epsrel = 1e-6;

    // Número máximo de subdivisiones que puede realizar GSL durante el proceso de integración
    int limit = 1000;

    // GSL utiliza una regla de integración numérica llamada Gauss-Kronrod

    // Esta regla aproxima el valor de la integral evaluando la función
    // en varios puntos del intervalo

    // Una característica importante de Gauss-Kronrod es que permite obtener
    // tanto una aproximación de la integral como una estimación del error

    // Esto resulta útil para que GSL pueda decidir si necesita aumentar
    // la precisión en alguna parte del intervalo

    // El parámetro "key" permite elegir el número de puntos utilizados
    // por la regla. El valor 6 es la regla de 61 puntos
    int key = 6;


    // GSL aplica la regla de Gauss-Kronrod sobre el intervalo y, utilizando la estimación del error, determina si es necesario
    // subdividir alguna parte del intervalo para obtener mayor precisión. Por éso, la cuadratura es "adaptativa": el método no utiliza
    // el mismo nivel de subdivisión en todo el intervalo

    // Los resultados se almacenan en:
    // - resultado_gsl: aproximación numérica de la integral
    // - error_gsl: estimación del error realizada por GSL
    gsl_integration_qag(&F, a, b, epsabs, epsrel, limit, key, workspace, &resultado_gsl, &error_gsl);
    
    // Se calcula el error comparando el resultado obtenido por GSL con el valor de referencia
    double error_exacto_gsl = std::abs(resultado_gsl - REFERENCIA);
    

    std::cout << "Resultados Cuadratura Adaptativa (GSL)" << std::endl;

    // Se utilizan 10 cifras significativas para mostrar los resultados
    std::cout << std::setprecision(10);

    // Aproximación de la integral obtenida por GSL
    std::cout << "Resultado                : " << resultado_gsl << std::endl;

    // Error estimado por el algoritmo de GSL
    std::cout << "Error estimado por GSL   : " << error_gsl << std::endl;

    // Diferencia absoluta entre el resultado de GSL y el valor de referencia
    std::cout << "Error exacto (vs Ref)    : " << error_exacto_gsl << std::endl;

    // Se muestran las tolerancias utilizadas durante la integración
    std::cout << "Tolerancia absoluta      : " << epsabs << std::endl;
    std::cout << "Tolerancia relativa      : " << epsrel << std::endl;
    
    // Se libera la memoria reservada para el espacio de trabajo utilizado por GSL
    gsl_integration_workspace_free(workspace);
    
    // Regla del trapecio:
    
    // Número de subintervalos utilizados para aproximar la integral
    // Un valor mayor de n genera más trapecios
    int n = 500;

    // Se calcula la integral utilizando la función de la regla del trapecio implementada manualmente
    double resultado_trap = regla_trapecio(a, b, n);
    
    // Se calcula el error absoluto comparando el resultado obtenido por el método del trapecio con el valor de referencia
    double error_exacto_trap = std::abs(resultado_trap - REFERENCIA);
    


    std::cout << "\nResultados de método del trapecio)" << std::endl;

    // Se muestra el resultado de la integral y el número de subintervalos utilizados
    std::cout << "Resultado (n=" << n << ")        : " << resultado_trap << std::endl;

    // Se muestra el error absoluto respecto al valor de referencia
    std::cout << "Error exacto (vs Ref)    : " << error_exacto_trap << std::endl;
    
    return 0;
}
#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <gsl/gsl_integration.h>

// ============================================================
// Taller: Integracion y diferenciacion numerica
// Ejercicio 1 - C++ con GSL
//
// Integrantes:
// Jimmy Alexander Calderon Trujillo
// Sebastian Barahona
// ============================================================


// ============================================================
// FUNCION DEL EJERCICIO
//
// f(x) = exp(-0.4x) * (1 + 0.5*sin(3x))
// ============================================================

double funcion(double x, void* params)
{
    return std::exp(-0.4 * x) *
           (1.0 + 0.5 * std::sin(3.0 * x));
}


// ============================================================
// REGLA COMPUESTA DEL TRAPECIO
// ============================================================

double trapecio_compuesto(double a, double b, int n)
{
    // Tamano del paso
    double h = (b - a) / n;

    // Los extremos tienen peso 1/2
    double suma =
        0.5 * funcion(a, nullptr) +
        0.5 * funcion(b, nullptr);

    // Suma de los puntos interiores
    for (int i = 1; i < n; ++i)
    {
        double x = a + i * h;
        suma += funcion(x, nullptr);
    }

    return h * suma;
}


// ============================================================
// PROGRAMA PRINCIPAL
// ============================================================

int main()
{
    // Limites de integracion
    const double a = 0.0;
    const double b = 8.0;

    // Valor de referencia obtenido analiticamente
    const double I_ref = 2.559824508302;

    std::cout << std::fixed << std::setprecision(12);

    std::cout
        << "TALLER DE INTEGRACION Y DIFERENCIACION NUMERICA\n";

    std::cout
        << "Ejercicio 1 - C++ con GSL\n";

    std::cout
        << "Jimmy Alexander Calderon Trujillo\n";

    std::cout
        << "Sebastian Barahona\n\n";

    std::cout
        << "Valor de referencia de la integral: "
        << I_ref
        << "\n";


    // ========================================================
    // PARTE 1
    // REGLA COMPUESTA DEL TRAPECIO
    // ========================================================

    int valores_n[] = {
        10,
        20,
        50,
        100,
        500,
        1000
    };

    std::cout
        << "\nREGLA COMPUESTA DEL TRAPECIO\n";

    std::cout
        << "--------------------------------------------------------------------------\n";

    std::cout
        << "   n          Integral             Error absoluto       Tiempo (s)\n";

    std::cout
        << "--------------------------------------------------------------------------\n";


    for (int n : valores_n)
    {
        // Inicio de medicion
        auto inicio =
            std::chrono::high_resolution_clock::now();

        // Integracion
        double I_trap =
            trapecio_compuesto(a, b, n);

        // Fin de medicion
        auto fin =
            std::chrono::high_resolution_clock::now();

        // Tiempo transcurrido
        std::chrono::duration<double> tiempo =
            fin - inicio;

        // Error real respecto a la referencia
        double error_abs =
            std::abs(I_ref - I_trap);

        // Resultados
        std::cout
            << std::setw(4)
            << n
            << "     "

            << std::fixed
            << std::setprecision(12)
            << std::setw(16)
            << I_trap
            << "     "

            << std::scientific
            << std::setprecision(6)
            << std::setw(14)
            << error_abs
            << "     "

            << std::setw(12)
            << tiempo.count()

            << '\n';
    }


    // ========================================================
    // PARTE 2
    // CUADRATURA ADAPTATIVA CON GSL
    // gsl_integration_qag()
    // ========================================================

    // Tolerancia absoluta
    const double epsabs = 1e-10;

    // Tolerancia relativa
    const double epsrel = 1e-10;

    // Numero maximo de subintervalos del workspace
    const size_t limite = 1000;

    // Reserva de memoria para el algoritmo adaptativo
    gsl_integration_workspace* workspace =
        gsl_integration_workspace_alloc(limite);

    // Estructura que GSL utiliza para representar la funcion
    gsl_function F;

    F.function = &funcion;
    F.params = nullptr;

    // Variables donde GSL almacenara los resultados
    double resultado_gsl = 0.0;
    double error_estimado_gsl = 0.0;


    // Inicio de medicion de tiempo
    auto inicio_gsl =
        std::chrono::high_resolution_clock::now();


    // Cuadratura adaptativa Gauss-Kronrod
    //
    // GSL_INTEG_GAUSS61 selecciona una regla
    // Gauss-Kronrod de 61 puntos.
    gsl_integration_qag(
        &F,                     // Funcion
        a,                      // Limite inferior
        b,                      // Limite superior
        epsabs,                 // Tolerancia absoluta
        epsrel,                 // Tolerancia relativa
        limite,                 // Limite de subintervalos
        GSL_INTEG_GAUSS61,      // Regla Gauss-Kronrod
        workspace,              // Espacio de trabajo
        &resultado_gsl,         // Resultado
        &error_estimado_gsl     // Error estimado por GSL
    );


    // Fin de medicion
    auto fin_gsl =
        std::chrono::high_resolution_clock::now();


    std::chrono::duration<double> tiempo_gsl =
        fin_gsl - inicio_gsl;


    // Error real respecto al valor de referencia
    double error_real_gsl =
        std::abs(I_ref - resultado_gsl);


    // ========================================================
    // RESULTADOS GSL
    // ========================================================

    std::cout
        << "\n\nCUADRATURA ADAPTATIVA CON GSL\n";

    std::cout
        << "----------------------------------------------------------\n";

    std::cout
        << std::fixed
        << std::setprecision(12);

    std::cout
        << "Resultado GSL       : "
        << resultado_gsl
        << '\n';

    std::cout
        << std::scientific
        << std::setprecision(6);

    std::cout
        << "Error estimado GSL  : "
        << error_estimado_gsl
        << '\n';

    std::cout
        << "Error vs referencia : "
        << error_real_gsl
        << '\n';

    std::cout
        << "Tolerancia absoluta : "
        << epsabs
        << '\n';

    std::cout
        << "Tolerancia relativa : "
        << epsrel
        << '\n';

    std::cout
        << "Tiempo GSL (s)      : "
        << tiempo_gsl.count()
        << '\n';


    // Liberar la memoria utilizada por GSL
    gsl_integration_workspace_free(workspace);


    return 0;
}
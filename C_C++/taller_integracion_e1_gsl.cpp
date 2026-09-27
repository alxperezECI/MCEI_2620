#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono>
 
#include <gsl/gsl_integration.h>
 
// Funcion del problema
double funcion(double x, void *params)
{
    return std::exp(-0.4 * x) *
           (1.0 + 0.5 * std::sin(3.0 * x));
}
 
// Primitiva analitica para obtener el valor de referencia
double primitiva(double x)
{
    return -2.5 * std::exp(-0.4 * x)
        + (0.5 * std::exp(-0.4 * x) / 9.16)
        * (-0.4 * std::sin(3.0 * x)
           - 3.0 * std::cos(3.0 * x));
}
 
int main()
{
    const double a = 0.0;
    const double b = 8.0;
 
    // Valor analitico de referencia
    const double I_ref = primitiva(b) - primitiva(a);
 
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Valor de referencia = "
<< I_ref << "\n\n";
 
    // -------------------------------------------------
    // 1. Regla compuesta del trapecio
    // -------------------------------------------------
 
    int n_valores[] = {10, 20, 50, 100, 500, 1000};
 
    std::cout
<< "n\tIntegral trapecio\tError\t\tTiempo (s)\n";
 
    for (int n : n_valores)
    {
        auto inicio = std::chrono::high_resolution_clock::now();
 
        double h = (b - a) / n;
 
        double suma =
            0.5 * funcion(a, nullptr)
            + 0.5 * funcion(b, nullptr);
 
        for (int i = 1; i < n; i++)
        {
            double x = a + i * h;
            suma += funcion(x, nullptr);
        }
 
        double I_trap = h * suma;
 
        auto fin = std::chrono::high_resolution_clock::now();
 
        double tiempo =
            std::chrono::duration<double>(fin - inicio).count();
 
        double error =
            std::abs(I_trap - I_ref);
 
        std::cout
<< n << "\t"
<< I_trap << "\t"
<< std::scientific << error << "\t"
<< tiempo << "\n"
<< std::fixed;
    }
 
    // -------------------------------------------------
    // 2. Cuadratura adaptativa GSL
    // -------------------------------------------------
 
    gsl_function F;
 
    F.function = &funcion;
    F.params = nullptr;
 
    const double epsabs = 1e-10;
    const double epsrel = 1e-10;
    const size_t limit = 1000;
 
    gsl_integration_workspace *workspace =
        gsl_integration_workspace_alloc(limit);
 
    double resultado;
    double error_estimado;
 
    auto inicio_gsl =
        std::chrono::high_resolution_clock::now();
 
    gsl_integration_qag(
&F,
        a,
        b,
        epsabs,
        epsrel,
        limit,
        6,
        workspace,
&resultado,
&error_estimado
    );
 
    auto fin_gsl =
        std::chrono::high_resolution_clock::now();
 
    double tiempo_gsl =
        std::chrono::duration<double>(
            fin_gsl - inicio_gsl).count();
 
    double error_real =
        std::abs(resultado - I_ref);
 
    std::cout << "\nCuadratura adaptativa GSL\n";
    std::cout << std::fixed << std::setprecision(12);
 
    std::cout << "Integral GSL         = "
<< resultado << "\n";
 
    std::cout << std::scientific;
 
    std::cout << "Error estimado       = "
<< error_estimado << "\n";
 
    std::cout << "Error vs referencia  = "
<< error_real << "\n";
 
    std::cout << "Tolerancia absoluta  = "
<< epsabs << "\n";
 
    std::cout << "Tolerancia relativa  = "
<< epsrel << "\n";
 
    std::cout << "Tiempo               = "
<< tiempo_gsl << " s\n";
 
    gsl_integration_workspace_free(workspace);
 
    return 0;
}
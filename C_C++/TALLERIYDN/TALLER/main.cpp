#include <iostream>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <gsl/gsl_integration.h>

// Definicion de la funcion f(x) solicitada en el taller
double f(double x, void *params)
{
    (void)(params); 
    return std::exp(-0.4 * x) * (1.0 + 0.5 * std::sin(3.0 * x));
}

// Implementación de la Regla de Simpson Compuesta (requiere n par)
double simpson_compuesto(double a, double b, int n)
{
    if (n % 2 != 0) n++; // Asegurar que n sea par

    double h = (b - a) / n;
    double suma_pares = 0.0;
    double suma_impares = 0.0;

    double term_extremos = f(a, NULL) + f(b, NULL);

    for (int i = 1; i < n; i++)
    {
        double x_i = a + i * h;
        if (i % 2 == 0)
        {
            suma_pares += f(x_i, NULL);
        }
        else
        {
            suma_impares += f(x_i, NULL);
        }
    }

    return (h / 3.0) * (term_extremos + 2.0 * suma_pares + 4.0 * suma_impares);
}

int main()
{
    double a = 0.0;
    double b = 8.0;
    double I_ref = 2.30231617823528; // Valor de referencia de alta precisión
    int N_vals[] = {10, 20, 50, 100, 500, 1000};
    int num_evals = sizeof(N_vals) / sizeof(N_vals[0]);

    std::cout << "=========================================================================\n";
    std::cout << "  n   |  Integral Aproximada (Simpson) |   Error Absoluto   |   Tiempo (ms)\n";
    std::cout << "=========================================================================\n";

    for (int i = 0; i < num_evals; i++)
    {
        int n = N_vals[i];

        std::clock_t start = std::clock();
        double I_simp = simpson_compuesto(a, b, n);
        std::clock_t end = std::clock();

        double t_exec = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        double err = std::fabs(I_simp - I_ref);

        std::cout << std::left << std::setw(5) << n << " | "
                  << std::setw(29) << std::fixed << std::setprecision(8) << I_simp << " | "
                  << std::setw(18) << std::scientific << std::setprecision(2) << err << " | "
                  << std::setw(12) << std::fixed << std::setprecision(6) << t_exec << "\n";
    }

    std::cout << "=========================================================================\n";

    // Integracion adaptativa con GSL (qags) como pide el taller
    gsl_integration_workspace *w = gsl_integration_workspace_alloc(1000);
    double result, error;
    gsl_function F;
    F.function = &f;
    F.params = NULL;

    std::clock_t start_gsl = std::clock();
    gsl_integration_qags(&F, a, b, 0, 1e-7, 1000, w, &result, &error);
    std::clock_t end_gsl = std::clock();

    double t_gsl = ((double)(end_gsl - start_gsl)) / CLOCKS_PER_SEC * 1000.0;

    std::cout << "\n--- SEGUNDA APROXIMACION: GSL (qags) ---\n";
    std::cout << "Integral aproximada (GSL)  : " << std::fixed << std::setprecision(8) << result << "\n";
    std::cout << "Error estimado GSL         : " << std::scientific << std::setprecision(2) << error << "\n";
    std::cout << "Error real vs I_ref        : " << std::scientific << std::setprecision(2) << std::fabs(result - I_ref) << "\n";
    std::cout << "Tiempo de ejecucion        : " << std::fixed << std::setprecision(6) << t_gsl << " ms\n";
    std::cout << "=========================================================================\n";

    gsl_integration_workspace_free(w);
    return 0;
}
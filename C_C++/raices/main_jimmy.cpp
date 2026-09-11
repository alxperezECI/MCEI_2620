#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>

// ============================================================
// SELECCION DE LA FUNCION
// funcion1: f(x) = x^3 - 5x + 1
// funcion2: f(x) = exp(-x) - x
// ============================================================

int funcion_seleccionada = 1;


// ============================================================
// FUNCION f(x)
// ============================================================

double f(double x, void *params)
{
    if (funcion_seleccionada == 1)
    {
        return x*x*x - 5.0*x + 1.0;
    }
    else
    {
        return std::exp(-x) - x;
    }
}


// ============================================================
// DERIVADA f'(x)
// ============================================================

double df(double x, void *params)
{
    if (funcion_seleccionada == 1)
    {
        return 3.0*x*x - 5.0;
    }
    else
    {
        return -std::exp(-x) - 1.0;
    }
}


// ============================================================
// FUNCION Y DERIVADA PARA GSL
// ============================================================

void fdf(double x, void *params, double *y, double *dy)
{
    *y = f(x, params);
    *dy = df(x, params);
}


// ============================================================
// METODOS CERRADOS
// Biseccion - False Position - Brent
// ============================================================

int metodo_cerrado(const std::string& metodo,
                   double x_inf,
                   double x_sup)
{
    const gsl_root_fsolver_type *T;
    gsl_root_fsolver *solver;

    gsl_function F;

    F.function = &f;
    F.params = nullptr;

    if (metodo == "biseccion")
    {
        T = gsl_root_fsolver_bisection;
    }
    else if (metodo == "falsepos")
    {
        T = gsl_root_fsolver_falsepos;
    }
    else if (metodo == "brent")
    {
        T = gsl_root_fsolver_brent;
    }
    else
    {
        std::cerr << "Metodo cerrado no reconocido." << std::endl;
        return 1;
    }

    solver = gsl_root_fsolver_alloc(T);

    int status = gsl_root_fsolver_set(
        solver,
        &F,
        x_inf,
        x_sup
    );

    if (status != GSL_SUCCESS)
    {
        std::cerr
            << "Error: el intervalo inicial no es valido "
            << "para el metodo seleccionado."
            << std::endl;

        gsl_root_fsolver_free(solver);

        return 1;
    }

    std::cout << "\n========================================\n";
    std::cout << "Metodo: "
              << gsl_root_fsolver_name(solver)
              << "\n";
    std::cout << "========================================\n";

    std::cout
        << "Iter\t"
        << "Inferior\t"
        << "Superior\t"
        << "Raiz\t\t"
        << "f(Raiz)"
        << std::endl;

    int iteracion = 0;
    const int max_iteraciones = 100;

    double raiz = 0.0;

    do
    {
        iteracion++;

        status = gsl_root_fsolver_iterate(solver);

        if (status != GSL_SUCCESS)
        {
            std::cerr << "Error durante la iteracion." << std::endl;
            break;
        }

        raiz = gsl_root_fsolver_root(solver);

        x_inf = gsl_root_fsolver_x_lower(solver);
        x_sup = gsl_root_fsolver_x_upper(solver);

        std::cout
            << iteracion << "\t"
            << std::fixed
            << std::setprecision(10)
            << x_inf << "\t"
            << x_sup << "\t"
            << raiz << "\t"
            << f(raiz, nullptr)
            << std::endl;

        status = gsl_root_test_interval(
            x_inf,
            x_sup,
            0.0,
            1e-8
        );

    }
    while (
        status == GSL_CONTINUE &&
        iteracion < max_iteraciones
    );

    std::cout << "\nRaiz encontrada: "
              << std::setprecision(12)
              << raiz
              << std::endl;

    std::cout << "f(raiz): "
              << f(raiz, nullptr)
              << std::endl;

    std::cout << "Iteraciones: "
              << iteracion
              << std::endl;

    gsl_root_fsolver_free(solver);

    return 0;
}


// ============================================================
// METODOS ABIERTOS
// Newton - Secante - Steffenson
// ============================================================

int metodo_abierto(const std::string& metodo,
                   double x_inicial)
{
    const gsl_root_fdfsolver_type *T;
    gsl_root_fdfsolver *solver;

    gsl_function_fdf FDF;

    FDF.f = &f;
    FDF.df = &df;
    FDF.fdf = &fdf;
    FDF.params = nullptr;

    if (metodo == "newton")
    {
        T = gsl_root_fdfsolver_newton;
    }
    else if (metodo == "secante")
    {
        T = gsl_root_fdfsolver_secant;
    }
    else if (metodo == "steffenson")
    {
        T = gsl_root_fdfsolver_steffenson;
    }
    else
    {
        std::cerr
            << "Metodo abierto no reconocido."
            << std::endl;

        return 1;
    }

    solver = gsl_root_fdfsolver_alloc(T);

    gsl_root_fdfsolver_set(
        solver,
        &FDF,
        x_inicial
    );

    std::cout << "\n========================================\n";
    std::cout << "Metodo: "
              << gsl_root_fdfsolver_name(solver)
              << "\n";
    std::cout << "========================================\n";

    std::cout
        << "Iter\t"
        << "Anterior\t"
        << "Actual\t\t"
        << "f(x)"
        << std::endl;

    int iteracion = 0;
    const int max_iteraciones = 100;

    double x = x_inicial;
    double x_anterior;

    int status;

    do
    {
        iteracion++;

        x_anterior = x;

        status = gsl_root_fdfsolver_iterate(solver);

        if (status != GSL_SUCCESS)
        {
            std::cerr
                << "Error durante la iteracion."
                << std::endl;

            break;
        }

        x = gsl_root_fdfsolver_root(solver);

        std::cout
            << iteracion << "\t"
            << std::fixed
            << std::setprecision(10)
            << x_anterior << "\t"
            << x << "\t"
            << f(x, nullptr)
            << std::endl;

        status = gsl_root_test_delta(
            x,
            x_anterior,
            0.0,
            1e-8
        );

    }
    while (
        status == GSL_CONTINUE &&
        iteracion < max_iteraciones
    );

    std::cout << "\nRaiz encontrada: "
              << std::setprecision(12)
              << x
              << std::endl;

    std::cout << "f(raiz): "
              << f(x, nullptr)
              << std::endl;

    std::cout << "Iteraciones: "
              << iteracion
              << std::endl;

    gsl_root_fdfsolver_free(solver);

    return 0;
}


// ============================================================
// PROGRAMA PRINCIPAL
// ============================================================

int main(int argc, char *argv[])
{
    gsl_set_error_handler_off();

    // Se requieren dos parametros:
    // 1. Funcion
    // 2. Metodo

    if (argc < 3)
    {
        std::cout << "\nUso del programa:\n\n";

        std::cout << "./raices funcion1 biseccion\n";
        std::cout << "./raices funcion1 falsepos\n";
        std::cout << "./raices funcion1 brent\n";
        std::cout << "./raices funcion1 newton\n";
        std::cout << "./raices funcion1 secante\n";
        std::cout << "./raices funcion1 steffenson\n\n";

        std::cout << "./raices funcion2 biseccion\n";
        std::cout << "./raices funcion2 falsepos\n";
        std::cout << "./raices funcion2 brent\n";
        std::cout << "./raices funcion2 newton\n";
        std::cout << "./raices funcion2 secante\n";
        std::cout << "./raices funcion2 steffenson\n";

        return 1;
    }

    std::string funcion = argv[1];
    std::string metodo = argv[2];


    // ========================================================
    // SELECCIONAR FUNCION
    // ========================================================

    if (funcion == "funcion1")
    {
        funcion_seleccionada = 1;
    }
    else if (funcion == "funcion2")
    {
        funcion_seleccionada = 2;
    }
    else
    {
        std::cerr
            << "Funcion no valida: "
            << funcion
            << std::endl;

        return 1;
    }


    // ========================================================
    // INFORMACION EN PANTALLA
    // ========================================================

    std::cout
        << "\n========================================\n";

    std::cout
        << " Taller de Busqueda de Raices con GSL\n";

    std::cout
        << " Jimmy Alexander Calderon Trujillo\n";

    if (funcion_seleccionada == 1)
    {
        std::cout
            << " f(x) = x^3 - 5x + 1\n";
    }
    else
    {
        std::cout
            << " f(x) = exp(-x) - x\n";
    }

    std::cout
        << "========================================\n";


    // ========================================================
    // METODOS CERRADOS
    // ========================================================

    if (
        metodo == "biseccion" ||
        metodo == "falsepos" ||
        metodo == "brent"
    )
    {
        // Las dos funciones tienen una raiz
        // dentro del intervalo [0,1].

        return metodo_cerrado(
            metodo,
            0.0,
            1.0
        );
    }


    // ========================================================
    // METODOS ABIERTOS
    // ========================================================

    if (
        metodo == "newton" ||
        metodo == "secante" ||
        metodo == "steffenson"
    )
    {
        // Valor inicial utilizado para la comparacion.

        return metodo_abierto(
            metodo,
            0.5
        );
    }


    std::cerr
        << "Metodo no valido: "
        << metodo
        << std::endl;

    return 1;
}
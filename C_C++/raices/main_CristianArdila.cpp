#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>

double f(double x, void *params) {
    return x*x*x - 5*x + 1;
   //return exp(-x) - x;
}

double df(double x, void *params) {
    return 3*x*x - 5;
    //return -exp(-x) - 1;
}

void fdf(double x, void *params, double *y, double *dy){
    *y = f(x, params);
    *dy = df(x, params);
}

void resolver_cerrado(const gsl_root_fsolver_type *T,
                      const char *nombre,
                      double x_lo,
                      double x_hi) {

    gsl_function F;
    F.function = &f;
    F.params = nullptr;

    gsl_root_fsolver *s = gsl_root_fsolver_alloc(T);
    gsl_root_fsolver_set(s, &F, x_lo, x_hi);

    std::cout << "\n=== " << nombre << " ===\n";
    std::cout << "iter\tinf\tsup\traiz\n";

    int status;
    int iter = 0;
    const int max_iter = 100;
    double r = 0.0;

    do {
        iter++;

        status = gsl_root_fsolver_iterate(s);
        r = gsl_root_fsolver_root(s);
        x_lo = gsl_root_fsolver_x_lower(s);
        x_hi = gsl_root_fsolver_x_upper(s);

        std::cout << iter << "\t"
                  << std::setprecision(10) << x_lo << "\t"
                  << x_hi << "\t"
                  << r << "\n";

        status = gsl_root_test_interval(x_lo, x_hi, 0.0, 1e-8);

    } while (status == GSL_CONTINUE && iter < max_iter);

    std::cout << "Raiz encontrada = "
              << std::setprecision(12) << r << "\n";
    std::cout << "Iteraciones = " << iter << "\n";

    gsl_root_fsolver_free(s);
}

void resolver_abierto(const gsl_root_fdfsolver_type *T,
                      const char *nombre,
                      double x_inicial) {

    gsl_function_fdf F;
    F.f = &f;
    F.df = &df;
    F.fdf = &fdf;
    F.params = nullptr;

    gsl_root_fdfsolver *s = gsl_root_fdfsolver_alloc(T);
    gsl_root_fdfsolver_set(s, &F, x_inicial);

    std::cout << "\n=== " << nombre << " ===\n";
    std::cout << "iter\tx\terror\n";

    int status;
    int iter = 0;
    const int max_iter = 100;
    double x = x_inicial;
    double x_anterior = x;

    do {
        iter++;
        x_anterior = x;

        status = gsl_root_fdfsolver_iterate(s);
        x = gsl_root_fdfsolver_root(s);

        double error = std::fabs(x - x_anterior);

        std::cout << iter << "\t"
                  << std::setprecision(12) << x << "\t"
                  << error << "\n";

        status = gsl_root_test_delta(x, x_anterior, 0.0, 1e-8);

    } while (status == GSL_CONTINUE && iter < max_iter);

    std::cout << "Raiz encontrada = "
              << std::setprecision(12) << x << "\n";
    std::cout << "Iteraciones = " << iter << "\n";

    gsl_root_fdfsolver_free(s);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cout << "Uso:\n";
        std::cout << "  ./raices biseccion\n";
        std::cout << "  ./raices falsepos\n";
        std::cout << "  ./raices brent\n";
        std::cout << "  ./raices newton\n";
        std::cout << "  ./raices secante\n";
        std::cout << "  ./raices steffenson\n";
        return 1;
    }

    std::string metodo = argv[1];

    /*
     * Funcion:
     * f(x) = x^3 - 5x + 1
     *
     * Se utilizan diferentes intervalos/valores iniciales
     * para localizar las tres raíces reales.
     */

    if (metodo == "biseccion") {

        const gsl_root_fsolver_type *T = gsl_root_fsolver_bisection;

        resolver_cerrado(T, "Biseccion - raiz 1", 0.0, 1.0);
        resolver_cerrado(T, "Biseccion - raiz 2", 1.0, 2.0);
        resolver_cerrado(T, "Biseccion - raiz 3", -3.0, -2.0);

    } else if (metodo == "falsepos") {

        const gsl_root_fsolver_type *T = gsl_root_fsolver_falsepos;

        resolver_cerrado(T, "False Position - raiz 1", 0.0, 1.0);
        resolver_cerrado(T, "False Position - raiz 2", 1.0, 2.0);
        resolver_cerrado(T, "False Position - raiz 3", -3.0, -2.0);

    } else if (metodo == "brent") {

        const gsl_root_fsolver_type *T = gsl_root_fsolver_brent;

        resolver_cerrado(T, "Brent - raiz 1", 0.0, 1.0);
        resolver_cerrado(T, "Brent - raiz 2", 1.0, 2.0);
        resolver_cerrado(T, "Brent - raiz 3", -3.0, -2.0);

    } else if (metodo == "newton") {

        const gsl_root_fdfsolver_type *T = gsl_root_fdfsolver_newton;

        resolver_abierto(T, "Newton - raiz 1", 0.2);
        resolver_abierto(T, "Newton - raiz 2", 2.0);
        resolver_abierto(T, "Newton - raiz 3", -2.0);

    } else if (metodo == "secante") {

        const gsl_root_fdfsolver_type *T = gsl_root_fdfsolver_secant;

        resolver_abierto(T, "Secante - raiz 1", 0.2);
        resolver_abierto(T, "Secante - raiz 2", 2.0);
        resolver_abierto(T, "Secante - raiz 3", -2.0);

    } else if (metodo == "steffenson") {

        const gsl_root_fdfsolver_type *T = gsl_root_fdfsolver_steffenson;

        resolver_abierto(T, "Steffenson - raiz 1", 0.2);
        resolver_abierto(T, "Steffenson - raiz 2", 2.0);
        resolver_abierto(T, "Steffenson - raiz 3", -2.0);

    } else {
        std::cout << "Metodo no reconocido: " << metodo << "\n";
        return 1;
    }

    return 0;
}

/*
=========================================================
CODGIOO ORIGINAL MAIN:
=========================================================
#include <iostream>
#include <iomanip>
#include <cmath> //Libreria funciones matematicas
#include <gsl/gsl_roots.h> //Algotirmos de busqueda de raices de GSL
#include <gsl/gsl_errno.h> //Permite manejar errores y estados de convergencia de GSL.

double f(double x, void *params) {
  return x*x*x - 5*x +1; //Definicion de la funcion
}

int main() {
  const gsl_root_fsolver_type *T; //Almacenara el tipo de algoritmo.
  gsl_root_fsolver *s; //Objeto que ejecutara el metodo numerico.
  gsl_function F; //Almacenamiento de la funcion.
  F.function = &f; //Asignacion de la funcion &Direccion de la funcion
  F.params = nullptr; //Mi funcion no recibe parametros extra.
  double x_lo = 0.0; //Define un intervalo
  double x_hi = 1.0; //Con el anterior se define el intervalo entre [0,1]
  T = gsl_root_fsolver_bisection; //Selecciona el metodo de la biseccion.
  s = gsl_root_fsolver_alloc(T); //Creacion del solver - Reserva memoria para el metodo seleccionado.
  gsl_root_fsolver_set(s, &F, x_lo, x_hi); //Inicializacion para resolver la funcion.
  std::cout << "iter\t" << "inf\t" << "sup\t" << "raíz\n"; //Ceacion de una tabla.

  int status; //Guarda el estado de convergencia
  int iter = 0; //Contador de iteraciones
  int max_iter = 100; //Limite maximo de iteraciones
  double r; //Almacenamiento de resultado de raiz

  do { //Repeticion del algoritmo
    iter++; // Incrementa cada iteracion
    status = gsl_root_fsolver_iterate(s); //Realiza una iteracion del metodo de biseccion
    r = gsl_root_fsolver_root(s); //Obtiene el valor estimado de las raices
    x_lo = gsl_root_fsolver_x_lower(s); //Obtiene nuevo limite inferior
    x_hi = gsl_root_fsolver_x_upper(s); //Obtiene limite superior
    std::cout << iter << "\t" << x_lo << "\t" << x_hi << "\t" << r << "\n"; //Mostrar resultados en tabla
          status = gsl_root_test_interval( x_lo, x_hi, 0.0, 1e-8); //Verifica el intervalo y si el error es suficientemente pequeño
  } while(status == GSL_CONTINUE && iter < max_iter); //Condicón del ciclo para continuar si el error es mayor a lo requerido

  std::cout << "\nRaiz encontrada = " << r << std::endl; gsl_root_fsolver_free(s); //Muestra el resultado de la raiz encontrada
  return 0;
}
*/
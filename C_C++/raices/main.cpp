#include <iostream>   // se encarga de la entrada y salida de datos
#include <iomanip>  // se encarga de la manipulacion de datos
#include <cmath>  // se encarga de las funciones matematicas
#include <gsl/gsl_roots.h> // se encarga de la busqueda de raices
#include <gsl/gsl_errno.h> // se encarga de la gestion de errores

double f(double x, void *params) {  
  return x*x*x - 5*x + 1;             
}

double df(double x, void *params) {  
  return 3*x*x - 5; // Derivada de x^3 - 5x + 1             
}

void fdf(double x, void *params, double *y, double *dy) {
  *y = x*x*x - 5*x + 1;
  *dy = 3*x*x - 5;
}

int main() {            // funcion principal
  // NOTA: Para métodos con derivada se usa obligatoriamente gsl_root_fdfsolver_type
  const gsl_root_fdfsolver_type *T;   
  gsl_root_fdfsolver *s; 
  
  // NOTA: Para Newton se usa obligatoriamente la estructura gsl_function_fdf
  gsl_function_fdf F; 
  F.f = &f;  
  F.df = &df;
  F.fdf = &fdf;
  F.params = nullptr;

  // Mantenemos tus variables de límites e inicio
  double x_lo = 0.0;
  double x_hi = 1.0;
  double x_inicial = 0.5; // Punto de partida que requiere Newton

  T = gsl_root_fdfsolver_steffenson;
  s = gsl_root_fdfsolver_alloc(T);
  
  // GSL inicializa Newton con un único punto de partida
  gsl_root_fdfsolver_set(s, &F, x_inicial);
  
  // Mantenemos tus mismas columnas en pantalla
  std::cout << "iter\t" << "inf\t" << "sup\t" << "raíz\n";

  int status; // se encarga de la gestion de errores
  int iter = 0; // se encarga de contar el numero de iteraciones
  int max_iter = 100;  // se encarga de limitar el numero de iteraciones
  double r = x_inicial; // se encarga de almacenar la raiz encontrada
  double r_anterior;

  do {
    iter++;
    r_anterior = r;

    status = gsl_root_fdfsolver_iterate(s);  // se encarga de iterar
    r = gsl_root_fdfsolver_root(s); // se encarga de almacenar la raiz encontrada
    
    // TRUCO: Como Newton no tiene intervalos fsolver_x_lower, usamos tus variables
    // para mostrar el paso anterior (x_lo) y el paso actual (x_hi)
    x_lo = r_anterior;
    x_hi = r;

    // Se muestra el resultado respetando tus columnas originales
    std::cout << iter << "\t" << x_lo << "\t" << x_hi << "\t" << r << "\n";   
    
    // NOTA: En métodos abiertos se usa test_delta para ver si la raíz ya no se mueve
    status = gsl_root_test_delta(r, r_anterior, 0.0, 1e-8); 

  } while(status == GSL_CONTINUE && iter < max_iter);  // se encarga de continuar iterando

  std::cout << "\nRaiz encontrada = " << r << std::endl; 
  gsl_root_fdfsolver_free(s);  // se encarga de liberar la memoria utilizada por el solver
  return 0;
}

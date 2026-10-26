// Por: David Orlando Riaño Morales
// Para cambiar de método, en la terminal de Ubuntu a la hora de ejecutar poner ./raices seguido del número del método que se desea utilizar, por ejemplo: ./raices 4
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>

// Se define la función a utilizar
double f(double x, void *params) {
  // Función 1: f(x) = x^3 - 5x + 1
  //return x * x * x - 5.0 * x + 1.0;
  // Función 2: f(x) = exp(-x) - x
  return exp(-x) - x;
}

// Se define la primera derivada de la función
double df(double x, void *params) {
  // Derivada función 1: df(x) = 3x^2 - 5 
  //return 3.0 * x * x - 5.0; 
  // Derivada función 2: df(x) = -exp(-x) - 1
  return -exp(-x) - 1.0;
}

// La función que combina f y df para los métodos abiertos
void fdf(double x, void *params, double *y, double *dy) { 
    *y = f(x, params); 
    *dy = df(x, params); 
}

int main(int argc, char *argv[]) {
  /* SELECTOR DE MÉTODO
    1 es bisección       
    2 es falsa posición  
    3 es Brent            
    4 es Newton
    5 es secante
    6 es Steffenson
    */
  // Se verifica que el usuario haya ingresado un argumento al llamar el ejecutable
  if (argc < 2) {
      std::cout << "Error: Debes ingresar el numero del metodo. Ejemplo: ./raices 4\n";
      return 1;
  }

  // Se convierte el argumento ingresado de la terminal a un número entero
  int metodo = std::stoi(argv[1]);

  if (metodo == 1 || metodo == 2 || metodo == 3) {

      // Sección para métodos cerrados

      const gsl_root_fsolver_type *T;
      gsl_root_fsolver *s;
      gsl_function F;
      
      F.function = &f;
      F.params = nullptr;
      // Intervalo para la búsqueda de la raíz, donde la raíz debe estar encerrada entre x_lo y x_hi
      double x_lo = -2.0;
      double x_hi = 3.0;

      // Selección del algoritmo cerrado
      if (metodo == 1){ 
        T = gsl_root_fsolver_bisection;
      }
      else if (metodo == 2){
        T = gsl_root_fsolver_falsepos;
      }
      else if (metodo == 3) {
        T = gsl_root_fsolver_brent;
      }
      // Inicialización del solver
      s = gsl_root_fsolver_alloc(T);
      // Configuración del solver con la función y el intervalo
      gsl_root_fsolver_set(s, &F, x_lo, x_hi);
      std::cout << "Método Cerrado (" << metodo << ")\n";
      std::cout << "iter\t" << "inf\t\t" << "sup\t\t" << "raíz\n";

      // Iteración del solver hasta converger o alcanzar el máximo de iteraciones
      int status; // Variable para almacenar el estado de la iteración
      int iter = 0; // Contador de iteraciones
      int max_iter = 100;
      double r;
      // Bucle de iteración para encontrar la raíz
      do {
        // Se incrementa el contador de iteraciones
          iter++;
          // Se realiza una iteración del solver
          status = gsl_root_fsolver_iterate(s);
          // Se obtiene la raíz actual estimada
          r = gsl_root_fsolver_root(s);
          x_lo = gsl_root_fsolver_x_lower(s); // Se obtiene el límite inferior del intervalo
          x_hi = gsl_root_fsolver_x_upper(s); // Se obtiene el límite superior del intervalo
          std::cout << iter << "\t" << x_lo << "\t" << x_hi << "\t" << r << "\n";
          status = gsl_root_test_interval(x_lo, x_hi, 0.0, 1e-8); // Se verifica la convergencia del intervalo, empleando un criterio de parada basado en la longitud del intervalo y una tolerancia de 1e-8
      } while(status == GSL_CONTINUE && iter < max_iter);
      // Se imprime la raíz encontrada y se libera la memoria del solver
      std::cout << "\nRaiz encontrada = " << r << std::endl; 
      gsl_root_fsolver_free(s);

  } 
  else if (metodo == 4 || metodo == 5 || metodo == 6) {
      // Sección para métodos abiertos

      const gsl_root_fdfsolver_type *T; // Tipo de solver para métodos abiertos, usando fdfsolver
      gsl_root_fdfsolver *s; 
      // Se define la función fdf que combina la función y su derivada para los métodos abiertos
      gsl_function_fdf F;
      F.f = &f;
      F.df = &df;
      F.fdf = &fdf;
      F.params = nullptr;
      // Valor inicial para la búsqueda de la raíz
      double x_inicial = -1.0;

      // Selección del algoritmo abierto
      if (metodo == 4){ 
        T = gsl_root_fdfsolver_newton;
      }

      else if (metodo == 5){ 
        T = gsl_root_fdfsolver_secant;
      }
      else if (metodo == 6){ 
        T = gsl_root_fdfsolver_steffenson;
      }

      // Inicialización del solver para métodos abiertos
      s = gsl_root_fdfsolver_alloc(T); 
      gsl_root_fdfsolver_set(s, &F, x_inicial);
      
      std::cout << "Método Abierto (" << metodo << ")\n";
      std::cout << "iter\t" << "raiz\n";
      // Iteración del solver hasta converger o alcanzar el máximo de iteraciones
      int status;
      int iter = 0;
      int max_iter = 100;
      double r0; // Variable para almacenar la raíz anterior y verificar la convergencia
      double r = x_inicial; // Se inicializa la raíz con el valor inicial

      do {
          //Se incrementa el contador de iteraciones
          iter++; 
          status = gsl_root_fdfsolver_iterate(s); // Se realiza una iteración del solver
          r0 = r; // Se almacena la raíz anterior antes de actualizarla
          r = gsl_root_fdfsolver_root(s); // Se obtiene la raíz actual estimada
          
          // Se imprime la iteración actual y la raíz estimada
          std::cout << iter << "\t" << r << "\n";
          status = gsl_root_test_delta(r, r0, 0.0, 1e-8); //Se verifica la convergencia de la raíz, empleando un criterio de parada basado en la diferencia entre la raíz actual y la anterior, con una tolerancia de 1e-8
      } while(status == GSL_CONTINUE && iter < max_iter);

      std::cout << "\nRaiz encontrada = " << r << std::endl; 
      gsl_root_fdfsolver_free(s);
      
  } 
  // Manejo de error cuando el caso ingresado no corresponde a ningún método valido
  else {
      std::cout << "Error: El método seleccionado (" << metodo << ") no es válido." << std::endl;
  }

  return 0;
}
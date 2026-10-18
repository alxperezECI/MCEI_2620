
// Para cambiar de método, se debe pasar el parámetro por consola al llamar el ejecutable (por ejemplo ./raices 2)

// Permite mostrar información en la pantalla mediante std::cout.
#include <iostream>
// Incluye herramientas para controlar la precisión y el formato de salida.
#include <iomanip>
// Incluye funciones matemáticas de la biblioteca estándar.
#include <cmath>
// Incluye std::atoi para convertir el argumento de consola en un número entero.
#include <cstdlib>
// Incluye los métodos de GSL para encontrar raíces de funciones.
#include <gsl/gsl_roots.h>
// Incluye constantes y códigos de estado utilizados por GSL.
#include <gsl/gsl_errno.h>

// Define la función de la que se busca una raíz, es decir, un valor x tal que f(x) = x^3 - 5x + 1.
// Si se desea cambiar por la segunda función, se cambia por: std::exp(-x) - x;
double f(double x, void *params)
{
  // Evalúa la función: x^3 - 5x + 1
  return std::exp(-x) - x;
}

// Define la derivada de la función, requerida por los métodos abiertos.
// Si se desea cambiar por la segunda función, se cambia por: -std::exp(-x) - 1;
double df(double x, void *params)
{
  // Evalúa la derivada: -exp(-x) - 1
  return -std::exp(-x) - 1.0;
}

// Evalúa la función y su derivada de forma simultánea.
void fdf(double x, void *params, double *y, double *dy)
{
  // Guarda en *y el valor de la función.
  *y = f(x, params);
  // Guarda en *dy el valor de la derivada.
  *dy = df(x, params);
}

// Punto de inicio de la ejecución del programa.
int main(int argc, char *argv[])
{
  // Variable para seleccionar el método a ejecutar por la terminal.
  // Cerrados (del 1 al 3): 1) Bisección, 2) Posición falsa, 3) Brent
  // Abiertos (del 4 al 6): 4) Newton, 5) Secante, 6) Steffenson
  int metodo = 1; // Bisección por defecto si no se pasa argumento

  // Se lee el parámetro pasado por consola (por ejemplo ./raices 2).
  if (argc > 1)
  {
    // Convierte el argumento de texto a número entero.
    metodo = std::atoi(argv[1]);
  }

  // Código para métodos cerrados (Bisección, Posición falsa y Brent).
  // Requieren un intervalo [x_lo, x_hi] que encierre a la raíz.
  if (metodo >= 1 && metodo <= 3)
  {
    // Puntero al tipo de método numérico cerrado que se utilizará.
    const gsl_root_fsolver_type *T;
    // Puntero al solucionador de raíces reservado por GSL.
    gsl_root_fsolver *s;
    // Estructura de GSL que contiene la función y sus parámetros.
    gsl_function F;

    // Asocia la función y sus parámetros con la estructura.
    F.function = &f;
    F.params = nullptr;

    // Define el extremo inferior del intervalo inicial de búsqueda.
    double x_lo = 0.0;
    // Define el extremo superior del intervalo inicial de búsqueda.
    double x_hi = 1.0;

    // Selección del algoritmo en GSL según la variable 'metodo'.
    if (metodo == 1)
    {
      // Selecciona el método de bisección.
      T = gsl_root_fsolver_bisection;
      std::cout << "Método Cerrado: Bisección\n";
    }
    else if (metodo == 2)
    {
      // Selecciona el método de posición falsa.
      T = gsl_root_fsolver_falsepos;
      std::cout << "Método Cerrado: Posición falsa\n";
    }
    else
    {
      // Selecciona el método de Brent.
      T = gsl_root_fsolver_brent;
      std::cout << "Método Cerrado: Brent\n";
    }

    // Reserva memoria para el solucionador usando el método seleccionado.
    s = gsl_root_fsolver_alloc(T);
    // Configura el solucionador con la función y el intervalo inicial.
    gsl_root_fsolver_set(s, &F, x_lo, x_hi);

    // Muestra el encabezado de la tabla de resultados.
    std::cout << "iter\t" << "inf\t" << "sup\t" << "raíz\n";

    // Almacena el estado de cada operación realizada por GSL.
    int status;
    // Inicializa en cero el contador de iteraciones.
    int iter = 0;
    // Límite de iteraciones para que el programa no se quede en un bucle infinito.
    int max_iter = 100;
    // Almacena la aproximación actual de la raíz.
    double r;

    // Ejecuta el bloque al menos una vez antes de evaluar la condición.
    do
    {
      // Aumenta en uno el número de iteración.
      iter++;
      // Calcula el siguiente paso del algoritmo.
      status = gsl_root_fsolver_iterate(s);
      // Extrae la aproximación actual de la raíz.
      r = gsl_root_fsolver_root(s);
      // Obtiene el nuevo límite inferior del intervalo.
      x_lo = gsl_root_fsolver_x_lower(s);
      // Obtiene el nuevo límite superior del intervalo.
      x_hi = gsl_root_fsolver_x_upper(s);

      // Muestra la iteración, el intervalo actual y la raíz aproximada.
      std::cout << iter << "\t" << x_lo << "\t" << x_hi << "\t" << r << "\n";

      // Criterio de parada: comprueba si el intervalo es menor a la tolerancia.
      // 0.0 es la tolerancia absoluta y 1e-8 la tolerancia relativa.
      status = gsl_root_test_interval(x_lo, x_hi, 0.0, 1e-8);
      // Continúa mientras GSL pida más iteraciones y no se alcance el máximo.
    } while (status == GSL_CONTINUE && iter < max_iter);

    // Muestra la última aproximación calculada.
    std::cout << "\nRaiz encontrada = " << r << std::endl;
    // Libera la memoria reservada para el solucionador.
    gsl_root_fsolver_free(s);
  }

  // Código para métodos abiertos (Newton, Secante y Steffenson).
  // Parten de un único valor inicial usando derivadas.
  else if (metodo >= 4 && metodo <= 6)
  {
    // Puntero al tipo de método numérico abierto que se utilizará.
    const gsl_root_fdfsolver_type *T;
    // Puntero al solucionador de raíces reservado por GSL.
    gsl_root_fdfsolver *s;
    // Estructura de GSL que contiene la función, su derivada y sus parámetros.
    gsl_function_fdf F;

    // Asocia la función, su derivada y la función conjunta con la estructura.
    F.f = &f;
    F.df = &df;
    F.fdf = &fdf;
    F.params = nullptr;

    // Valor inicial (semilla) para arrancar la búsqueda.
    double x_inicial = 0.0;

    // Selección del algoritmo en GSL según la variable 'metodo'.
    if (metodo == 4)
    {
      // Selecciona el método de Newton.
      T = gsl_root_fdfsolver_newton;
      std::cout << "Método Abierto: Newton\n";
    }
    else if (metodo == 5)
    {
      // Selecciona el método de la secante.
      T = gsl_root_fdfsolver_secant;
      std::cout << "Método Abierto: Secante\n";
    }
    else
    {
      // Selecciona el método de Steffenson.
      T = gsl_root_fdfsolver_steffenson;
      std::cout << "Método Abierto: Steffenson\n";
    }

    // Reserva memoria para el solucionador usando el método seleccionado.
    s = gsl_root_fdfsolver_alloc(T);
    // Configura el solucionador con la función y la aproximación inicial.
    gsl_root_fdfsolver_set(s, &F, x_inicial);

    // Muestra el encabezado de la tabla de resultados.
    std::cout << "iter\t" << "raiz\n";

    // Almacena el estado de cada operación realizada por GSL.
    int status;
    // Inicializa en cero el contador de iteraciones.
    int iter = 0;
    // Límite de iteraciones para que el programa no se quede en un bucle infinito.
    int max_iter = 100;
    // Almacena la aproximación de la iteración anterior.
    double r0;
    // Almacena la aproximación actual de la raíz.
    double r = x_inicial;

    // Ejecuta el bloque al menos una vez antes de evaluar la condición.
    do
    {
      // Aumenta en uno el número de iteración.
      iter++;
      // Calcula la siguiente aproximación.
      status = gsl_root_fdfsolver_iterate(s);
      // Guarda la estimación de la iteración anterior.
      r0 = r;
      // Obtiene la nueva aproximación calculada.
      r = gsl_root_fdfsolver_root(s);

      // Muestra la iteración y la raíz aproximada.
      std::cout << iter << "\t" << r << "\n";

      // Criterio de parada: comprueba si la diferencia |r - r0| es menor a la tolerancia.
      // 0.0 es la tolerancia absoluta y 1e-8 la tolerancia relativa.
      status = gsl_root_test_delta(r, r0, 0.0, 1e-8);
      // Continúa mientras GSL pida más iteraciones y no se alcance el máximo.
    } while (status == GSL_CONTINUE && iter < max_iter);

    // Muestra la última aproximación calculada.
    std::cout << "\nRaiz encontrada = " << r << std::endl;
    // Libera la memoria reservada para el solucionador.
    gsl_root_fdfsolver_free(s);
  }

  // Se ejecuta si el número recibido por consola está fuera del rango permitido.
  else
  {
    // Avisa al usuario que debe elegir un método entre 1 y 6.
    std::cout << "Error: Ingresa un método válido (1-6)." << std::endl;
  }

  // Indica que el programa terminó correctamente.
  return 0;
}
#include <stdio.h>
#include <math.h>
#include <omp.h>
 
double f(double x) {
    return sin(x);
}
 
double trapecio_parallel(double a, double b, int n) {
    double h = (b - a) / n;
    double suma = 0.0;
    #pragma omp parallel for reduction(+:suma)
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        suma += f(x);
    }
    return h * (0.5 * f(a) + suma + 0.5 * f(b));
}
 
int main() {
    double a = 0.0;
    double b = acos(-1.0);
    int n = 1000000;
    double inicio = omp_get_wtime();
    double resultado = trapecio_parallel(a, b, n);
    double fin = omp_get_wtime();
    printf("Resultado (paralelo): %.10f\n", resultado);
    printf("Tiempo: %.6f segundos\n", fin - inicio);

    return 0;
}

 
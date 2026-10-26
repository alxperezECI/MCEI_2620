#include <stdio.h>
#include <math.h>
#include <omp.h>

// Función a integrar
double f(double x) {
    return sin(x);
}

// Método del trapecio (Secuencial)
double trapecio_secuencial(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return h * sum;
}

// Método del trapecio (Paralelo con OpenMP)
double trapecio_parallel(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    
    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    return h * sum;
}

int main() {
    // Condiciones
    double a = 0.0, b = M_PI;
    int n = 10000000;

    // Ejecución Secuencial
    double t0_seq = omp_get_wtime();
    double resultado_seq = trapecio_secuencial(a, b, n);
    double t1_seq = omp_get_wtime();

    printf("Método del Trapecio Secuencial\n");
    printf("Resultado        : %.10f\n", resultado_seq);
    printf("Tiempo de ejec.  : %.5f segundos\n\n", t1_seq - t0_seq);

    // Ejecución Paralela
    double t0_par = omp_get_wtime();
    double resultado_par = trapecio_parallel(a, b, n);
    double t1_par = omp_get_wtime();

    printf("Método del Trapecio Paralelo\n");
    printf("Resultado        : %.10f\n", resultado_par);
    printf("Tiempo de ejec.  : %.5f segundos\n", t1_par - t0_par);

    return 0;
}
/*
===================================================================
TALLER: INTEGRACIÓN Y DIFERENCIACIÓN NUMÉRICA
Ejercicio 1: Integración de una función conocida en C (GSL)
Carpeta: taller_dif_int
Archivo: ejercicio1.c

Función: f(x) = exp(-0.4*x) * (1 + 0.5*sin(3*x)) en [0, 8]
Referencia analítica: I_ref = 2.55805663731
===================================================================
*/

#include <stdio.h>
#include <math.h>
#include <gsl/gsl_integration.h>

double f(double x, void *params) {
    (void)(params);
    return 2.0 + 0.35 * sin(0.7 * x) + 0.15 * cos(2.1 * x) + 0.03 * x;
}

int main() {
    double a = 0.0;
    double b = 9.8;
    int N = 100;
    double h = (b - a) / N;

    // Evaluaciones
    double x[101], y[101];
    for (int i = 0; i <= N; i++) {
        x[i] = a + i * h;
        y[i] = f(x[i], NULL);
    }

    // 1. Rectángulo (Izq)
    double suma_rect = 0.0;
    for (int i = 0; i < N; i++) suma_rect += y[i];
    double I_rect = h * suma_rect;

    // 2. Trapecio
    double suma_trap = 0.0;
    for (int i = 1; i < N; i++) suma_trap += y[i];
    double I_trap = (h / 2.0) * (y[0] + 2.0 * suma_trap + y[N]);

    // 3. Simpson 1/3
    double suma_4 = 0.0, suma_2 = 0.0;
    for (int i = 1; i < N; i++) {
        if (i % 2 != 0) suma_4 += y[i];
        else suma_2 += y[i];
    }
    double I_simp = (h / 3.0) * (y[0] + 4.0 * suma_4 + 2.0 * suma_2 + y[N]);

    // 4. Adaptativo GSL (Referencia)
    gsl_integration_workspace *w = gsl_integration_workspace_alloc(1000);
    double I_gsl, err_gsl;
    gsl_function F;
    F.function = &f;
    F.params = NULL;
    gsl_integration_qags(&F, a, b, 1e-8, 1e-8, 1000, w, &I_gsl, &err_gsl);
    gsl_integration_workspace_free(w);

    printf("=========================================================\n");
    printf("  EJERCICIO 1: FUNCIÓN CONTINUA (C - N=%d)\n", N);
    printf("=========================================================\n");
    printf("  Integral Rectangulo     : %.8f | Err: %.2e\n", I_rect, fabs(I_rect - I_gsl));
    printf("  Integral Trapecio       : %.8f | Err: %.2e\n", I_trap, fabs(I_trap - I_gsl));
    printf("  Integral Simpson 1/3    : %.8f | Err: %.2e\n", I_simp, fabs(I_simp - I_gsl));
    printf("  Integral GSL (Ref)      : %.8f\n", I_gsl);
    printf("=========================================================\n");

    return 0;
}

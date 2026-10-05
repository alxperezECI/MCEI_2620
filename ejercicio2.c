#include <stdio.h>
#include <stdlib.h>

#define MAX_DATOS 100

int main() {
    FILE *file = fopen("datos_sensor.csv", "r");
    if (file == NULL) {
        printf("Error al abrir el archivo datos_sensor.csv\n");
        return 1;
    }

    double x[MAX_DATOS], y[MAX_DATOS];
    int N = 0;

    // Leer los datos del archivo CSV
    while (fscanf(file, "%lf,%lf", &x[N], &y[N]) == 2) {
        N++;
    }
    fclose(file);

    double h = x[1] - x[0];

    // ---------------------------------------------------------
    // A. DIFERENCIACIÓN NUMÉRICA dy/dx
    // ---------------------------------------------------------
    double dydx[MAX_DATOS];

    // Extremo inicial: Diferencias hacia adelante
    dydx[0] = (y[1] - y[0]) / h;

    // Puntos intermedios: Diferencias centradas
    for (int i = 1; i < N - 1; i++) {
        dydx[i] = (y[i + 1] - y[i - 1]) / (2.0 * h);
    }

    // Extremo final: Diferencias hacia atrás
    dydx[N - 1] = (y[N - 1] - y[N - 2]) / h;

    // ---------------------------------------------------------
    // B. INTEGRACIÓN NUMÉRICA (3 MÉTODOS)
    // ---------------------------------------------------------
    // 1. Regla del Rectángulo (Extremo Izquierdo)
    double suma_rect = 0.0;
    for (int i = 0; i < N - 1; i++) {
        suma_rect += y[i];
    }
    double I_rect = h * suma_rect;

    // 2. Regla del Trapecio Compuesta
    double suma_trapecio = 0.0;
    for (int i = 1; i < N - 1; i++) {
        suma_trapecio += y[i];
    }
    double I_trap = (h / 2.0) * (y[0] + 2.0 * suma_trapecio + y[N - 1]);

    // 3. Regla de Simpson 1/3 Compuesta
    double suma_pares = 0.0;
    double suma_impares = 0.0;

    for (int i = 1; i < N - 1; i++) {
        if (i % 2 == 1) {
            suma_pares += y[i];
        } else {
            suma_impares += y[i];
        }
    }
    double I_simp = (h / 3.0) * (y[0] + 4.0 * suma_pares + 2.0 * suma_impares + y[N - 1]);

    // ---------------------------------------------------------
    // C. IMPRESIÓN DE RESULTADOS
    // ---------------------------------------------------------
    printf("=========================================================\n");
    printf("  EJERCICIO 2: PROCESAMIENTO DE DATOS DISCRETOS (C)\n");
    printf("=========================================================\n");
    printf("  Total de mediciones (N) : %d\n", N);
    printf("  Paso de muestreo (h)    : %.4f\n", h);
    printf("---------------------------------------------------------\n");
    printf("  Integral Rectangulo     : %.8f\n", I_rect);
    printf("  Integral Trapecio (C)   : %.8f\n", I_trap);
    printf("  Integral Simpson 1/3 (C): %.8f\n", I_simp);
    printf("---------------------------------------------------------\n");
    printf("  Primera derivada dy/dx(0) : %.6f\n", dydx[0]);
    printf("  Derivada central dy/dx(1) : %.6f\n", dydx[1]);
    printf("  Última derivada dy/dx(end): %.6f\n", dydx[N - 1]);
    printf("=========================================================\n");

    return 0;
}

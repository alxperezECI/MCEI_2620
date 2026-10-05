// SERGIO ANDRES MARIN PATIÑO 03/10/2026

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_deriv.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define MAXN 2000

static double x_eval(double t, void *p)
{
    (void)p;
    return 0.08 * t * t + 0.40 * sin(0.45 * t);
}

int main(void)
{
    FILE *f = fopen("trayectoria_robot.csv", "r");
    if (!f)
    {
        perror("abrir csv");
        return 1;
    }
    char line[256];
    double t[MAXN], x[MAXN], y[MAXN];
    int n = 0;
    if (!fgets(line, sizeof line, f))
    {
        fclose(f);
        return 1;
    } /* encabezado */
    while (fgets(line, sizeof line, f))
        if (sscanf(line, "%lf,%lf,%lf", &t[n], &x[n], &y[n]) == 3)
            n++;
    fclose(f);
    double h = t[1] - t[0];

    double vx[MAXN], vy[MAXN], v[MAXN], th[MAXN], om[MAXN];
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            vx[i] = (x[1] - x[0]) / h;
            vy[i] = (y[1] - y[0]) / h;
        }
        else if (i == n - 1)
        {
            vx[i] = (x[i] - x[i - 1]) / h;
            vy[i] = (y[i] - y[i - 1]) / h;
        }
        else
        {
            vx[i] = (x[i + 1] - x[i - 1]) / (2 * h);
            vy[i] = (y[i + 1] - y[i - 1]) / (2 * h);
        }
        v[i] = sqrt(vx[i] * vx[i] + vy[i] * vy[i]);
        th[i] = atan2(vy[i], vx[i]);
    }

    for (int i = 1; i < n; i++)
    {
        double d = th[i] - th[i - 1];

        while (d > M_PI)
        {
            th[i] -= 2 * M_PI;
            d = th[i] - th[i - 1];
        }

        while (d < -M_PI)
        {
            th[i] += 2 * M_PI;
            d = th[i] - th[i - 1];
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (i == 0)
            om[i] = (th[1] - th[0]) / h;
        else if (i == n - 1)
            om[i] = (th[i] - th[i - 1]) / h;
        else
            om[i] = (th[i + 1] - th[i - 1]) / (2 * h);
    }

    FILE *g = fopen("resultados_c.csv", "w");
    fprintf(g, "t,vx,vy,v,theta,omega\n");
    for (int i = 0; i < n; i++)
        fprintf(g, "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", t[i], vx[i], vy[i], v[i], th[i], om[i]);
    fclose(g);

    /* DEMO: GSL deriva una FUNCION evaluable, no un arreglo */
    gsl_function F;
    F.function = &x_eval;
    F.params = NULL;
    double r, e;
    gsl_deriv_central(&F, 1.0, 1e-6, &r, &e);
    printf("C: v(0)=%.6f v(25)=%.6f v(50)=%.6f\n", v[0], v[25], v[50]);
    printf("gsl_deriv_central x'(1.0)=%.8f +/- %.1e  (requiere f(t) evaluable)\n", r, e);
    return 0;
}
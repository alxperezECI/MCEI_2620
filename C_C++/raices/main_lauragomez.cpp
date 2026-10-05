#include <iostream>
#include <iomanip>
#include <cmath>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>


double f (double x, void * params) {
    return std::exp(-x) - x;
}

int main (void) {
    int status;
    int iter = 0, max_iter = 100;
    const gsl_root_fsolver_type *T;
    gsl_root_fsolver *s;
    
    double r = 0.0;
    double x_lo = 0.0, x_hi = 1.0; // Intervalo [0, 1]

    gsl_function F;
    F.function = &f;
    F.params = 0;
    T = gsl_root_fsolver_brent;
    s = gsl_root_fsolver_alloc (T);
    gsl_root_fsolver_set (s, &F, x_lo, x_hi);

    do {
        iter++;
        status = gsl_root_fsolver_iterate (s);
        r = gsl_root_fsolver_root (s);
        x_lo = gsl_root_fsolver_x_lower (s);
        x_hi = gsl_root_fsolver_x_upper (s);
        status = gsl_root_test_interval (x_lo, x_hi, 0, 1e-5);

        printf ("iter: %d  inf: %f  sup: %f  raiz: %f\n", iter, x_lo, x_hi, r);
    } while (status == GSL_CONTINUE && iter < max_iter);

    printf ("\nRaiz encontrada = %f\n", r);
    gsl_root_fsolver_free (s);
    return 0;
}

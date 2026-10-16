/* Simpson y trapecio: versiones secuenciales y OpenMP.
   Mediana de 9 repeticiones; impresiones fuera de la medicion. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#define REP 9
static volatile double observable;

double integrar(int simpson, int paralelo, int n, int *hilos) {
    const double a=0.0, b=acos(-1.0), h=(b-a)/n;
    double pares=0.0, impares=0.0;
    *hilos=1;
    if(paralelo) {
        #pragma omp parallel reduction(+:pares,impares)
        {
            #pragma omp single
            *hilos=omp_get_num_threads();
            #pragma omp for schedule(static)
            for(int i=1;i<n;i++) {
                double valor=sin(a+i*h);
                if(i%2==0) pares+=valor; else impares+=valor;
            }
        }
    } else {
        for(int i=1;i<n;i++) {
            double valor=sin(a+i*h);
            if(i%2==0) pares+=valor; else impares+=valor;
        }
    }
    /* Los dos esquemas comparten las sumas para comparar trabajo equivalente.
       En trapecio ambos grupos tienen peso 1; en Simpson, 2 y 4. */
    if(simpson) return h/3*(sin(a)+sin(b)+2*pares+4*impares);
    return h*(0.5*(sin(a)+sin(b))+pares+impares);
}
int comparar(const void *a,const void *b) {
    double x=*(const double*)a,y=*(const double*)b;
    return (x>y)-(x<y);
}
int main(void) {
    const int n=1000000;
    const int pedidos[]={1,2,4};
    omp_set_dynamic(0);
    FILE *csv=fopen("resultados.csv","w");
    FILE *raw=fopen("tiempos_repeticiones.csv","w");
    if(!csv || !raw) {perror("No se pueden crear resultados");return 1;}
    fprintf(csv,"metodo,modo,n,hilos_solicitados,hilos_reales,resultado,error_abs,mediana_s,speedup,eficiencia\n");
    fprintf(raw,"metodo,modo,hilos_solicitados,repeticion,tiempo_s\n");
    printf("n=%d; %d repeticiones; compilador %s; OpenMP=%d\n",n,REP,__VERSION__,_OPENMP);
    for(int metodo=0;metodo<2;metodo++) {
        const char *nombre=metodo?"Simpson":"Trapecio";
        double tiempos[4][REP], resultado[4], base;
        int reales[4];
        /* Calentar cada configuracion. */
        for(int c=0;c<4;c++) {
            omp_set_num_threads(c?pedidos[c-1]:1);
            observable=integrar(metodo,c!=0,n,&reales[c]);
        }
        /* Rotar orden para reducir sesgo por calentamiento/carga. */
        for(int r=0;r<REP;r++) for(int j=0;j<4;j++) {
            int c=(r+j)%4;
            omp_set_num_threads(c?pedidos[c-1]:1);
            double inicio=omp_get_wtime();
            resultado[c]=integrar(metodo,c!=0,n,&reales[c]);
            tiempos[c][r]=omp_get_wtime()-inicio;
            observable=resultado[c];
            fprintf(raw,"%s,%s,%d,%d,%.12g\n",nombre,c?"paralelo":"secuencial",c?pedidos[c-1]:1,r+1,tiempos[c][r]);
        }
        for(int c=0;c<4;c++) qsort(tiempos[c],REP,sizeof(double),comparar);
        base=tiempos[0][REP/2];
        for(int c=0;c<4;c++) {
            double med=tiempos[c][REP/2], sp=base/med;
            if(!isfinite(resultado[c]) || fabs(resultado[c]-2)>1e-8) {
                fprintf(stderr,"Validacion fallida\n");return 2;
            }
            printf("%-8s %-10s hilos=%d I=%.12f error=%.3e tiempo=%.6f s speedup=%.2f\n",nombre,c?"paralelo":"secuencial",reales[c],resultado[c],fabs(resultado[c]-2),med,sp);
            fprintf(csv,"%s,%s,%d,%d,%d,%.17g,%.12g,%.12g,%.8g,%.8g\n",nombre,c?"paralelo":"secuencial",n,c?pedidos[c-1]:1,reales[c],resultado[c],fabs(resultado[c]-2),med,sp,sp/reales[c]);
        }
    }
    fclose(csv);fclose(raw);return 0;
}

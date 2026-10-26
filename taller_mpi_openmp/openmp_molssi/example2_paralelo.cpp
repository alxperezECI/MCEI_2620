#include <stdio.h>
#include <omp.h>
#include <math.h>
 
int N = 10000000;
 
int main()
{
    double total_start = omp_get_wtime();
    double start_time;
 
    // Declare two arrays
    int* a = new int[N];
    int* b = new int[N];
 
    // Initialize a
    start_time = omp_get_wtime();
 
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        a[i] = 1;
    }
 
    printf("Initialize a time: %f\n", omp_get_wtime() - start_time);
 
    // Initialize b
    start_time = omp_get_wtime();
 
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        b[i] = 1 + i;
    }
 
    printf("Initialize b time: %f\n", omp_get_wtime() - start_time);
 
    // Add the two arrays
    start_time = omp_get_wtime();
 
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        a[i] = a[i] + b[i];
    }
 
    printf("Add arrays time: %f\n", omp_get_wtime() - start_time);
 
    // Average the result
    start_time = omp_get_wtime();
    double average = 0.0;
 
    #pragma omp parallel for reduction(+:average)
    for (int i = 0; i < N; i++) {
        average += a[i];
    }
 
    average = average / double(N);
 
    printf("Average result time: %f\n", omp_get_wtime() - start_time);
 
    // Print the result
    printf("\nAverage: %f\n", average);
 
    delete[] a;
    delete[] b;
 
    printf("Total time: %f\n", omp_get_wtime() - total_start);
 
    return 0;
}
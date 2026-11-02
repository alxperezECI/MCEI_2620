#include <iostream>
#include <omp.h>

int main() {
    // Definir región paralela
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        std::cout << "Hola desde el hilo " << thread_id << " de " << total_threads << std::endl;
    }
    return 0;
}
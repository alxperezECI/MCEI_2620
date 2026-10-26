#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <vector>
 
const int nside = 40;
const int natoms = nside * nside;
const double datoms = 1.0;
const double dt = 0.01;
const double mass = 1.0;
const int niter = 1000;
 
double force_zero_time = 0.0;
double force_calc_time = 0.0;
double vel_update_time = 0.0;
double coord_update_time = 0.0;
 
void calculate_velocities(
    double coords[][2],
    double velocities[][2],
    double* energy,
    double* potential)
{
    double start_loop = omp_get_wtime();
 
    double forces[natoms][2];
 
    #pragma omp parallel for
    for (int i = 0; i < natoms; i++) {
        forces[i][0] = 0.0;
        forces[i][1] = 0.0;
    }
 
    force_zero_time += omp_get_wtime() - start_loop;
 
    start_loop = omp_get_wtime();
 
    int num_threads = omp_get_max_threads();
 
    // Independent force arrays for each thread
    std::vector<double> local_forces(
        num_threads * natoms * 2, 0.0
    );
 
    double v = 0.0;
 
    #pragma omp parallel reduction(+:v)
    {
        int tid = omp_get_thread_num();
        double* local = local_forces.data()
                      + tid * natoms * 2;
 
        #pragma omp for schedule(static)
        for (int i = 0; i < natoms; i++) {
            for (int j = i + 1; j < natoms; j++) {
 
                double dx = coords[j][0] - coords[i][0];
                double dy = coords[j][1] - coords[i][1];
 
                double dr2 = dx * dx + dy * dy;
                double dr = sqrt(dr2);
 
                double fx = (dx / dr) * (1.0 / dr2);
                double fy = (dy / dr) * (1.0 / dr2);
 
                local[2*i]     -= fx;
                local[2*i + 1] -= fy;
 
                local[2*j]     += fx;
                local[2*j + 1] += fy;
 
                v += 1.0 / dr;
            }
        }
    }
 
    // Combine forces calculated by each thread
    #pragma omp parallel for
    for (int i = 0; i < natoms; i++) {
        for (int t = 0; t < num_threads; t++) {
            forces[i][0] += local_forces[t*natoms*2 + 2*i];
            forces[i][1] += local_forces[t*natoms*2 + 2*i + 1];
        }
    }
 
    *potential = v;
 
    force_calc_time += omp_get_wtime() - start_loop;
 
    // Update velocities
    start_loop = omp_get_wtime();
 
    double ke = 0.0;
 
    #pragma omp parallel for reduction(+:ke)
    for (int i = 0; i < natoms; i++) {
 
        ke += 0.5 * mass *
              (velocities[i][0] * velocities[i][0]
             + velocities[i][1] * velocities[i][1]);
 
        velocities[i][0] += (dt / mass) * forces[i][0];
        velocities[i][1] += (dt / mass) * forces[i][1];
    }
 
    *energy = *potential + ke;
 
    vel_update_time += omp_get_wtime() - start_loop;
}
 
int main()
{
    double start_time = omp_get_wtime();
 
    double coords[natoms][2];
    double velocities[natoms][2];
 
    #pragma omp parallel for
    for (int i = 0; i < natoms; i++) {
        coords[i][0] = (i % nside) * datoms;
        coords[i][1] = (i / nside) * datoms;
    }
 
    #pragma omp parallel for
    for (int i = 0; i < natoms; i++) {
        velocities[i][0] = 0.0;
        velocities[i][1] = 0.0;
    }
 
    double energy;
    double potential;
    double start_loop;
 
    for (int iter = 0; iter < niter; iter++) {
 
        calculate_velocities(
            coords, velocities, &energy, &potential
        );
 
        start_loop = omp_get_wtime();
 
        #pragma omp parallel for
        for (int i = 0; i < natoms; i++) {
            coords[i][0] += velocities[i][0] * dt;
            coords[i][1] += velocities[i][1] * dt;
        }
 
        coord_update_time += omp_get_wtime() - start_loop;
 
        printf("Iteration: %i      Energy: %f      PE: %f\n",
               iter, energy, potential);
    }
 
    printf("\nTimings:\n");
    printf("   Force Zero:      %f\n", force_zero_time);
    printf("   Force Calc:      %f\n", force_calc_time);
    printf("   Velocity Update: %f\n", vel_update_time);
    printf("   Coords Update:   %f\n", coord_update_time);
    printf("   Total:           %f\n",
           omp_get_wtime() - start_time);
 
    return 0;
}
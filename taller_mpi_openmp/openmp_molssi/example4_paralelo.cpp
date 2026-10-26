#include <iostream>
#include <cmath>
#include <cstdio>
#include <list>
#include <vector>
#include <utility>
#include <algorithm>
#include <omp.h>
 
// Global parameters
const int natom = 2000;
const double sigma = 20.0;
const double L = 1600.0;
const double epsilon = 1.0;
const double target_temp = 0.4;
const int nprint = 100;
const int nneigh = 20;
const int nstep = 1000;
 
const double r2cut_neigh = 35.0;
const double r2cut_force = 30.0;
const double excess_vel = 1.6;
 
// Timers
double time_force = 0.0;
double time_neigh = 0.0;
double time_total = 0.0;
 
typedef std::pair<int, int> pairT;
typedef std::pair<double, double> xyT;
typedef std::list<pairT> neighT;
typedef std::vector<xyT> coordT;
 
// Periodic boundary conditions
inline double periodic(double x, double box) {
    while (x > box) x -= box;
    while (x < 0) x += box;
    return x;
}
 
// Parallel construction of neighbor list
void neighbor_list(const coordT& coords, neighT& neigh) {
    double start = omp_get_wtime();
 
    neigh.clear();
 
    int num_threads = omp_get_max_threads();
 
    std::vector<std::vector<pairT>> local_neigh(num_threads);
 
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        std::vector<pairT>& local = local_neigh[tid];
 
        #pragma omp for schedule(dynamic, 16)
        for (int i = 0; i < natom; i++) {
            double xi = coords[i].first;
            double yi = coords[i].second;
 
            for (int j = 0; j < i; j++) {
                double dx = xi - coords[j].first;
                double dy = yi - coords[j].second;
 
                if (dx > L / 2.0) dx -= L;
                else if (dx < -L / 2.0) dx += L;
 
                if (dy > L / 2.0) dy -= L;
                else if (dy < -L / 2.0) dy += L;
 
                double r2 = (dx * dx + dy * dy) /
                            (sigma * sigma);
 
                if (r2 < r2cut_neigh) {
                    local.push_back(pairT(i, j));
                }
            }
        }
    }
 
    // Combine neighbor lists
    for (int t = 0; t < num_threads; t++) {
        for (const pairT& p : local_neigh[t]) {
            neigh.push_back(p);
        }
    }
 
    time_neigh += omp_get_wtime() - start;
}
 
// Parallel force calculation
coordT forces(
    const neighT& neigh,
    const coordT& coords,
    double& virial,
    double& pe)
{
    double start = omp_get_wtime();
 
    coordT f(natom, xyT(0.0, 0.0));
 
    int num_threads = omp_get_max_threads();
 
    std::vector<coordT> local_forces(
        num_threads,
        coordT(natom, xyT(0.0, 0.0))
    );
 
    // Convert list to vector for parallel iteration
    std::vector<pairT> pairs(neigh.begin(), neigh.end());
 
    virial = 0.0;
    pe = 0.0;
 
    const double fac = epsilon * 12.0 / (sigma * sigma);
 
    #pragma omp parallel reduction(+:virial,pe)
    {
        int tid = omp_get_thread_num();
        coordT& local = local_forces[tid];
 
        #pragma omp for schedule(static)
        for (int k = 0; k < static_cast<int>(pairs.size()); k++) {
            int i = pairs[k].first;
            int j = pairs[k].second;
 
            double dx = coords[i].first - coords[j].first;
            double dy = coords[i].second - coords[j].second;
 
            if (dx > L / 2.0) dx -= L;
            else if (dx < -L / 2.0) dx += L;
 
            if (dy > L / 2.0) dy -= L;
            else if (dy < -L / 2.0) dy += L;
 
            double r2 = (dx * dx + dy * dy) /
                        (sigma * sigma);
 
            if (r2 < r2cut_force) {
                double r6 = r2 * r2 * r2;
                double r12 = r6 * r6;
 
                double vij = epsilon *
                             (1.0 / r12 - 2.0 / r6);
 
                double df = fac *
                            (1.0 / (r12 * r2) -
                             1.0 / (r6 * r2));
 
                double dfx = df * dx;
                double dfy = df * dy;
 
                local[i].first += dfx;
                local[j].first -= dfx;
 
                local[i].second += dfy;
                local[j].second -= dfy;
 
                pe += vij;
                virial += dfx * dx + dfy * dy;
            }
        }
    }
 
    // Combine forces from all threads
    #pragma omp parallel for
    for (int i = 0; i < natom; i++) {
        double fx = 0.0;
        double fy = 0.0;
 
        for (int t = 0; t < num_threads; t++) {
            fx += local_forces[t][i].first;
            fy += local_forces[t][i].second;
        }
 
        f[i] = xyT(fx, fy);
    }
 
    time_force += omp_get_wtime() - start;
 
    return f;
}
 
// Limit a value to [-b, b]
inline double restrict(double a, double b) {
    if (a > b) return b;
    if (a < -b) return -b;
    return a;
}
 
// Initial optimization
void optimize(coordT& coords, neighT& neigh) {
    double dt = 0.1;
    double prev = 1e99;
 
    for (int step = 0; step < 600; step++) {
        if ((step % (3 * nneigh)) == 0 || step < 10) {
            neighbor_list(coords, neigh);
        }
 
        double virial, pe;
        coordT f = forces(neigh, coords, virial, pe);
 
        #pragma omp parallel for
        for (int i = 0; i < natom; i++) {
            double x = coords[i].first;
            double y = coords[i].second;
 
            double fx = restrict(f[i].first, 2.0);
            double fy = restrict(f[i].second, 2.0);
 
            coords[i] = xyT(
                periodic(x + dt * fx, L),
                periodic(y + dt * fy, L)
            );
        }
 
        if ((step % 50) == 0) {
            std::cout << "optim: " << pe
<< " " << dt << std::endl;
        }
 
        if (std::abs(pe - prev) < prev * 0.01) {
            break;
        }
 
        prev = pe;
    }
}
 
// Pseudo-random number generator
double drand() {
    static const unsigned int a = 1664525;
    static const unsigned int c = 1013904223;
    static unsigned int x = 23111;
    static const double fac = 2.3283064365386963e-10;
 
    x = a * x + c;
 
    return fac * x;
}
 
// Molecular dynamics simulation
void md() {
    const double dt = 0.03;
 
    std::cout << "Time step " << dt << std::endl;
 
    coordT coords(natom, xyT(0.0, 0.0));
    coordT v(natom, xyT(0.0, 0.0));
 
    double vxmean = 0.0;
    double vymean = 0.0;
 
    double box = std::min(
        std::sqrt(1.0 * natom) * sigma * 1.25, L
    );
 
    // Sequential random initialization
    for (int i = 0; i < natom; i++) {
        double xi = 0.0;
        double yi = 0.0;
 
        for (int attempt = 0; attempt < 5; attempt++) {
            xi = box * (drand() - 0.5) + L / 2.0;
            yi = box * (drand() - 0.5) + L / 2.0;
 
            double r2min = 1000000.0;
 
            for (int j = 0; j < i; j++) {
                double dx = xi - coords[j].first;
                double dy = yi - coords[j].second;
 
                r2min = std::min(
                    r2min, dx * dx + dy * dy
                );
            }
 
            if (r2min > 0.125 * sigma * sigma) {
                break;
            }
        }
 
        coords[i] = xyT(xi, yi);
 
        double vx = (drand() - 0.5) *
                    std::sqrt(2.0 * target_temp) *
                    2.0 * excess_vel;
 
        double vy = (drand() - 0.5) *
                    std::sqrt(2.0 * target_temp) *
                    2.0 * excess_vel;
 
        vxmean += vx;
        vymean += vy;
 
        v[i] = xyT(vx, vy);
    }
 
    vxmean /= natom;
    vymean /= natom;
 
    #pragma omp parallel for
    for (int i = 0; i < natom; i++) {
        v[i].first -= vxmean;
        v[i].second -= vymean;
    }
 
    neighT neigh;
 
    optimize(coords, neigh);
    neighbor_list(coords, neigh);
 
    double virial = 0.0;
    double temp = 0.0;
 
    double potential_energy;
    coordT f = forces(
        neigh, coords, virial, potential_energy
    );
 
    int step_stats = 0;
 
    for (int step = 1; step <= nstep; step++) {
 
        // Update velocities and positions
        #pragma omp parallel for
        for (int i = 0; i < natom; i++) {
            double vx = v[i].first;
            double vy = v[i].second;
 
            double Fx = f[i].first;
            double Fy = f[i].second;
 
            double x = coords[i].first;
            double y = coords[i].second;
 
            vx += Fx * dt * 0.5;
            vy += Fy * dt * 0.5;
 
            x += vx * dt;
            y += vy * dt;
 
            v[i] = xyT(vx, vy);
 
            coords[i] = xyT(
                periodic(x, L),
                periodic(y, L)
            );
        }
 
        // Update neighbor list
        if ((step % nneigh) == 0) {
            neighbor_list(coords, neigh);
        }
 
        double virial_step;
 
        f = forces(
            neigh, coords, virial_step, potential_energy
        );
 
        virial += virial_step;
 
        // Complete velocity update
        double kinetic_energy = 0.0;
 
        #pragma omp parallel for reduction(+:kinetic_energy)
        for (int i = 0; i < natom; i++) {
            double vx = v[i].first;
            double vy = v[i].second;
 
            double Fx = f[i].first;
            double Fy = f[i].second;
 
            vx += Fx * dt * 0.5;
            vy += Fy * dt * 0.5;
 
            v[i] = xyT(vx, vy);
 
            kinetic_energy += 0.5 * (vx * vx + vy * vy);
        }
 
        temp += kinetic_energy / (natom - 1.0);
        step_stats++;
 
        if ((step % nprint) == 0) {
 
            if (step == nprint) {
                printf("\n");
                printf("    time         ke            pe             e            T          P\n");
                printf("  -------    -----------   ------------  ------------    ------    ------\n");
            }
 
            temp /= step_stats;
            virial = 0.5 * virial / step_stats;
 
            double pressure =
                (natom * temp + virial) / (L * L);
 
            double energy =
                kinetic_energy + potential_energy;
 
            double vscale =
                std::sqrt(target_temp / temp);
 
            if (step >= (nstep / 3)) {
                vscale = 1.0;
            }
 
            const char scaling[2] = {' ', '*'};
 
            printf(
                "%9.2f   %12.5f   %12.5f  %12.5f %8.3f %12.8f %c\n",
                step * dt,
                kinetic_energy,
                potential_energy,
                energy,
                temp,
                pressure,
                scaling[vscale != 1.0]
            );
 
            #pragma omp parallel for
            for (int i = 0; i < natom; i++) {
                v[i].first *= vscale;
                v[i].second *= vscale;
            }
 
            temp = 0.0;
            virial = 0.0;
            step_stats = 0;
        }
    }
}
 
int main() {
    time_force = 0.0;
    time_neigh = 0.0;
    time_total = 0.0;
 
    double start = omp_get_wtime();
 
    md();
 
    time_total = omp_get_wtime() - start;
 
    printf(
        "times:  force=%.2fs  neigh=%.2fs  total=%.2fs\n",
        time_force,
        time_neigh,
        time_total
    );
 
    return 0;
}
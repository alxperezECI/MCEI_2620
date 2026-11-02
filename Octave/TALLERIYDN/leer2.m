

% 1. Definicion de la funcion y parametros
f = @(x) exp(-0.4*x) .* (1 + 0.5*sin(3*x));
a = 0; 
b = 8;
I_ref = 2.30231617823528;

N_vals = [10, 20, 50, 100, 500, 1000];

fprintf('=========================================================================\n');
fprintf('  n   |  Integral Aproximada (Trap) |   Error Absoluto   |   Tiempo (s)\n');
fprintf('=========================================================================\n');

for n = N_vals
    tic;
    x = linspace(a, b, n + 1);
    y = f(x);
    h = (b - a) / n;
    
    I_trap = h * (0.5*y(1) + sum(y(2:end-1)) + 0.5*y(end));
    t_exec = toc;
    err = abs(I_trap - I_ref);
    
    fprintf('%-5d | %-27.8f | %-18.2e | %-12.6f\n', n, I_trap, err, t_exec);
end

fprintf('=========================================================================\n');

tic;
I_quad = quad(f, a, b);
t_quad = toc;

fprintf('\n--- SEGUNDA APROXIMACION: quad() DE OCTAVE ---\n');
fprintf('Integral aproximada (quad) : %.8f\n', I_quad);
fprintf('Error absoluto             : %.2e\n', abs(I_quad - I_ref));
fprintf('Tiempo de ejecucion        : %.6f s\n', t_quad);
fprintf('=========================================================================\n');
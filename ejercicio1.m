% =========================================================
% EJERCICIO 1: INTEGRACIÓN DE FUNCIÓN CONTINUA (OCTAVE)
% =========================================================

% Definición de la función
f = @(x) 2.0 + 0.35*sin(0.7*x) + 0.15*cos(2.1*x) + 0.03*x;

a = 0.0;
b = 9.8;  % Coincide con x_end de los 50 puntos (h=0.2 * 49)
N = 100;  % 100 subintervalos (N_puntos = 101)
h = (b - a) / N;
x = linewidth = a:h:b;
y = f(x);

% 1. Regla del Rectángulo (Extremo Izquierdo)
I_rect = h * sum(y(1:N));

% 2. Regla del Trapecio Compuesta
I_trap = (h / 2) * (y(1) + 2 * sum(y(2:N)) + y(N+1));

% 3. Regla de Simpson 1/3 Compuesta (N es par)
I_simp = (h / 3) * (y(1) + 4 * sum(y(2:2:N)) + 2 * sum(y(3:2:N-1)) + y(N+1));

% 4. Referencia / Nativa (Integral Cuadratura)
I_ref = integral(f, a, b);

fprintf('=========================================================\n');
fprintf('  EJERCICIO 1: FUNCIÓN CONTINUA (OCTAVE - N=%d)\n', N);
fprintf('=========================================================\n');
fprintf('  Integral Rectángulo       : %.8f | Err: %.2e\n', I_rect, abs(I_rect - I_ref));
fprintf('  Integral Trapecio         : %.8f | Err: %.2e\n', I_trap, abs(I_trap - I_ref));
fprintf('  Integral Simpson 1/3      : %.8f | Err: %.2e\n', I_simp, abs(I_simp - I_ref));
fprintf('  Integral Nativa (Ref)     : %.8f\n', I_ref);
fprintf('=========================================================\n');

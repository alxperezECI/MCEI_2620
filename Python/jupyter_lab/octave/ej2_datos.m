% ============================================================
% Ejercicio 2 - Integración de 50 datos equiespaciados
% Parte B: GNU Octave
% ============================================================

clear; clc; close all;

% Ruta raíz del taller
RAIZ = '/home/lunam/MCEI_2620/Python/jupyter_lab';

% --- Paso 1: Cargar el CSV ---
data = dlmread(fullfile(RAIZ, 'datos', 'datos_sensor.csv'), ',', 1, 0);
x = data(:, 1);
y = data(:, 2);

% --- Paso 2: Verificación de los datos ---
n_datos = length(x);
h = x(2) - x(1);
h_constante = all(abs(diff(x) - h) < 1e-10);
faltantes = any(isnan(x)) || any(isnan(y));

printf('Numero de datos:  %d\n', n_datos);
printf('x va de:          %.2f a %.2f\n', x(1), x(end));
printf('Paso h:           %.6f\n', h);
printf('Equiespaciados?   %d\n', h_constante);
printf('Datos faltantes?  %d\n', faltantes);
printf('\n');

% --- Paso 3: Trapecio compuesto (explícito) ---
t0 = tic;
I_trap = h * ( y(1)/2 + sum(y(2:end-1)) + y(end)/2 );
t_trap = toc(t0);
printf('Trapecio (explicito):  I = %.10f   (t = %.6f s)\n', I_trap, t_trap);

% --- Paso 4: Trapecio con trapz de Octave ---
t0 = tic;
I_trapz = trapz(x, y);
t_trapz = toc(t0);
printf('Trapecio (trapz):      I = %.10f   (t = %.6f s)\n', I_trapz, t_trapz);

% --- Paso 5: Simpson compuesto ---
% 50 datos -> 49 intervalos (impar).
% Aplicamos Simpson en los primeros 48 intervalos y trapecio en el ultimo.
x_s = x(1:49);
y_s = y(1:49);

I_simp = (h/3) * ( y_s(1) + y_s(end) + ...
                   4*sum(y_s(2:2:end-1)) + ...
                   2*sum(y_s(3:2:end-2)) );

I_trap_last = h * (y(49) + y(50)) / 2;

I_simpson_last = I_simp + I_trap_last;

printf('Simpson (ultimo):      I = %.10f\n', I_simpson_last);
printf('\n');

% --- Paso 6: Comparación ---
printf('|trapecio - simpson| = %.3e\n', abs(I_trap - I_simpson_last));

% --- Paso 7: Gráfica ---
figure;
plot(x, y, 'bo-', 'MarkerSize', 4, 'LineWidth', 1);
hold on;
fill([x; flipud(x)], [y; zeros(size(y))], 'b', 'FaceAlpha', 0.2, 'EdgeColor', 'none');
hold off;
xlabel('x');
ylabel('y');
title(sprintf('Ejercicio 2 — Area aproximada = %.5f', I_trap));
legend('Datos del sensor', 'Area aproximada');
grid on;
print(fullfile(RAIZ, 'resultados', 'ej2_octave.png'), '-dpng', '-r150');

% --- Paso 8: Guardar resultados ---
fid = fopen(fullfile(RAIZ, 'resultados', 'ej2_octave.csv'), 'w');
fprintf(fid, 'metodo,integral\n');
fprintf(fid, 'trapecio_explicito,%.10f\n', I_trap);
fprintf(fid, 'trapecio_trapz,%.10f\n', I_trapz);
fprintf(fid, 'simpson,%.10f\n', I_simpson_last);
fclose(fid);

printf('Resultados guardados en resultados/ej2_octave.csv\n');

input('ENTER para salir');
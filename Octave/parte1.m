% =========================================================================
% PARTE 1 - GNU Octave
% =========================================================================
graphics_toolkit('gnuplot');

clear; clc; close all;

% Iniciar la medición del tiempo de ejecución
tic;

% Lectura del archivo CSV omitiendo la fila de encabezados
datos = csvread('/home/paula/MCEI_2620/Python/trayectoria_robot.csv', 1, 0);

t = datos(:, 1);
x = datos(:, 2);
y = datos(:, 3);

N = length(t);
h = t(2) - t(1);

vx = zeros(N, 1);
vy = zeros(N, 1);

% Diferencias centrales para puntos internos
for i = 2:(N-1)
    vx(i) = (x(i+1) - x(i-1)) / (2 * h);
    vy(i) = (y(i+1) - y(i-1)) / (2 * h);
end

% Diferencias hacia adelante y hacia atrás para bordes
vx(1) = (x(2) - x(1)) / h;
vy(1) = (y(2) - y(1)) / h;
vx(N) = (x(N) - x(N-1)) / h;
vy(N) = (y(N) - y(N-1)) / h;

% Cálculo de la velocidad lineal
v = sqrt(vx.^2 + vy.^2);

% Orientación y desenvolvimiento angular
theta_raw = atan2(vy, vx);
theta = unwrap(theta_raw);

% Velocidad angular mediante diferencias finitas
omega = zeros(N, 1);
for i = 2:(N-1)
    omega(i) = (theta(i+1) - theta(i-1)) / (2 * h);
end
omega(1) = (theta(2) - theta(1)) / h;
omega(N) = (theta(N) - theta(N-1)) / h;

% Generación de gráficos (modo silencioso)
figure('visible', 'off');

subplot(2, 2, 1);
plot(x, y, 'b.-');
title('Trayectoria del Robot (y vs x)');
xlabel('x [m]'); ylabel('y [m]'); grid on;

subplot(2, 2, 2);
plot(t, v, 'r.-');
title('Velocidad Lineal (v vs t)');
xlabel('t [s]'); ylabel('v [m/s]'); grid on;

subplot(2, 2, 3);
plot(t, theta, 'g.-');
title('Orientación (\theta vs t)');
xlabel('t [s]'); ylabel('\theta [rad]'); grid on;

subplot(2, 2, 4);
plot(t, omega, 'm.-');
title('Velocidad Angular (\omega vs t)');
xlabel('t [s]'); ylabel('\omega [rad/s]'); grid on;

% Guardar la imagen con alta resolución
print('resultado_octave.png', '-dpng', '-r300');

% Detener el cronómetro y guardar el tiempo
tiempo_ejecucion = toc;

printf('¡Cálculo completado! La gráfica se guardó como resultado_octave.png\n');
printf('Tiempo de ejecución: %.6f segundos\n', tiempo_ejecucion);
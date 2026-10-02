clear;
clc;
close all;
tic;
 
% Taller de diferenciacion numerica
% Cinematica diferencial de un robot
% Parte 1 - GNU Octave
 
% Cargar datos
data = dlmread("../trayectoria_robot.csv", ",", 1, 0);
 
t = data(:,1);
x = data(:,2);
y = data(:,3);
 
N = length(t);
h = t(2) - t(1);
 
% Inicializar vectores
vx = zeros(N,1);
vy = zeros(N,1);
 
% Diferencias centrales para los puntos interiores
for i = 2:N-1
    vx(i) = (x(i+1) - x(i-1)) / (2*h);
    vy(i) = (y(i+1) - y(i-1)) / (2*h);
end
 
% Diferencias unilaterales en los extremos
vx(1) = (x(2) - x(1)) / h;
vy(1) = (y(2) - y(1)) / h;
 
vx(N) = (x(N) - x(N-1)) / h;
vy(N) = (y(N) - y(N-1)) / h;
 
% Velocidad lineal
v = sqrt(vx.^2 + vy.^2);
 
% Orientacion instantanea
theta = atan2(vy, vx);
 
% Desenvolvimiento angular
theta = unwrap(theta);
 
% Velocidad angular
omega = zeros(N,1);
 
% Diferencias centrales
for i = 2:N-1
    omega(i) = (theta(i+1) - theta(i-1)) / (2*h);
end
 
% Diferencias unilaterales en los extremos
omega(1) = (theta(2) - theta(1)) / h;
omega(N) = (theta(N) - theta(N-1)) / h;
 
% Mostrar informacion
fprintf("Numero de muestras: %d\n", N);
fprintf("Paso temporal h: %.2f s\n", h);
fprintf("Velocidad inicial: %.6f m/s\n", v(1));
fprintf("Velocidad final: %.6f m/s\n", v(N));
fprintf("Omega inicial: %.6f rad/s\n", omega(1));
fprintf("Omega final: %.6f rad/s\n", omega(N));
 
 tiempo_ejecucion = toc;
fprintf("Tiempo de ejecucion: %.6f s\n", tiempo_ejecucion);

% Graficas

% Se generan en una sola figura y se guardan en formato PNG
 
graphics_toolkit("gnuplot");
 
figure("visible", "off");
 
subplot(2,2,1);

plot(x, y, "LineWidth", 1.5);

grid on;

xlabel("x [m]");

ylabel("y [m]");

title("Trayectoria del robot");
 
subplot(2,2,2);

plot(t, v, "LineWidth", 1.5);

grid on;

xlabel("t [s]");

ylabel("v [m/s]");

title("Velocidad lineal");
 
subplot(2,2,3);

plot(t, theta, "LineWidth", 1.5);

grid on;

xlabel("t [s]");

ylabel("\theta [rad]");

title("Orientacion");
 
subplot(2,2,4);

plot(t, omega, "LineWidth", 1.5);

grid on;

xlabel("t [s]");

ylabel("\omega [rad/s]");

title("Velocidad angular");
 
print("diferenciacion_robot_octave.png", "-dpng", "-r300");
 
fprintf("Grafica guardada en diferenciacion_robot_octave.png\n");
 
clear;
clc;
close all;
 
% Taller - Ecuaciones diferenciales
% Sistema masa-resorte-amortiguador
% Metodo de Runge-Kutta de cuarto orden (RK4)
 
% Parametros del sistema
m = 20;          % masa [kg]
k = 20;          % constante del resorte [N/m]
 
% Coeficientes de amortiguamiento
c_valores = [5, 40, 200];
 
% Condiciones iniciales
x0 = 1;          % desplazamiento inicial [m]
v0 = 0;          % velocidad inicial [m/s]
 
% Intervalo de tiempo
t0 = 0;
tf = 15;
h = 0.01;
 
t = t0:h:tf;
N = length(t);
 
% Matriz para almacenar los desplazamientos
% Cada columna corresponde a un valor de c
X = zeros(N, length(c_valores));
 
tic;
 
% Resolver para cada coeficiente de amortiguamiento
for j = 1:length(c_valores)
 
    c = c_valores(j);
 
    % Estado:
    % y(1) = desplazamiento x
    % y(2) = velocidad dx/dt
    y = zeros(2, N);
 
    % Condiciones iniciales
    y(:,1) = [x0; v0];
 
    % Sistema de ecuaciones diferenciales
    f = @(ti, yi) [
        yi(2);
        -(c/m)*yi(2) - (k/m)*yi(1)
    ];
 
    % Metodo RK4
    for i = 1:N-1
 
        k1 = f(t(i), y(:,i));
 
        k2 = f(t(i) + h/2, ...
               y(:,i) + (h/2)*k1);
 
        k3 = f(t(i) + h/2, ...
               y(:,i) + (h/2)*k2);
 
        k4 = f(t(i) + h, ...
               y(:,i) + h*k3);
 
        y(:,i+1) = y(:,i) + ...
                   (h/6)*(k1 + 2*k2 + 2*k3 + k4);
 
    end
 
    % Guardar desplazamiento
    X(:,j) = y(1,:)';
 
end
 
tiempo_ejecucion = toc;
 
% Mostrar resultados
fprintf("Metodo: Runge-Kutta de cuarto orden (RK4)\n");
fprintf("Masa m = %.2f kg\n", m);
fprintf("Constante k = %.2f N/m\n", k);
fprintf("Paso h = %.3f s\n", h);
fprintf("Numero de pasos = %d\n", N-1);
fprintf("Tiempo de ejecucion = %.6f s\n\n", tiempo_ejecucion);
 
for j = 1:length(c_valores)
 
    fprintf("c = %.0f N*s/m -> x(15 s) = %.6f m\n", ...
            c_valores(j), X(end,j));
 
end
 
% Grafica
graphics_toolkit("gnuplot");
 
figure("visible", "off");
 
plot(t, X(:,1), "LineWidth", 1.5);
hold on;
 
plot(t, X(:,2), "LineWidth", 1.5);
plot(t, X(:,3), "LineWidth", 1.5);
 
grid on;
 
xlabel("Tiempo [s]");
ylabel("Desplazamiento x [m]");
 
title("Sistema masa-resorte-amortiguador - RK4");
 
legend(
    "c = 5 N s/m - Subamortiguado",
    "c = 40 N s/m - Critico",
    "c = 200 N s/m - Sobreamortiguado",
    "location", "northeast"
);
 
hold off;
 
% Guardar grafica
print(
    "masa_resorte_amortiguador_octave.png",
    "-dpng",
    "-r300"
);
 
fprintf("\nGrafica guardada en masa_resorte_amortiguador_octave.png\n");
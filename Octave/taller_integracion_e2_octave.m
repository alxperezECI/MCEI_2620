clear;
clc;
close all;
 
% Taller de integracion y diferenciacion numerica
% Ejercicio 2 - Integracion de datos experimentales
 
% Leer los datos del archivo CSV
datos = dlmread('../Python/datos_sensor.csv', ',', 1, 0);
 
x = datos(:,1);
y = datos(:,2);
 
% --------------------------------------------------
% Verificacion de los datos
% --------------------------------------------------
 
N = length(x);
h = x(2) - x(1);
 
equiespaciados = all(abs(diff(x) - h) < 1e-12);
faltantes = any(isnan(datos(:)));
 
fprintf('Numero de datos       = %d\n', N);
fprintf('Paso h                = %.4f\n', h);
fprintf('Datos equiespaciados  = %d\n', equiespaciados);
fprintf('Valores faltantes     = %d\n\n', faltantes);
 
% --------------------------------------------------
% Regla compuesta del trapecio
% --------------------------------------------------
 
tic;
 
I_trap = h * ( ...
    y(1)/2 + ...
    sum(y(2:end-1)) + ...
    y(end)/2 );
 
tiempo_trap = toc;
 
fprintf('Integral por trapecio = %.12f\n', I_trap);
fprintf('Tiempo trapecio       = %.6e s\n', tiempo_trap);
 
% --------------------------------------------------
% Regla de Simpson
% --------------------------------------------------
 
% Numero de subintervalos
n = N - 1;
 
if mod(n,2) == 0
 
    tic;
 
    I_simp = (h/3) * ( ...
        y(1) + y(end) + ...
        4*sum(y(2:2:end-1)) + ...
        2*sum(y(3:2:end-2)) );
 
    tiempo_simp = toc;
 
    fprintf('Integral por Simpson  = %.12f\n', I_simp);
    fprintf('Tiempo Simpson        = %.6e s\n', tiempo_simp);
 
else
    fprintf('\nSimpson 1/3 compuesto no puede aplicarse directamente.\n');
    fprintf('Numero de subintervalos = %d (impar)\n', n);
end
 
% --------------------------------------------------
% Grafica de los datos
% --------------------------------------------------
 
figure('visible', 'off');
area(x, y);
hold on;
plot(x, y, 'o-');
xlabel('x');
ylabel('y');
title('Datos del sensor y area aproximada');
grid on;
print('integracion_datos_sensor.png', '-dpng');
close;
fprintf('\nGrafica guardada como integracion_datos_sensor.png\n');
 
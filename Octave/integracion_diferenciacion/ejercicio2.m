% ============================================================
% TALLER DE INTEGRACION Y DIFERENCIACION NUMERICA
% Ejercicio 2 - Datos experimentales
% Octave
%
% Jimmy Alexander Calderon Trujillo
% Sebastian Barahona
% ============================================================

clear;
clc;
close all;

% ------------------------------------------------------------
% 1. CARGA DE LOS DATOS EXPERIMENTALES
% ------------------------------------------------------------

ruta_script = fileparts(mfilename('fullpath'));
ruta_csv = fullfile(ruta_script, '..', '..', 'datos', 'datos_sensor.csv');
datos = dlmread(ruta_csv, ',', 1, 0);

x = datos(:,1);
y = datos(:,2);

N = length(x);

fprintf('EJERCICIO 2 - DATOS EXPERIMENTALES\n');
fprintf('---------------------------------------------\n');
fprintf('Numero de mediciones : %d\n', N);
fprintf('x inicial            : %.1f\n', x(1));
fprintf('x final              : %.1f\n', x(end));

% ------------------------------------------------------------
% 2. VERIFICACION DEL ESPACIAMIENTO
% ------------------------------------------------------------

dx = diff(x);
h = dx(1);

fprintf('Espaciado h          : %.1f\n', h);

if all(abs(dx - h) < 1e-12)
    fprintf('Espaciado uniforme   : SI\n');
else
    fprintf('Espaciado uniforme   : NO\n');
end

% ------------------------------------------------------------
% 3. INTEGRACION POR REGLA DEL TRAPECIO
% ------------------------------------------------------------

I_trap = trapz(x, y);

fprintf('\nINTEGRACION NUMERICA\n');
fprintf('---------------------------------------------\n');
fprintf('Integral por trapecio : %.12f\n', I_trap);

% ------------------------------------------------------------
% 4. REGLA DE SIMPSON
% ------------------------------------------------------------
%
% Tenemos 50 puntos, por lo tanto existen 49 subintervalos.
% La regla compuesta de Simpson 1/3 requiere un numero PAR
% de subintervalos.
%
% Por esta razon se aplica Simpson 1/3 a los primeros
% 49 puntos (48 subintervalos) y trapecio al ultimo intervalo.

n_simpson = N - 2;   % ultimo indice de subintervalos para Simpson

% Primeros 49 puntos: indices 1 hasta 49
y_simpson = y(1:49);

suma_impares = sum(y_simpson(2:2:end-1));
suma_pares   = sum(y_simpson(3:2:end-2));

I_simpson_parcial = (h/3) * ...
    (y_simpson(1) + ...
     4*suma_impares + ...
     2*suma_pares + ...
     y_simpson(end));

% Ultimo intervalo [9.6, 9.8] mediante trapecio
I_ultimo_trap = ...
    (x(50)-x(49)) * (y(49)+y(50)) / 2;

I_simpson_mixto = I_simpson_parcial + I_ultimo_trap;

fprintf('Simpson 1/3 [0,9.6]  : %.12f\n', I_simpson_parcial);
fprintf('Trapecio [9.6,9.8]   : %.12f\n', I_ultimo_trap);
fprintf('Metodo mixto total   : %.12f\n', I_simpson_mixto);

% ------------------------------------------------------------
% 5. GRAFICA DE LOS DATOS Y AREA APROXIMADA
% ------------------------------------------------------------

figure;

area(x, y);
hold on;

plot(x, y, 'o-', 'LineWidth', 1.5);

xlabel('x');
ylabel('y');
title('Ejercicio 2 - Datos experimentales del sensor');

grid on;

legend('Area aproximada', 'Mediciones', ...
       'Location', 'best');

hold off;


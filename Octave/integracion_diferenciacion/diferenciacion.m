% ============================================================
% TALLER DE INTEGRACION Y DIFERENCIACION NUMERICA
% Diferenciacion numerica de datos experimentales
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

% ------------------------------------------------------------
% 2. VERIFICACION DEL ESPACIAMIENTO
% ------------------------------------------------------------

dx = diff(x);
h = dx(1);

fprintf('DIFERENCIACION NUMERICA - OCTAVE\n');
fprintf('---------------------------------------------\n');
fprintf('Numero de mediciones : %d\n', N);
fprintf('Espaciado h          : %.1f\n', h);

if all(abs(dx - h) < 1e-12)
    fprintf('Espaciado uniforme   : SI\n');
else
    fprintf('Espaciado uniforme   : NO\n');
end

% ------------------------------------------------------------
% 3. DIFERENCIA HACIA ADELANTE
% ------------------------------------------------------------
%
% f'(xi) ~= [f(xi+1) - f(xi)] / h
%
% Se puede calcular desde el primer punto hasta el penultimo.
% En el ultimo punto no existe una medicion posterior.

derivada_adelante = NaN(N,1);

for i = 1:N-1
    derivada_adelante(i) = ...
        (y(i+1) - y(i)) / h;
end

% ------------------------------------------------------------
% 4. DIFERENCIA CENTRAL
% ------------------------------------------------------------
%
% f'(xi) ~= [f(xi+1) - f(xi-1)] / (2h)
%
% Solo puede calcularse en los puntos interiores porque
% necesita una medicion anterior y una posterior.

derivada_central = NaN(N,1);

for i = 2:N-1
    derivada_central(i) = ...
        (y(i+1) - y(i-1)) / (2*h);
end

% ------------------------------------------------------------
% 5. MOSTRAR ALGUNOS RESULTADOS
% ------------------------------------------------------------

fprintf('\nPRIMEROS RESULTADOS\n');
fprintf('-------------------------------------------------------------\n');
fprintf('   x          y        Adelante       Central\n');
fprintf('-------------------------------------------------------------\n');

for i = 1:10

    fprintf('%5.1f   %10.6f   ', x(i), y(i));

    if isnan(derivada_adelante(i))
        fprintf('      NaN      ');
    else
        fprintf('%10.6f   ', derivada_adelante(i));
    end

    if isnan(derivada_central(i))
        fprintf('      NaN\n');
    else
        fprintf('%10.6f\n', derivada_central(i));
    end

end

% ------------------------------------------------------------
% 6. INFORMACION SOBRE LOS EXTREMOS
% ------------------------------------------------------------

fprintf('\nTRATAMIENTO DE LOS EXTREMOS\n');
fprintf('---------------------------------------------\n');
fprintf(['Diferencia hacia adelante: no se calcula en ', ...
         'x = %.1f\n'], x(end));

fprintf(['Diferencia central: no se calcula en ', ...
         'x = %.1f ni x = %.1f\n'], ...
         x(1), x(end));

% ------------------------------------------------------------
% 7. GRAFICA DE LAS DERIVADAS
% ------------------------------------------------------------

figure;

plot(
    x,
    derivada_adelante,
    'o-',
    'LineWidth', 1.3
);

hold on;

plot(
    x,
    derivada_central,
    's-',
    'LineWidth', 1.3
);

xlabel('x');
ylabel('dy/dx');

title(
    'Diferenciacion numerica de los datos experimentales'
);

legend(
    'Diferencia hacia adelante',
    'Diferencia central',
    'Location',
    'northeast'
);

grid on;
hold off;

clear;
clc;
close all;
 
% TALLER DE INTEGRACIÓN Y DIFERENCIACIÓN NUMERICA
% Ejercicio 1 - Integracion de una funcion
 
% Funcion del problema
f = @(x) exp(-0.4*x) .* (1 + 0.5*sin(3*x));
 
% Intervalo de integracion
a = 0;
b = 8;
 
% Primitiva analitica de la funcion
F = @(x) -2.5*exp(-0.4*x) + ...
    (0.5*exp(-0.4*x)/9.16) .* ...
    (-0.4*sin(3*x) - 3*cos(3*x));
 
% Valor exacto de referencia
I_ref = F(b) - F(a);
 
fprintf('Valor de referencia = %.12f\n\n', I_ref);
 
% Valores de n solicitados
n_valores = [10 20 50 100 500 1000];
 
fprintf('   n        Integral trapecio          Error          Tiempo (s)\n');
fprintf('-------------------------------------------------------------------\n');
 
for n = n_valores
 
    tic;
 
    % Tamano del paso
    h = (b-a)/n;
 
    % Puntos de la malla
    x = linspace(a, b, n+1);
 
    % Evaluacion de la funcion
    y = f(x);
 
    % Regla compuesta del trapecio
    I_trap = h * ( ...
        y(1)/2 + ...
        sum(y(2:end-1)) + ...
        y(end)/2 );
 
    tiempo = toc;
 
    % Error absoluto
    error_abs = abs(I_trap - I_ref);
 
    fprintf('%5d      %.12f      %.3e      %.6e\n', ...
            n, I_trap, error_abs, tiempo);
end

% Segunda aproximacion: cuadratura adaptativa de Octave
fprintf('\nCuadratura adaptativa con quadgk\n');
 
tic;
[I_quad, err_quad] = quadgk(f, a, b);
tiempo_quad = toc;
 
error_real_quad = abs(I_quad - I_ref);
 
fprintf('Integral quadgk      = %.12f\n', I_quad);
fprintf('Error estimado       = %.3e\n', err_quad);
fprintf('Error vs referencia  = %.3e\n', error_real_quad);
fprintf('Tiempo               = %.6e s\n', tiempo_quad);
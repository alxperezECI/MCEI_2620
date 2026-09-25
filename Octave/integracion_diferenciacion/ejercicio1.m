% Taller: Integracion y diferenciacion numerica
% Ejercicio 1 - Integracion de una funcion conocida
% Integrantes: Jimmy Alexander Calderon Trujillo
%              Sebastian Barahona

clear;
clc;

% Funcion del ejercicio
f = @(x) exp(-0.4*x) .* (1 + 0.5*sin(3*x));

% Limites de integracion
a = 0;
b = 8;

% Primitiva analitica
F = @(x) -2.5*exp(-0.4*x) + ...
    (0.5*exp(-0.4*x).*(-0.4*sin(3*x) - 3*cos(3*x))) / ...
    (0.4^2 + 3^2);

% Integral de referencia
I_ref = F(b) - F(a);

fprintf('Valor de referencia de la integral: %.12f\n', I_ref);

% Valores de n solicitados en el taller
valores_n = [10, 20, 50, 100, 500, 1000];

fprintf('\nREGLA COMPUESTA DEL TRAPECIO\n');
fprintf('---------------------------------------------------------------\n');
fprintf('   n          Integral             Error absoluto       Tiempo (s)\n');
fprintf('---------------------------------------------------------------\n');

for n = valores_n

    tic;

    % Tamano del paso
    h = (b - a) / n;

    % Puntos de la particion
    x = linspace(a, b, n + 1);

    % Evaluacion de la funcion
    y = f(x);

    % Regla compuesta del trapecio
    I_trap = h * ( ...
        0.5*y(1) + ...
        sum(y(2:end-1)) + ...
        0.5*y(end));

    tiempo = toc;

    % Error respecto al valor de referencia
    error_abs = abs(I_ref - I_trap);

    fprintf('%4d     %16.12f     %14.6e     %10.6e\n', ...
            n, I_trap, error_abs, tiempo);
end

% Integracion adaptativa con la funcion integral de Octave
fprintf('\nINTEGRACION ADAPTATIVA CON integral()\n');
fprintf('---------------------------------------------------------------\n');

tic;
I_octave = integral(f, a, b);
tiempo_octave = toc;

error_octave = abs(I_ref - I_octave);

fprintf('Integral obtenida : %.12f\n', I_octave);
fprintf('Error absoluto    : %.6e\n', error_octave);
fprintf('Tiempo            : %.6e s\n', tiempo_octave);

% Grafica de la funcion y del area acumulada
x_graf = linspace(a, b, 500);
y_graf = f(x_graf);

figure;

area(x_graf, y_graf);
hold on;
plot(x_graf, y_graf, 'LineWidth', 2);

xlabel('x');
ylabel('f(x)');
title('Ejercicio 1 - Area bajo la curva');
grid on;

legend('Area acumulada', 'f(x)');
clear;

clc;

close all;
 
% Taller de integracion y diferenciacion numerica

% Extension - Diferenciacion numerica
 
% Leer los datos experimentales

datos = dlmread('../Python/datos_sensor.csv', ',', 1, 0);
 
x = datos(:,1);

y = datos(:,2);
 
N = length(x);

h = x(2) - x(1);
 
fprintf('Numero de datos = %d\n', N);

fprintf('Paso h          = %.4f\n\n', h);
 
% --------------------------------------------------

% 1. Diferencia hacia adelante

% --------------------------------------------------
 
dy_adelante = NaN(N,1);
 
for i = 1:N-1

    dy_adelante(i) = (y(i+1) - y(i))/h;

end
 
% El ultimo punto no puede calcularse con

% diferencia hacia adelante porque no existe y(i+1)
 
% --------------------------------------------------

% 2. Diferencia central

% --------------------------------------------------
 
dy_central = NaN(N,1);
 
for i = 2:N-1

    dy_central(i) = (y(i+1) - y(i-1))/(2*h);

end
 
% Los extremos no pueden calcularse directamente

% mediante diferencia central.
 
% --------------------------------------------------

% Mostrar algunos resultados

% --------------------------------------------------
 
fprintf('   x          Adelante          Central\n');

fprintf('--------------------------------------------\n');
 
for i = 1:N

    fprintf('%6.2f     %12.6f     %12.6f\n', ...

            x(i), dy_adelante(i), dy_central(i));

end
 
% --------------------------------------------------

% Grafica

% --------------------------------------------------
 
figure('visible', 'off');
 
plot(x, dy_adelante, 'o-');

hold on;

plot(x, dy_central, 's-');
 
xlabel('x');

ylabel('dy/dx');

title('Diferenciacion numerica');

legend('Diferencia hacia adelante', 'Diferencia central');

grid on;
 
print('diferenciacion_numerica.png', '-dpng');
 
close;
 
fprintf('\nGrafica guardada como diferenciacion_numerica.png\n');
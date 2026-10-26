% Cargar el CSV (ignorando la fila de encabezados)
datos = csvread('datos_sensor.csv', 1, 0);
x = datos(:, 1);
y = datos(:, 2);

n = length(x);
h = x(2) - x(1);

% Inicializar vectores de derivadas
dy_adelante = zeros(n, 1);
dy_central = zeros(n, 1);

% Diferencia hacia adelante:
% f'(x_i) = (f(x_{i+1}) - f(x_i)) / h
dy_adelante(1:n-1) = diff(y) / h;

% Extremo final (diferencia hacia atrás)
dy_adelante(n) = (y(n) - y(n-1)) / h;

% Diferencia central:
% f'(x_i) = (f(x_{i+1}) - f(x_{i-1})) / 2h
dy_central(2:n-1) = (y(3:n) - y(1:n-2)) / (2*h);

% Extremos para diferencia central
dy_central(1) = (y(2) - y(1)) / h;         % Primer dato: diferencia hacia adelante
dy_central(n) = (y(n) - y(n-1)) / h;       % Último dato: diferencia hacia atrás

% Resultados
fprintf('%-5s %12s %12s %15s %15s\n', 'i', 'x', 'y', 'dy (Adelante)', 'dy (Central)');
fprintf('-----------------------------------------------------------------\n');

for i = 1:n
    % Se utiliza (i-1) para que el índice mostrado coincida con C++ (inicia en 0)
    fprintf('%-5d %12.6f %12.6f %15.6f %15.6f\n', ...
            i-1, x(i), y(i), dy_adelante(i), dy_central(i));
end

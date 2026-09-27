% =========================================================
% EJERCICIO 2: INTEGRACIÓN Y DIFERENCIACIÓN (OCTAVE)
% =========================================================

% 1. Cargar datos del CSV
data = csvread('datos_sensor.csv');
x = data(:, 1);
y = data(:, 2);

N = length(x);
h = x(2) - x(1); % Paso de muestreo (h = 0.2)

% ---------------------------------------------------------
% A. DIFERENCIACIÓN NUMÉRICA dy/dx
% ---------------------------------------------------------
dydx = zeros(N, 1);

% Extremo inicial: Diferencias finitas hacia adelante O(h)
dydx(1) = (y(2) - y(1)) / h;

% Puntos intermedios: Diferencias finitas centradas O(h^2)
for i = 2:(N - 1)
    dydx(i) = (y(i + 1) - y(i - 1)) / (2 * h);
end

% Extremo final: Diferencias finitas hacia atrás O(h)
dydx(N) = (y(N) - y(N - 1)) / h;

% ---------------------------------------------------------
% B. INTEGRACIÓN NUMÉRICA (3 MÉTODOS)
% ---------------------------------------------------------
% 1. Regla del Rectángulo (Suma por la izquierda / Punto Izq)
I_rect = h * sum(y(1:N-1));

% 2. Regla del Trapecio Compuesta
I_trap = (h / 2) * (y(1) + 2 * sum(y(2:N-1)) + y(N));

% 3. Regla de Simpson 1/3 Compuesta
suma_impares = sum(y(3:2:N-2)); 
suma_pares = sum(y(2:2:N-1));   
I_simp = (h / 3) * (y(1) + 4 * suma_pares + 2 * suma_impares + y(N));

% 4. Integración nativa para verificación
I_native = trapz(x, y);

% ---------------------------------------------------------
% C. IMPRESIÓN DE RESULTADOS
% ---------------------------------------------------------
fprintf('=========================================================\n');
fprintf('  EJERCICIO 2: PROCESAMIENTO DE DATOS DISCRETOS (OCTAVE)\n');
fprintf('=========================================================\n');
fprintf('  Total de mediciones (N) : %d\n', N);
fprintf('  Paso de muestreo (h)    : %.4f\n', h);
fprintf('---------------------------------------------------------\n');
fprintf('  Integral Rectángulo       : %.8f\n', I_rect);
fprintf('  Integral Trapecio (Manual): %.8f\n', I_trap);
fprintf('  Integral Simpson 1/3      : %.8f\n', I_simp);
fprintf('  Integral trapz (Nativa)   : %.8f\n', I_native);
fprintf('---------------------------------------------------------\n');
fprintf('  Primera derivada dy/dx(0) : %.6f\n', dydx(1));
fprintf('  Derivada central dy/dx(1) : %.6f\n', dydx(2));
fprintf('  Última derivada dy/dx(end): %.6f\n', dydx(N));
fprintf('=========================================================\n');

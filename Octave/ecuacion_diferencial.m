% Parámetros del sistema
m = 20;   % Masa (kg)
k = 20;   % Constante del resorte (N/m)
c_values = [5, 40, 200]; % Coeficientes de amortiguamiento
labels = {'Subamortiguado (c=5)', 'Amortiguamiento critico (c=40)', 'Sobreamortiguado (c=200)'};

% Configuración del método numérico (RK4 de paso fijo)
h = 0.05;              % Tamaño del paso de integración
t = 0:h:15;            % Vector de tiempo de 0 a 15 s
N = length(t);         % Número total de puntos

figure;
hold on;

% Bucle externo para iterar sobre cada valor de amortiguamiento
for j = 1:length(c_values)
    c = c_values(j);

    % Inicialización del vector de estado para el ciclo actual
    x = zeros(2, N);
    x(:, 1) = [1; 0];  % Condiciones iniciales: x=1 m, velocidad=0

    % Sistema de ecuaciones diferenciales
    sistema = @(t, y) [y(2); -(k/m)*y(1) - (c/m)*y(2)];

    % Bucle interno: Método Runge-Kutta de 4to orden manual
    for i = 1:(N - 1)
        y_actual = x(:, i);
        t_actual = t(i);

        % Cálculo de las 4 pendientes
        k1 = sistema(t_actual, y_actual);
        k2 = sistema(t_actual + h/2, y_actual + (h/2)*k1);
        k3 = sistema(t_actual + h/2, y_actual + (h/2)*k2);
        k4 = sistema(t_actual + h, y_actual + h*k3);

        % Cálculo del siguiente estado
        x(:, i+1) = y_actual + (h/6) * (k1 + 2*k2 + 2*k3 + k4);
    end

    % Graficar la fila 1 (desplazamiento) para el valor actual de c
    plot(t, x(1,:), 'LineWidth', 2, 'DisplayName', labels{j});
end

% Configuración de la gráfica
title('Respuesta del sistema usando RK4 Manual');
xlabel('Tiempo t (s)');
ylabel('Desplazamiento x (m)');
yline(0, '--k', 'HandleVisibility', 'off'); % Línea de referencia
legend('show');
grid on;
hold off;

% =========================================================================
% Solución del Sistema Masa-Resorte-Amortiguador en GNU Octave
% =========================================================================

% 1. Parámetros físicos del sistema
m = 20;    % Masa [kg]
k = 20;    % Constante del resorte [N/m]
x0 = 1;    % Desplazamiento inicial [m]
v0 = 0;    % Velocidad inicial [m/s]

tspan = [0, 15];      % Intervalo de tiempo [0, 15] s
y0 = [x0; v0];        % Vector de condiciones iniciales [x(0); v(0)]

% 2. Casos de coeficientes de amortiguamiento c
c_vals = [5, 40, 200];
labels = {
    'c = 5 N s/m (Subamortiguado)', ...
    'c = 40 N s/m (Amortiguamiento Crítico)', ...
    'c = 200 N s/m (Sobreamortiguado)'
};
colors = {'b', 'g', 'r'};

figure;
hold on;

% 3. Integración numérica con ode45 (Runge-Kutta 4/5)
for i = 1:length(c_vals)
    c = c_vals(i);
    
    % Definición del sistema de EDOs de primer orden: [dx/dt; dv/dt]
    f = @(t, y) [y(2); -(c/m)*y(2) - (k/m)*y(1)];
    
    % Resolver la EDO con ode45
    [t, y] = ode45(f, tspan, y0);
    
    % Graficar el desplazamiento x(t) vs t
    plot(t, y(:,1), colors{i}, 'LineWidth', 2, 'DisplayName', labels{i});
end

% 4. Formato de la gráfica
title('Respuesta Temporal del Sistema Masa-Resorte-Amortiguador (0 < t < 15 s)');
xlabel('Tiempo t [s]');
ylabel('Desplazamiento x(t) [m]');
grid on;
legend('show', 'Location', 'northeast');
hold off;

% 5. Guardar la gráfica en un archivo PNG
saveas(gcf, 'grafica_octave_edo.png');
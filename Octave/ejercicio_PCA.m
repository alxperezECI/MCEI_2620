% 1. Definir la matriz X (10x3)
X = [48, 5.1, 2.1;
     50, 5.3, 2.4;
     51, 5.4, 2.5;
     53, 5.7, 2.8;
     55, 5.8, 3.0;
     56, 6.0, 3.1;
     58, 6.2, 3.5;
     60, 6.3, 3.6;
     61, 6.5, 3.8;
     63, 6.7, 4.0];

N = size(X, 1); % Se obtiene la cantidad de muestras en la matriz de datos (debe haber 10)
% Media y desviación estándar, por defecto normaliza con N-1 para aplicar la corrección de Bessel
medias = mean(X); % Saca la media para cada variable de la matriz
desv_est = std(X); % Saca la desvición estandar de cada variable

% Se obtiene la matriz de estandarización Z
Z = (X - medias) ./ desv_est;
disp("Media para T, I y V:"), disp(medias);
disp("Desviación estandar para T, I y V:"), disp(desv_est);
disp("Matriz Z:"), disp(Z);

% Se saca la matriz de covarianza usando la ecuacion (Z^T * Z) / (N-1)
S = (Z' * Z) / (N - 1);
disp("Matriz de covarianza S:"), disp(S);

% Se calculan los autovectores y los autovalores usando
[EV, ED] = eig(S);


% Se extrae la diagonal de ED como vector y ordenar de forma descendente
[autovalores, idx] = sort(diag(ED), 'descend');

% Se reordenan las columnas de la matriz de autovectores EV
autovectores = EV(:, idx);
disp("Autovalores:"), disp(autovalores')
disp("Autovectores (columnas PC1, PC2, PC3):"), disp(autovectores)

% Verificación: Suma de autovalores vs Traza de S
disp(["Suma de autovalores: ", num2str(sum(autovalores))]);
disp(["Traza de S: ", num2str(trace(S))]);

% Ahora se calcula la varianza explicada individual y acumulada
var_explicada = (autovalores / sum(autovalores)) * 100;
var_acumulada = cumsum(var_explicada);

disp("Varianza explicada (%):"), disp(var_explicada');
disp("Varianza acumulada (%):"), disp(var_acumulada');


% Se seleccionan los primeros 2 autovectores (3x2) mayores
W = autovectores(:, 1:2);

% Se realiza la proyección: los datos de Z(10x3) * W(3x2) -> Proyeccion(10x2)
proyeccion = Z * W;

% Graficar e identificar cada muestra
figure;
scatter(proyeccion(:, 1), proyeccion(:, 2), 'filled');
hold on;

for i = 1:N
    text(proyeccion(i, 1) + 0.04, proyeccion(i, 2) + 0.004, sprintf('M%d', i), 'FontSize', 10);
end

line(xlim, [0 0], 'Color', 'k', 'LineStyle', '--');
line([0 0], ylim, 'Color', 'k', 'LineStyle', '--');
xlabel(sprintf('PC1 (%.2f%% varianza)', var_explicada(1)));
ylabel(sprintf('PC2 (%.2f%% varianza)', var_explicada(2)));
title('Proyeccion de observaciones en el plano PC1 - PC2');
grid on;
hold off;

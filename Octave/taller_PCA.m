% Taller de Reducción de Dimensionalidad mediante PCA
% 1. Construir la matriz X (10x3) con los datos del motor
% Columnas: Temperatura (°C), Corriente (A), Vibración (mm/s)
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

fprintf('--- 1. Matriz estandarizada Z ---\n');
% Calcular media y desviación estándar de cada variable
mu = mean(X);
sigma = std(X);

% Obtener matriz Z estandarizada
Z = (X - mu) ./ sigma;
disp('Matriz Z (primeras 5 filas):');
disp(Z(1:5, :));

fprintf('\n--- 2. Matriz de covarianza S ---\n');
% Calcular S, la matriz de covarianza de Z
S = cov(Z);
disp('Matriz S:');
disp(S);

fprintf('\n--- 3. Autovalores y autovectores ---\n');
% Obtener autovalores y autovectores
[V, D] = eig(S);

% Extraer autovalores y ordenarlos de mayor a menor
autovalores = diag(D);
[autovalores_ord, indices] = sort(autovalores, 'descend');

% Ordenar los autovectores de forma correspondiente
autovectores_ord = V(:, indices);

disp('Autovalores ordenados:');
disp(autovalores_ord);

% Verificar que la suma de autovalores coincide con la traza de S
suma_autovalores = sum(autovalores_ord);
traza_S = trace(S);
fprintf('Suma de autovalores: %.4f\n', suma_autovalores);
fprintf('Traza de la matriz S: %.4f\n', traza_S);

fprintf('\n--- 4. Varianza explicada ---\n');
% Varianza explicada individual y acumulada
var_explicada = (autovalores_ord / suma_autovalores) * 100;
var_acumulada = cumsum(var_explicada);

fprintf('Varianza explicada por PC1: %.2f%%\n', var_explicada(1));
fprintf('Varianza explicada por PC1 + PC2: %.2f%%\n', var_acumulada(2));

fprintf('\n--- 5. Proyección sobre PC1 y PC2 ---\n');
% Proyectar las observaciones (Nuevas coordenadas / Scores)
Scores = Z * autovectores_ord;
PC1 = Scores(:, 1);
PC2 = Scores(:, 2);

% Graficar
figure;
scatter(PC1, PC2, 50, 'b', 'filled');
title('Proyección de muestras del motor (PC1 vs PC2)');
xlabel(sprintf('PC1 (%.2f%% Varianza)', var_explicada(1)));
ylabel(sprintf('PC2 (%.2f%% Varianza)', var_explicada(2)));
grid on; hold on;

%% Ajustar límites para dar margen visual y colocar etiquetas al lado derecho de cada punto
ylim([-0.12, 0.12]);
xlim([-3, 3]);
for i = 1:size(X, 1)
    text(PC1(i) + 0.08, PC2(i), num2str(i), 'FontSize', 11, 'FontWeight', 'bold', 'Color', 'k');
end
hold off;

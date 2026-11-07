clc; clear; close all;

% --- Datos (10 muestras x 3 variables: T, I, V) ---
X = [48 5.1 2.1;
     50 5.3 2.4;
     51 5.4 2.5;
     53 5.7 2.8;
     55 5.8 3.0;
     56 6.0 3.1;
     58 6.2 3.5;
     60 6.3 3.6;
     61 6.5 3.8;
     63 6.7 4.0];

% --- Media y desviacion estandar por columna ---
mu    = mean(X);
sigma = std(X);      % muestral (N-1)

% --- Estandarizacion ---
Z = (X - mu) ./ sigma;

% --- Verificacion ---
disp('Medias (T, I, V):');          disp(mu)
disp('Desv. estandar (T, I, V):');  disp(sigma)
disp('Media de Z (debe ser ~0):');  disp(mean(Z))
disp('Std de Z (debe ser 1):');     disp(std(Z))

% --- Paso 2: matriz de covarianza de Z ---
n = size(Z, 1);
S = (Z' * Z) / (n - 1);

disp('Matriz de covarianza S:');
disp(S)
disp('Verificacion con cov(Z):');
disp(cov(Z))

% --- Paso 3: autovalores y autovectores ---
[V, D] = eig(S);
lambda = diag(D);

[lambda, idx] = sort(lambda, 'descend');
V = V(:, idx);

disp('Autovalores ordenados:');
disp(lambda')
disp('Autovectores (columnas = PC1, PC2, PC3):');
disp(V)

disp('Suma de autovalores:');
disp(sum(lambda))
disp('Traza de S:');
disp(trace(S))

% --- Paso 4: varianza explicada ---
var_ind = lambda / sum(lambda) * 100;
var_acu = cumsum(var_ind);

disp('Varianza explicada individual (%):');
disp(var_ind')
disp('Varianza explicada acumulada (%):');
disp(var_acu')

fprintf('\nPC1 conserva: %.2f %%\n', var_ind(1));
fprintf('PC1+PC2 conservan: %.2f %%\n', var_acu(2));

% --- Paso 5: proyeccion y grafico PC1-PC2 ---
scores = Z * V(:, 1:2);

disp('Scores (PC1, PC2) por muestra:');
disp([(1:n)' scores])

figure;
plot(scores(:,1), scores(:,2), 'bo', 'MarkerFaceColor', 'b', 'MarkerSize', 8);
hold on;
for k = 1:n
  text(scores(k,1) + 0.05, scores(k,2) + 0.005, num2str(k));
end
grid on;
xlabel(sprintf('PC1 (%.2f %%)', var_ind(1)));
ylabel(sprintf('PC2 (%.2f %%)', var_ind(2)));
title('Proyeccion de las 10 muestras sobre PC1-PC2');
print('-dpng', '-r150', 'pca_pc1_pc2.png');
input('Presiona Enter para cerrar la figura...', 's');
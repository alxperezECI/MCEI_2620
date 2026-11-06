
clear; clc; close all;


diary('salida_octave.txt');
diary on;

%% Punto 1
T = [48; 50; 51; 53; 55; 56; 58; 60; 61; 63];
I = [5.1; 5.3; 5.4; 5.7; 5.8; 6.0; 6.2; 6.3; 6.5; 6.7];
V = [2.1; 2.4; 2.5; 2.8; 3.0; 3.1; 3.5; 3.6; 3.8; 4.0];

X = [T I V];
disp('=== Matriz X (10x3) ==='); disp(X);

mu = mean(X);
sd = std(X);
disp('=== Medias ===');   disp(mu);
disp('=== Desv. estandar ==='); disp(sd);

Z = (X - mu) ./ sd;
disp('=== Matriz Z ==='); disp(Z);

%% Punto 2
S = cov(Z);
disp('=== Matriz de covarianza S ==='); disp(S);

%% Punto 3
[Vc, D] = eig(S);
lambda = diag(D);
[lambda_ord, idx] = sort(lambda, 'descend');
Vc_ord = Vc(:, idx);

disp('=== Autovalores ordenados ==='); disp(lambda_ord);
disp('=== Traza de S ===');            disp(trace(S));
disp('=== Suma autovalores ===');      disp(sum(lambda_ord));
disp('=== Autovectores (PC1, PC2, PC3 por columnas) ===');
disp(Vc_ord);

%% Punto 4
var_ind  = 100 * lambda_ord / sum(lambda_ord);
var_acum = cumsum(var_ind);

disp('=== Varianza individual (%) ==='); disp(var_ind);
disp('=== Varianza acumulada (%) ===');  disp(var_acum);

%% Punto 5
scores = Z * Vc_ord;
PC1 = scores(:,1);
PC2 = scores(:,2);
disp('=== Puntajes PC1 y PC2 ===');
disp([PC1 PC2]);

% Gráfico de las muestras
figure('visible','off');
scatter(PC1, PC2, 80, 'filled');
hold on;
for k = 1:length(PC1)
    text(PC1(k)+0.05, PC2(k), num2str(k));
end
xlabel('PC1 (99.80% var)');
ylabel('PC2 (0.11% var)');
title('Proyeccion PC1-PC2 - Motor electrico');
grid on;
print('-dpng','-r150','pca_pc1_pc2.png');

%% Punto 6
fprintf('\n=== RECOMENDACION ===\n');
fprintf('PC1 explica %.2f%% de la varianza.\n', var_ind(1));
fprintf('PC1+PC2 explican %.2f%% de la varianza.\n', var_acum(2));
fprintf('Perdida al usar 2 componentes: %.3f%%\n', 100 - var_acum(2));
fprintf('=> Conservar 2 componentes es razonable para visualizacion/monitoreo.\n');

diary off;

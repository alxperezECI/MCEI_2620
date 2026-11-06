% Taller PCA - Metodos Computacionales en Ingenieria
% Autores: Jimmy Alexander Calderon Trujillo y Sebastian Barahona.
% Datos sinteticos suministrados por el profesor.
% Columnas: temperatura (C), corriente (A), vibracion (mm/s).

clear;
close all;
clc;
format long g;

% Localizar el repositorio desde la ubicacion del script.
carpeta = fileparts(mfilename('fullpath'));
raiz = fileparts(fileparts(carpeta));
salida = fullfile(raiz, 'datos', 'pca');
informe = fullfile(raiz, 'Informe', 'pca');
if ~exist(salida, 'dir'), mkdir(salida); end
if ~exist(informe, 'dir'), mkdir(informe); end

% 1. Datos y estandarizacion con desviacion muestral.
X = [
  48  5.1  2.1;
  50  5.3  2.4;
  51  5.4  2.5;
  53  5.7  2.8;
  55  5.8  3.0;
  56  6.0  3.1;
  58  6.2  3.5;
  60  6.3  3.6;
  61  6.5  3.8;
  63  6.7  4.0
];

n = rows(X);
medias = mean(X, 1);
desviaciones = std(X, 0, 1);
Z = (X - medias) ./ desviaciones;

% 2. Covarianza muestral de las variables estandarizadas.
S = (Z' * Z) / (n - 1);

% 3. Autovalores y autovectores ordenados de mayor a menor.
[A, D] = eig(S);
[lambda, orden] = sort(diag(D), 'descend');
A = A(:, orden);

% Fijar el signo para facilitar la comparacion entre ejecuciones.
% Cambiar el signo de un autovector no cambia el resultado del PCA.
for j = 1:columns(A)
  [~, posicion] = max(abs(A(:, j)));
  if A(posicion, j) < 0
    A(:, j) = -A(:, j);
  end
end

% 4. Varianza explicada.
porcentaje = 100 * lambda / sum(lambda);
acumulado = cumsum(porcentaje);

% 5. Proyeccion sobre las dos primeras componentes.
Y = Z * A(:, 1:2);

% Verificaciones numericas.
tol = 1e-10;
assert(max(abs(mean(Z, 1))) < tol);
assert(max(abs(std(Z, 0, 1) - 1)) < tol);
assert(norm(S - cov(Z), 'fro') < tol);
assert(norm(A' * A - eye(3), 'fro') < tol);
assert(norm(S * A - A * diag(lambda), 'fro') < tol);
assert(abs(sum(lambda) - trace(S)) < tol);
assert(norm(cov(Y) - diag(lambda(1:2)), 'fro') < tol);

% Reconstruccion aproximada y comprobacion de varianza descartada.
Z_aprox = Y * A(:, 1:2)';
error_residual = sum(sum((Z - Z_aprox).^2)) / (n - 1);
assert(abs(error_residual - lambda(3)) < tol);

% Identificar pares proximos y alejados en el plano PC1-PC2.
min_dist = Inf;
max_dist = -Inf;
for i = 1:n-1
  for j = i+1:n
    distancia = norm(Y(i, :) - Y(j, :));
    if distancia < min_dist
      min_dist = distancia;
      par_cercano = [i j];
    end
    if distancia > max_dist
      max_dist = distancia;
      par_lejano = [i j];
    end
  end
end

% Mostrar y registrar los resultados.
archivo_log = fullfile(salida, 'resultados_octave.txt');
if exist(archivo_log, 'file'), delete(archivo_log); end
diary(archivo_log);

fprintf('GNU Octave: %s\n', version());
disp('Columnas originales: temperatura, corriente, vibracion');
disp('Matriz X:'); disp(X);
disp('Medias:'); disp(medias);
disp('Desviaciones muestrales:'); disp(desviaciones);
disp('Matriz estandarizada Z:'); disp(Z);
disp('Matriz de covarianza S:'); disp(S);
disp('Autovectores por columnas (PC1, PC2, PC3):'); disp(A);

fprintf('\nPC   Autovalor       Individual(%%)   Acumulada(%%)\n');
for j = 1:3
  fprintf('%d    %.10f    %.6f        %.6f\n', ...
          j, lambda(j), porcentaje(j), acumulado(j));
end

fprintf('\nSuma de autovalores: %.12f\n', sum(lambda));
fprintf('Traza de S: %.12f\n', trace(S));
disp('Proyeccion: muestra, PC1, PC2');
disp([(1:n)' Y]);

fprintf('Par mas cercano: %d y %d; distancia: %.8f\n', ...
        par_cercano(1), par_cercano(2), min_dist);
fprintf('Par mas alejado: %d y %d; distancia: %.8f\n', ...
        par_lejano(1), par_lejano(2), max_dist);
fprintf('Variabilidad conservada con dos PC: %.6f %%\n', acumulado(2));
fprintf('Variabilidad descartada: %.6f %%\n', porcentaje(3));
fprintf('Todas las verificaciones numericas pasaron.\n');

diary off;

% Exportar matrices numericas. El orden de columnas se indica arriba.
dlmwrite(fullfile(salida, 'datos_motor.csv'), X, 'precision', 17);
dlmwrite(fullfile(salida, 'estandarizados.csv'), Z, 'precision', 17);
dlmwrite(fullfile(salida, 'covarianza.csv'), S, 'precision', 17);
dlmwrite(fullfile(salida, 'autovectores.csv'), A, 'precision', 17);
dlmwrite(fullfile(salida, 'varianza.csv'), ...
         [(1:3)' lambda porcentaje acumulado], 'precision', 17);
dlmwrite(fullfile(salida, 'proyeccion.csv'), ...
         [(1:n)' Y], 'precision', 17);

% Grafica: misma escala en ambos ejes para representar las distancias.
fig = figure('visible', 'off');
set(fig, 'position', [100 100 1100 380]);
plot(Y(:, 1), Y(:, 2), 'o', ...
     'markersize', 7, 'markerfacecolor', [0 0.5 0.5], ...
     'markeredgecolor', [0 0.5 0.5]);
hold on;

for i = 1:n
  desplazamiento = 0.10;
  if mod(i, 2) == 0
    desplazamiento = -0.13;
  end
  text(Y(i, 1), Y(i, 2) + desplazamiento, num2str(i), ...
       'horizontalalignment', 'center');
end

xlabel(sprintf('PC1 (%.4f %%)', porcentaje(1)));
ylabel(sprintf('PC2 (%.4f %%)', porcentaje(2)));
title('PCA del motor: diez muestras identificadas');
grid on;
axis equal;
xlim([min(Y(:, 1))-0.3, max(Y(:, 1))+0.3]);
ylim([-0.7 0.7]);

print(fig, fullfile(informe, 'proyeccion_pca.png'), '-dpng', '-r160');
close(fig);
fprintf('\nGrafica guardada en Informe/pca/proyeccion_pca.png\n');

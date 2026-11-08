
1;  % Octave trata el archivo como script (no como funcion) gracias a esta linea

% ============================================================
%  PCA sobre datos de un motor: T (temperatura), I (corriente), V (vibracion)
%  Objetivo: ver si 3 variables correlacionadas se pueden resumir en 2 componentes
% ============================================================

% --- Datos crudos: cada columna es una variable, cada fila una muestra ---
T = [48 50 51 53 55 56 58 60 61 63]';     % temperatura de carcasa (C)
I = [5.1 5.3 5.4 5.7 5.8 6.0 6.2 6.3 6.5 6.7]';   % corriente RMS (A)
V = [2.1 2.4 2.5 2.8 3.0 3.1 3.5 3.6 3.8 4.0]';   % vibracion RMS (mm/s)
X = [T I V];              % matriz 10x3
[n, p] = size(X);         % n = muestras (10), p = variables (3)

% --- PASO 1: estandarizar ---
% Cada variable tiene unidades y escalas distintas (C, A, mm/s). Si no las pongo
% en la misma escala, la de numeros mas grandes (temperatura) dominaria el PCA.
% Estandarizar = restar la media y dividir por la desviacion -> media 0, desv 1.
mu    = mean(X);          % media de cada columna
sigma = std(X);           % desviacion estandar muestral 
Z     = (X - mu) ./ sigma;  % datos estandarizados

printf('Media de cada variable : '); printf('%8.4f', mu);    printf('\n');
printf('Desv. de cada variable : '); printf('%8.4f', sigma); printf('\n');

% --- PASO 2: matriz de covarianza de las variables ya estandarizadas ---
% Como los datos estan estandarizados, esta covarianza es en realidad la matriz
% de CORRELACION: la diagonal vale 1, y cada termino fuera de la diagonal dice
% que tan relacionadas estan dos variables (cerca de 1 = suben juntas).
S = cov(Z);
disp('Matriz S (covarianza de las variables estandarizadas):'); disp(S);
printf('Traza(S) = %.4f   <- suma de la diagonal, debe dar p = 3\n', trace(S));

% --- PASO 3: autovalores y autovectores de S ---
% Los AUTOVECTORES son las direcciones de las nuevas componentes principales.
% Los AUTOVALORES dicen cuanta varianza captura cada una de esas direcciones.
[Vec, D] = eig(S);        % Octave los entrega en orden ASCENDENTE
lambda = diag(D);         % extraigo los autovalores (estan en la diagonal de D)

% Los reordeno de mayor a menor: la 1a componente debe ser la de mas varianza.
[lambda, ord] = sort(lambda, 'descend');
Vec = Vec(:, ord);        % reordeno los autovectores igual que los autovalores

disp('Autovalores (de mayor a menor):'); disp(lambda');
% Verificacion clave del metodo: la suma de autovalores = traza de S.
printf('Suma de autovalores = %.4f   (traza de S = %.4f)\n', sum(lambda), trace(S));
disp('Autovectores (cada columna es una componente principal):'); disp(Vec);

% --- PASO 4: varianza explicada ---
% Cada autovalor, dividido por la suma, es la fraccion de varianza que explica
% esa componente. La acumulada me dice cuanto conservo si me quedo con las
% primeras k componentes.
var_ind  = lambda / sum(lambda) * 100;   % % que explica cada componente
var_acum = cumsum(var_ind);              % % acumulado
printf('Varianza individual (%%): '); printf('%9.4f', var_ind);  printf('\n');
printf('Varianza acumulada  (%%): '); printf('%9.4f', var_acum); printf('\n');
printf('PC1 conserva %.3f%%   |   PC1+PC2 conservan %.3f%%\n', var_ind(1), var_acum(2));

% --- PASO 5: proyectar las muestras sobre las dos primeras componentes ---
% Multiplico los datos estandarizados por los autovectores para obtener las
% coordenadas (scores) de cada muestra en el nuevo sistema PC1-PC2.
Y   = Z * Vec;            % matriz de scores (10x3)
PC1 = Y(:,1);             % coordenada en la primera componente
PC2 = Y(:,2);             % coordenada en la segunda componente

figure('visible','off');
scatter(PC1, PC2, 60, 'filled'); hold on;
for i = 1:n
  text(PC1(i)+0.05, PC2(i), sprintf('m%d', i));  % etiqueto cada muestra
end
xlabel('PC1'); ylabel('PC2'); grid on;
title('Proyeccion de las 10 muestras sobre PC1-PC2');
print('pca_proyeccion.png', '-dpng');
printf('Grafica guardada: pca_proyeccion.png\n');
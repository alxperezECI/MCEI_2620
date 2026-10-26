% 1. Definir los puntos en x y la función y = f(x)
ref = 2.55982;
a = 0;
b = 8;
n = 1001;
h = (b - a) / n; % Tamaño de paso


% 2. Definir la función a integrar
y = @(x) exp(-0.4.*x).*(1 + 0.5.*sin(3.*x));

% 3. Crear el vector de puntos interiores (x1, x2, ..., x_{n-1})
x_interiores = a+h : h : b-h;
% 4. Aplicar la ecuación de la regla compuesta del trapecio y se mide el tiempo de calculo
tic;
I_aprox = h * ( (f(a) + f(b))/2 + sum(f(x_interiores)) );
tiempo = toc;

% 5. Calcular error absoluto y relativo
error_absoluto = abs(I_aprox - ref);
error_relativo = abs((I_aprox - ref) / ref) * 100;

% 6. Impresión de resultados
fprintf('Para n = %d:\n', n);
fprintf('Integral por regla compuesta (manual): %f\n', I_aprox);
fprintf('Error absoluto: %e\n', error_absoluto);
fprintf('Error relativo: %f %%\n', error_relativo);
fprintf('Tiempo: %f segundos\n', tiempo);

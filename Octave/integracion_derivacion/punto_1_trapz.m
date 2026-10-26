% 1. Definir los puntos en x y la función y = f(x)
ref = 2.55982;
n = 1001;

x = linspace(0, 8, n);  % 100 puntos entre 0 y 8

% 2. Definir la función a integrar
y = @(x) exp(-0.4.*x).*(1 + 0.5.*sin(3.*x));

% 3. Calcular la integral con trapz y se mide el tiempo que tarda integrando
tic;
resultado = trapz(x, y(x))
tiempo = toc;

% 4. Calcular el error relativo y absoluto entre el valor de referencia y el resultado de Octave
error_absoluto = abs(resultado - ref);
error_relativo = abs((resultado - ref) / ref) * 100;

% 4. Mostrar resultados
fprintf('Para n = %d:\n', n);
fprintf('Integral aproximada: %f\n', resultado);
fprintf('Error absoluto: %e\n', error_absoluto);
fprintf('Error relativo: %f %%\n', error_relativo);
fprintf('Tiempo: %f segundos\n', tiempo);

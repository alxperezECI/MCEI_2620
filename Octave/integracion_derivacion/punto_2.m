% Cargar el CSV (ignorando la primera fila de encabezados)
datos = csvread('datos_sensor.csv', 1, 0);
x = datos(:, 1);
y = datos(:, 2);

% Verificar paso de discretización (h) y cantidad de datos
h = x(2) - x(1);
N = length(x);

% Regla del Trapecio:

% Se usa la función trapz
I_trap = trapz(x, y);
fprintf('Integral con Trapecio: %.6f\n', I_trap);

% Regla de Simpson:
% Debido a que hay 50 datos (49 intervalos), se aplica Simpson 1/3
% a los primeros 49 datos (48 intervalos) y Trapecio al último
I_simp = 0;

% Aplicar Simpson 1/3 (índices en Octave empiezan en 1)
for i = 1:2:(N-2)
    I_simp = I_simp + (h/3) * (y(i) + 4*y(i+1) + y(i+2));
end

% Sumar el último intervalo con la regla del trapecio
I_simp = I_simp + (h/2) * (y(N-1) + y(N));
fprintf('Integral con Simpson (Híbrido 1/3 + Trapecio final): %.6f\n', I_simp);

% Graficar los datos y el área aproximada
figure;
hold on;

% Dibujar el área bajo la curva
area(x, y, 'FaceColor', [0.8 0.9 1], 'EdgeColor', 'none');

% Graficar los datos medidos encima del área
plot(x, y, 'b.-', 'MarkerSize', 15, 'LineWidth', 1.5);

title('Integración de Mediciones Discretas');
xlabel('Tiempo (s)');
ylabel('Medición del sensor y(x)');
legend('Área acumulada', 'Datos medidos');
grid on;
hold off;

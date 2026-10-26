% Cargar los datos del archivo CSV
% El archivo tiene encabezados (t, x, y) en la primera fila,
% se usa csvread y se omite la primera fila (índice 1) y columna 0
tic;

data = csvread('trayectoria_robot.csv', 1, 0);
t = data(:, 1);
x = data(:, 2);
y = data(:, 3);



% Calcular el paso de tiempo h
h = t(2) - t(1);
N = length(t);

% Inicializar vectores para las derivadas
vx = zeros(N, 1);
vy = zeros(N, 1);

% Calcular x_dot e y_dot con diferencias centrales
% Se itera sobre los puntos interiores
for i = 2:N-1
    vx(i) = (x(i+1) - x(i-1)) / (2*h);
    vy(i) = (y(i+1) - y(i-1)) / (2*h);
end

% Aproximación hacia adelante y hacia atrás para los extremos
vx(1) = (x(2) - x(1)) / h;
vy(1) = (y(2) - y(1)) / h;
vx(N) = (x(N) - x(N-1)) / h;
vy(N) = (y(N) - y(N-1)) / h;

% Calcular velocidad lineal (v) y orientación (theta)
v = sqrt(vx.^2 + vy.^2);
theta = atan2(vy, vx);

% Desenvolvimiento angular (unwrap) antes de calcular w para evitar saltos de 2*pi
theta_unwrapped = unwrap(theta);

% Calcular velocidad angular (w) usando diferencias centrales
w = zeros(N, 1);
for i = 2:N-1
    w(i) = (theta_unwrapped(i+1) - theta_unwrapped(i-1)) / (2*h);
end

% Aproximación de los extremos para w
w(1) = (theta_unwrapped(2) - theta_unwrapped(1)) / h;
w(N) = (theta_unwrapped(N) - theta_unwrapped(N-1)) / h;

tiempo_ejecucion = toc;
fprintf('Tiempo de los calculos: %f segundos\n', tiempo_ejecucion);

% Generar las gráficas en una sola ventana de 2x2
figure(1);

% Gráfica de trayectoria (y vs x)
subplot(2,2,1);
plot(x, y, 'b-', 'LineWidth', 1.5);
title('Trayectoria del robot');
xlabel('x [m]'); ylabel('y [m]');
grid on;

% Gráfica de velocidad lineal (v vs t)
subplot(2,2,2);
plot(t, v, 'r-', 'LineWidth', 1.5);
title('Velocidad lineal');
xlabel('t [s]'); ylabel('v [m/s]');
grid on;

% Gráfica de orientación (theta vs t)
subplot(2,2,3);
plot(t, theta_unwrapped, 'g-', 'LineWidth', 1.5);
title('Orientacion (\theta)');
xlabel('t [s]'); ylabel('\theta [rad]');
grid on;

% Gráfica de velocidad angular (w vs t)
subplot(2,2,4);
plot(t, w, 'm-', 'LineWidth', 1.5);
title('Velocidad angular (\omega)');
xlabel('t [s]'); ylabel('\omega [rad/s]');
grid on;


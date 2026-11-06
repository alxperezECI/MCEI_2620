if ~exist('resultados', 'dir')
    mkdir('resultados');
end

data = dlmread('trayectoria_robot.csv', ',', 1, 0);
t = data(:,1);
x = data(:,2);
y = data(:,3);
N = length(t);

vx = zeros(N,1);
vy = zeros(N,1);

vx(2:N-1) = (x(3:N) - x(1:N-2)) ./ (t(3:N) - t(1:N-2));
vy(2:N-1) = (y(3:N) - y(1:N-2)) ./ (t(3:N) - t(1:N-2));

vx(1) = (x(2) - x(1)) / (t(2) - t(1));
vx(N) = (x(N) - x(N-1)) / (t(N) - t(N-1));
vy(1) = (y(2) - y(1)) / (t(2) - t(1));
vy(N) = (y(N) - y(N-1)) / (t(N) - t(N-1));

v = sqrt(vx.^2 + vy.^2);

theta = atan2(vy, vx);
theta_u = unwrap(theta);

omega = zeros(N,1);
omega(2:N-1) = (theta_u(3:N) - theta_u(1:N-2)) ./ (t(3:N) - t(1:N-2));
omega(1) = (theta_u(2) - theta_u(1)) / (t(2) - t(1));
omega(N) = (theta_u(N) - theta_u(N-1)) / (t(N) - t(N-1));

set(0, 'defaultfigurevisible', 'off');

figure;
plot(x, y, 'b-');
axis equal;
xlabel('x [m]'); ylabel('y [m]');
title('Trayectoria del robot');
grid on;
print('resultados/trayectoria_octave.png', '-dpng', '-r150');

figure;
plot(t, v, 'r-');
xlabel('t [s]'); ylabel('v [m/s]');
title('Velocidad lineal');
grid on;
print('resultados/v_octave.png', '-dpng', '-r150');

figure;
plot(t, theta_u, 'g-');
xlabel('t [s]'); ylabel('theta [rad]');
title('Orientacion (unwrap)');
grid on;
print('resultados/theta_octave.png', '-dpng', '-r150');

figure;
plot(t, omega, 'm-');
xlabel('t [s]'); ylabel('omega [rad/s]');
title('Velocidad angular');
grid on;
print('resultados/omega_octave.png', '-dpng', '-r150');

printf('v: min = %f, max = %f\n', min(v), max(v));
printf('omega: min = %f, max = %f\n', min(omega), max(omega));
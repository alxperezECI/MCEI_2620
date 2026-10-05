%SERGIO ANDRES MARIN PATIÑO 03/10/2026

graphics_toolkit('gnuplot');                 % robusto en WSL sin servidor X

data = dlmread('trayectoria_robot.csv', ',', 1, 0);   % salta encabezado
t = data(:,1); 
x = data(:,2); 
y = data(:,3);

n = numel(t); 
h = t(2) - t(1);

vx = zeros(n,1); 
vy = zeros(n,1);

vx(2:n-1) = (x(3:n) - x(1:n-2)) / (2*h);
vy(2:n-1) = (y(3:n) - y(1:n-2)) / (2*h);

vx(1) = (x(2)-x(1))/h;  
vx(n) = (x(n)-x(n-1))/h;

vy(1) = (y(2)-y(1))/h;  
vy(n) = (y(n)-y(n-1))/h;

v     = sqrt(vx.^2 + vy.^2);
theta = unwrap(atan2(vy, vx));               % unwrap ANTES de derivar
omega = zeros(n,1);
omega(2:n-1) = (theta(3:n) - theta(1:n-2)) / (2*h);
omega(1) = (theta(2)-theta(1))/h;  
omega(n) = (theta(n)-theta(n-1))/h;

f=figure('visible','off'); 
plot(x,y,'-o'); 
axis equal;
xlabel('x [m]'); 
ylabel('y [m]'); 
title('Trayectoria'); 
print(f,'fig_trayectoria.png','-dpng');

f=figure('visible','off'); 
plot(t,v,'-'); 
xlabel('t [s]'); 
ylabel('v [m/s]'); 
title('v(t)'); 
print(f,'fig_v.png','-dpng');

f=figure('visible','off'); 
plot(t,theta,'-'); 
xlabel('t [s]'); 
ylabel('\theta [rad]'); 
title('\theta(t)'); 
print(f,'fig_theta.png','-dpng');

f=figure('visible','off'); 
plot(t,omega,'-'); 
xlabel('t [s]'); 
ylabel('\omega [rad/s]'); 
title('\omega(t)'); 
print(f,'fig_omega.png','-dpng');

printf('Octave: v(0)=%.6f v(25)=%.6f v(50)=%.6f\n', v(1), v(26), v(51));
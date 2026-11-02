data = csvread('datos_convertidor_realista.csv', 1, 0);
V = data(:,1); I = data(:,2); T = data(:,3); P = data(:,4);
n = length(P);

X = [ones(n,1), V, I, T];
y = P;
beta = X \ y;

printf('=== Coeficientes Octave ===\n');
printf('beta0 = %.6f\n', beta(1));
printf('beta1 (V) = %.6f\n', beta(2));
printf('beta2 (I) = %.6f\n', beta(3));
printf('beta3 (T) = %.6f\n', beta(4));

yhat = X * beta;
res = y - yhat;
SSE = res' * res;
MSE = SSE / n;
printf('\nMSE = %.6f\n', MSE);
printf('RMSE = %.6f\n', sqrt(MSE));
printf('cond(X) = %.6e\n', cond(X));

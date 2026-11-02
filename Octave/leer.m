A = [
  2, 1, 0, 3, 2, 1, 0, 2, 1, 4;
  1, 3, 2, 0, 1, 4, 2, 1, 0, 2;
  0, 2, 4, 1, 3, 0, 1, 2, 4, 1;
  3, 0, 1, 5, 2, 1, 3, 0, 2, 1;
  2, 1, 3, 2, 6, 2, 1, 4, 0, 3;
  1, 4, 0, 1, 2, 5, 2, 1, 3, 0;
  0, 2, 1, 3, 1, 2, 4, 0, 2, 1;
  2, 1, 2, 0, 4, 1, 0, 5, 3, 2;
  1, 0, 4, 2, 0, 3, 2, 3, 6, 1;
  4, 2, 1, 1, 3, 0, 1, 2, 1, 5
];

I = eye(10);

tic;
A_inv_dir = inv(A);
time_dir = toc * 1e6;
res_dir = norm(A * A_inv_dir - I, 'fro');

tic;
[Q, R] = qr(A);
A_inv_qr = R \ (Q' * I);
time_qr = toc * 1e6;
res_qr = norm(A * A_inv_qr - I, 'fro');

tic;
[U, S, V] = svd(A);
A_inv_svd = V * (diag(1 ./ diag(S))) * U';
time_svd = toc * 1e6;
res_svd = norm(A * A_inv_svd - I, 'fro');

disp('==========================================================================');
disp('           COMPARACION Y ESTABILIDAD NUMERICA (Octave)                    ');
disp('==========================================================================');
disp('Metodo        Tiempo (us)    Residual ||A*A_inv - I||    Estabilidad');
disp('--------------------------------------------------------------------------');
fprintf('1. Directo    %.2f           %.2e                 Moderada\n', time_dir, res_dir);
fprintf('2. QR         %.2f           %.2e                 Alta\n', time_qr, res_qr);
fprintf('3. SVD        %.2f           %.2e                 Maxima\n', time_svd, res_svd);
disp('==========================================================================');
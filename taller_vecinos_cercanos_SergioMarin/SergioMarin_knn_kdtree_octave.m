1;  % marca el archivo como script con funciones locales

function main()
  data = dlmread('Datos_MCEI.csv', ';', 1, 0);   % salta encabezado
  m = data(:,1);
  X = data(:,2:4);                               % voltaje, corriente, potencia
  n = rows(X);

  mu = mean(X); sd = std(X, 1);                  % std poblacional (como ddof=0)
  Xz = (X - mu) ./ sd;

  zv = (X(:,1)-mu(1)) / sd(1);
  [~, a] = max(abs(zv));                         % ancla = anomalia de voltaje
  k = 5; q = Xz(a,:);

  % --- KNN fuerza bruta ---
  d = sqrt(sum((Xz - q).^2, 2));
  [~, order] = sort(d);
  order(order==a) = [];
  nb_bruto = order(1:k);

  % --- kd-tree manual ---
  global BEST_D BEST_I
  BEST_D = inf(k+1,1); BEST_I = zeros(k+1,1);
  root = kd_build(Xz, (1:n)', 0);
  kd_search(root, q, Xz);
  [~, s] = sort(BEST_D); bi = BEST_I(s);
  bi(bi==a) = [];
  nb_kd = bi(1:k);

  printf('Ancla: muestra %d  V=%d I=%d P=%d\n', m(a), X(a,1), X(a,2), X(a,3));
  printf('KNN fuerza bruta -> muestras: '); printf('%d ', m(nb_bruto)); printf('\n');
  printf('kd-tree          -> muestras: '); printf('%d ', m(nb_kd));    printf('\n');
  printf('Vecinos identicos: %d\n', isequal(sort(m(nb_bruto)), sort(m(nb_kd))));

  R=2000; t=tic;
  for r=1:R, dd=sqrt(sum((Xz-q).^2,2)); [~,~]=sort(dd); end
  printf('Tiempo fuerza bruta: %.1f us/consulta\n', toc(t)/R*1e6);

  figure('visible','off');
  scatter3(Xz(:,1),Xz(:,2),Xz(:,3),25,'b','filled'); hold on;
  scatter3(Xz(a,1),Xz(a,2),Xz(a,3),90,'r','filled');
  scatter3(Xz(nb_bruto,1),Xz(nb_bruto,2),Xz(nb_bruto,3),60,'g','filled');
  xlabel('voltaje (z)'); ylabel('corriente (z)'); zlabel('potencia (z)');
  title('Ancla (rojo) y 5 vecinos (verde)');
  print('oct_knn_3d.png','-dpng');
endfunction

function node = kd_build(P, idx, depth)
  if isempty(idx), node = []; return; endif
  ax = mod(depth, columns(P)) + 1;
  [~, s] = sort(P(idx, ax)); idx = idx(s);
  mid = floor(numel(idx)/2) + 1;
  node.i = idx(mid); node.ax = ax;
  node.L = kd_build(P, idx(1:mid-1),   depth+1);
  node.R = kd_build(P, idx(mid+1:end), depth+1);
endfunction

function kd_search(node, q, P)
  global BEST_D BEST_I
  if isempty(node), return; endif
  i = node.i; ax = node.ax;
  d2 = sum((P(i,:) - q).^2);
  [mx, pos] = max(BEST_D);
  if d2 < mx, BEST_D(pos) = d2; BEST_I(pos) = i; endif
  diff = q(ax) - P(i, ax);
  if diff < 0, near=node.L; far=node.R; else near=node.R; far=node.L; endif
  kd_search(near, q, P);
  if diff^2 < max(BEST_D), kd_search(far, q, P); endif
endfunction

main();
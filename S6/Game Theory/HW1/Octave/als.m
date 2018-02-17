function [Ps, Qs] = als(R, k, lambda, iteration_count)
  n = rows(R);
  m = columns(R);
  
  P = rand(k, n);
  Q = rand(k, m); 
     
  filter = R;
  filter(filter != 0) = 1;
  
  for _ = 1:iteration_count
    V = Q*R';
    for u = 1:n
      M = Q*diag(filter(u, :))*Q' + lambda*eye(k);      
      P(:, u) = M \ V(:, u);
    endfor
    
    V = P*R;
    for i = 1:m
      M = P*diag(filter(:, i))*P' + lambda*eye(k);      
      Q(:, i) = M \ V(:, i);
    endfor
    
    Ps(end + 1, :, :) = P;
    Qs(end + 1, :, :) = Q;
  endfor
endfunction



function [fun, RMSE] = tests(R, P, Q, lambda=0.01)
  pq = P'*Q;
  pq(R == 0) = 0;
  res = norm(R - pq, "fro")^2;
  
  fun = 0.5*(res + lambda*(norm(P, "fro")^2 + norm(Q, "fro")^2));
  RMSE = (res / nnz(R))^0.5;  
endfunction
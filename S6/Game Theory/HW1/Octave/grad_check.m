function error_mean = grad_check(fun, dfun, x, varargin)
  # fun(x) - function for which gradient is checked
  # dfun(x, dx) - derivative of fun(x)
  # x - the point at which gradient is checked 
  
  eps = 1e-4;
  tests_count = 100;
  
  error_sum = 0;
  for k = 1:tests_count
    dx = eps*rand(size(x));    
    difference = fun(x + dx, varargin{:}) - fun(x, varargin{:});
    
    error = norm(difference - dfun(x, dx, varargin{:}), "fro") / norm(difference, "fro");
    error_sum += error;
  endfor
  
  error_mean = error_sum / tests_count;
endfunction
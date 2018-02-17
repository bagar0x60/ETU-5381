n = 3;
# 4.2 -------------------------------- 
function res = f(x, A, B, C)
  res = exp(A*exp(B*exp(C*x)));
endfunction

function res = df(x, dx, A, B, C)
  res = diag(exp(A*exp(B*exp(C*x))))*...
    A*diag(exp(B*exp(C*x)))*B*diag(exp(C*x))*C*dx;
endfunction 

x = 0.01*rand(n, 1);
A = rand(n);
B = rand(n);
C = rand(n);
grad_check(@f, @df, x, A, B, C)  # ans = 5.2212e-04

# 4.4 -------------------------------- 
function res = f(x, A, B)
  res = trace(A*inv(x)*B);
endfunction

function res = df(x, dx, A, B)
  res = trace(-inv(x)*B*A*inv(x)*dx);
endfunction 

x = rand(n);
A = rand(n);
B = rand(n);
grad_check(@f, @df, x, A, B)  # ans = 3.1501e-04

# 4.6 --------------------------------

function res = f(x, A, B)
  res = trace([x, B; B', A]);
endfunction

function res = df(x, dx)
  res = trace(dx);
endfunction 

x = rand(n);
A = rand(n);
B = rand(n);
grad_check(@f, @df, x, A, B) # ans = 3.2477e-12
# 4.8 --------------------------------

function res = f(x, A)
  res = det(x'*A*x);
endfunction

function res = df(x, dx, A)
  res = 2*det(x)^2*det(A)*trace(inv(x)*dx);
endfunction 

x = rand(n);
A = rand(n);
grad_check(@f, @df, x, A) # ans = 2.2538e-04

# 4.10 --------------------------------

function res = f(x, p)
  res = log(det(x^p));
endfunction

function res = df(x, dx, p)
  res = p*trace(inv(x)*dx);
endfunction 

x = rand(n);
p = floor(20*rand());
grad_check(@f, @df, x, p) # ans = 4.2008e-04

# 4.12 --------------------------------

function res = f(x, A, B)
  res = 0.5*norm(A*x - B, "fro")^2;
endfunction

function res = df(x, dx, A, B)
  res = trace((A*x - B)'*A*dx); 
endfunction

x = rand(n);
A = rand(n);
B = rand(n);
grad_check(@f, @df, x, A, B) # ans = 1.7692e-04

# 4.14 --------------------------------

function res = f(x, A, B)
  o = ones(1, rows(x)); # 1'
  res = log(o*exp(A'*x));
endfunction

function res = df(x, dx, A, B)
  o = ones(1, rows(x)); # 1'
  grad = A*exp(A'*x) / (o*exp(A'*x));
  res = grad'*dx;
endfunction

x = rand(n, 1);
A = rand(n);
B = rand(n);
grad_check(@f, @df, x, A, B) # ans = 2.8365e-06

# 4.16 --------------------------------

function res = f(x, A, C)
  res = trace(inv(x'*C*x)*A);
endfunction

function res = df(x, dx, A, C)
  res = -2*trace(inv(x'*C*x)*A*inv(x)*dx);
endfunction

x = rand(n);
A = rand(n); A = A*A';  # making symmetric
C = rand(n); C = C*C';  #
grad_check(@f, @df, x, A, C) # ans = 1.7692e-04
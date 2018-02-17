function main(R, R_test)
  lambda = 0.1;
  k = 15;
  iteration_count = 200; 
  
  [Ps, Qs] = als(R, k, lambda, iteration_count);
  
  for i = 1:iteration_count
    P(:, :) = Ps(i, :, :); Q(:, :) = Qs(i, :, :);
    
    [plot_R_fun(end + 1), plot_R_RMSE(end + 1)] = tests(R, P, Q, lambda);
    [plot_R_test_fun(end + 1), plot_R_test_RMSE(end + 1)] = tests(R_test, P, Q, lambda);   
  endfor
  
  draw_plots(plot_R_fun, plot_R_RMSE, plot_R_test_fun, plot_R_test_RMSE);
endfunction

function draw_plots(R_fun, R_RMSE, R_test_fun, R_test_RMSE)
  subplot (2, 2, 1);
  plot (R_fun);
  xlabel("iteration");
  ylabel("R function")
  
  subplot (2, 2, 3)
  plot (R_RMSE);
  xlabel("iteration");
  ylabel("R RMSE")
  
  subplot (2, 2, 2);
  plot (R_test_fun);
  xlabel("iteration");
  ylabel("R test function")
  
  subplot (2, 2, 4);
  plot (R_test_RMSE);
  xlabel("iteration");
  ylabel("R test RMSE");
endfunction

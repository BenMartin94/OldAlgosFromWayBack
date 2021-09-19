function [root, error, roots, errors] = bisectionmethod(f, lower, upper, errorTol, maxIter)
  iter = 0;
  error = inf;
  root = 0;
  roots = zeros(1, maxIter);
  errors = zeros(1, maxIter);
  while(iter<maxIter && error>errorTol)
    xr = (lower+upper)/2;
    if(f(lower)*f(xr)<0);
      upper = xr;
    elseif(f(xr)*f(upper)<0);
      lower = xr;
    else
      error = abs((xr - root)/xr)*100;
      root = xr;
      iter = iter+1;
      roots(iter) = root;
      errors(iter) = error;
      return
    endif
    error = abs((xr - root)/xr)*100;
    root = xr;
    iter = iter+1;
    roots(iter) = root;
    errors(iter) = error;
    
  endwhile
endfunction

function root = bisectionmethod(f, lower, upper, errorTol, maxIter)
  iter = 0;
  error = inf;
  root = 0;
  while(iter<maxIter && error>errorTol)
    xr = (lower+upper)/2;
    if(f(lower)*f(xr)<0);
      upper = xr;
    elseif(f(xr)*f(upper));
      lower = xr;
    endif
    error = abs((xr - root)/xr)*100;
    root = xr;
    iter = iter+1;
    
  endwhile
endfunction

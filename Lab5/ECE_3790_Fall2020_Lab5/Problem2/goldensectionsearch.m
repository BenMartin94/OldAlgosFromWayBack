function [max, error] = goldensectionsearch(f, lower, upper, errorTol, maxIter)
  R = (sqrt(5)-1)/2;
  iter = 0;
  error = inf;
  max = -inf;
  d = R*(upper-lower);
  x1 = lower+d;
  x2 = upper-d;
  fx1 = f(x1);
  fx2 = f(x2);

  while(iter<maxIter && error>errorTol);
    
    if(fx1>fx2)
      lower = x2;
      error = abs((x1 - max)/x1)*100;
      max = x1;
      d = R*(upper-lower);
      x2 = x1;
      x1 = lower+d;
      fx2 = fx1;
      fx1 = f(x1);
    elseif(fx1<fx2)
      upper = x1;
      error = abs((x2 - max)/x2)*100;
      max = x2;
      d = R*(upper-lower);
      x1 = x2
      x2 = upper-d;
      fx1 = fx2;
      fx2 = f(x2);
    endif
    iter = iter+1;
  endwhile
endfunction

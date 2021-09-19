function [max, error, estimates, errors] = goldensectionsearch(f, lower, upper, errorTol, maxIter)
  R = (sqrt(5)-1)/2;
  iter = 0;
  error = inf;
  max = -inf;
  d = R*(upper-lower);
  x1 = lower+d;
  x2 = upper-d;
  fx1 = f(x1);
  fx2 = f(x2);
  estimates = zeros(1, maxIter);
  errors = zeros(1, maxIter);
  while(iter<maxIter && error>errorTol);
    
    if(fx1>fx2)
      lower = x2;
      
      max = x1;
      d = R*(upper-lower);
      x2 = x1;
      x1 = lower+d;
      fx2 = fx1;
      fx1 = f(x1);
    elseif(fx1<fx2)
      upper = x1;
      
      max = x2;
      d = R*(upper-lower);
      x1 = x2;
      x2 = upper-d;
      fx1 = fx2;
      fx2 = f(x2);
    endif
    iter = iter+1;
    estimates(iter) = max;
    errors(iter) = (1-R)*abs((upper)-lower)/max*100;
  endwhile
endfunction

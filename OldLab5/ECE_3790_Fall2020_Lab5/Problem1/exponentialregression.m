function [coeffs, r2] = exponentialregression(data)
  n = length(data);
  x = 1:n;
  u = x;
  v = log(data);
  dataForLin = zeros(n, 3);
  dataForLin(:,1) = ones(n,1);
  dataForLin(:,2) = u;
  dataForLin(:,3) = v;
  [coeffs, r2] = linearregression(dataForLin);
  coeffs(1) = exp(coeffs(1));
endfunction

function [coeffs, r2] = polynomialregression(data, order)
  coeffs = zeros(order+1, 1);
  n = length(data);
  w = zeros(n, order+1);
  x = 1:n;
  for i = 1:n
    for j=0:order
      w(i, j+1) = x(i).^j;
    endfor
  endfor
  dataForLin = zeros(n, order+2);
  dataForLin(:, 1:order+1) = w;
  dataForLin(:, order+2) = data;
  [coeffs, r2] = linearregression(dataForLin);
  
endfunction

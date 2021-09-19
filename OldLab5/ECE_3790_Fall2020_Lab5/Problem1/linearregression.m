function [x, r2] = linearregression(sampleData)
  A = sampleData(:, 1:size(sampleData)(2)-1);
  b = sampleData(:, size(sampleData)(2));
  x = (A'*A)\A'*b;
  avg = mean(b);
  St = sum((b-avg).^2);
  temp = 0;
  for i=1:length(x)
    temp = temp+(x(i)*sampleData(:, i));
  endfor
  Sr = sum((b-(temp)).^2);
  r2 = (St-Sr)./St;
endfunction


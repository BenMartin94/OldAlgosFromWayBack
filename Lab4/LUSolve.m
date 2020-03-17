function x = LUSolve(A, b)
  d = zeros(length(A), 1);
  for i = 1:length(A)
    d(i,1) = b(i,1);
    d(i,1) = d(i,1) - A(i,1:i-1)*d(1:i-1,1);
  endfor
  x = zeros(length(A), 1);
  for i = length(A):-1:1;
    x(i,1)=d(i,1);
    x(i,1) = x(i,1)-A(i,i+1:length(A))*x(i+1:length(x),1);
    x(i,1) = x(i,1)/A(i,i);
  endfor
endfunction

function x = LUSolve(A, b)
  d = zeros(length(A), 1);
  for i = 1:length(A)
    d(i,1) = b(i,1);
    for j = 1:i-1;
      d(i,1) = d(i,1)-A(i,j)*d(j);
    endfor
    
  endfor
  x = zeros(length(A), 1);
  for i = length(A):-1:1;
    x(i,1)=d(i,1);
    for j = length(A):-1:i+1
      x(i,1) = x(i,1)-A(i,j)*x(j,1);
    endfor
    x(i,1) = x(i,1)/A(i,i);
  endfor
endfunction

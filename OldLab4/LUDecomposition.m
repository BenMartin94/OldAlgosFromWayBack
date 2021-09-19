function A = LUDecomposition(A)
  for j = 1:length(A)-1;
    if(A(j,j)==0)
      return
    endif
    for i = j+1:length(A);
      l = A(i,j)/A(j,j);
      A(i,j)=l;
      A(i,j+1:length(A)) = A(i, j+1:length(A)) - l*A(j, j+1:length(A));
    endfor
  endfor

endfunction

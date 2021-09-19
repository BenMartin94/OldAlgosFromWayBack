#### WRITTEN IN OCTAVE, Minor adjustments required if to be run in matlab
# Prof. Jefferies said it was not required to do so.
function C = MM(A,B)
  [Ar, Ac] = size(A);
  [Br, Bc] = size(B);
  M1 = rand(Ar, Ac);
  M2 = rand(Br, Bc);
  C = zeros(Ar, Bc);
  if(size(A)(2)!=size(B)(1))
    toRet = -1
    return
  endif
  for row = 1:Ar
    for col = 1:Bc
      sum = 0;
      for i = 1:Ac
        sum += A(row, i)*B(i, col);
      endfor
      C(row, col) = sum;
    endfor
  endfor 
endfunction

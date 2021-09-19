#### WRITTEN IN OCTAVE, Minor adjustments required if to be run in matlab
# Prof. Jefferies said it was not required to do so.
function A = insertionsort(A)
  for i = 2:length(A)
    key = A(i);
    j = i-1;
    while j>0 && A(j)>key
      A(j+1) = A(j);
      j = j-1;
    endwhile
    A(j+1) = key;
  endfor
  return
endfunction

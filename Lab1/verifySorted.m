#### WRITTEN IN OCTAVE, Minor adjustments required if to be run in matlab
# Prof. Jefferies said it was not required to do so.
function bool = verifySorted(A)
  bool = 1
  for i = 2:length(A)
    if(A(i)<A(i-1))
      bool = 0
      return
    endif
  endfor
endfunction

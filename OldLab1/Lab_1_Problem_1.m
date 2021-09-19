#### WRITTEN IN OCTAVE, Minor adjustments required if to be run in matlab
# Prof. Jefferies said it was not required to do so.


n = input("Enter size of matricies\n");
A = rand(n,n);
B = rand(n,n);
startTime = cputime();
myC = MM(A,B);
myTime = cputime() - startTime;
startTime = cputime();
trueAns = A*B;
matTime = cputime() - startTime;
if(isequal(round(myC.*10000)./10000, round(trueAns.*10000)./10000))
  disp("My MM took ");
  disp(myTime);
  disp("seconds");
  disp("Matlab took");
  disp(matTime);
  disp("seconds");
endif
myTimes = [];
matTimes = [];
sizes = [];
n = 2
for i = 1:7
  A = rand(n,n);
  B = rand(n,n);
  startTime = cputime();
  myC = MM(A, B);
  myTime = cputime() - startTime;
  myTimes(i) = myTime;
  startTime = time(); %gets seconds since the epoch
  matC = A*B;
  matTime = time() - startTime;
  matTime = matTime*100000;%now uS
  matTimes(i) = matTime;
  sizes(i) = n;
  n = n*2;
endfor

figure
hold
plot(sizes, myTimes);
plot(sizes, matTimes);
title("Matrix Multiplication Octave");
xlabel("n");
ylabel("time(s)");
legend("My matrix multiplication in s", "Octave's matrix multiplication * 10^5");
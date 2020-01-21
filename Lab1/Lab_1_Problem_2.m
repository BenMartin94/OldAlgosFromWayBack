#### WRITTEN IN OCTAVE, Minor adjustments required if to be run in matlab
# Prof. Jefferies said it was not required to do so.
n = input("Enter the size of the array to sort\n")
A = rand(1,n);
startTime = cputime();
mySort = insertionsort(A);
myTime = cputime() - startTime;
if(verifySorted(mySort) == 1)
  disp("My sort took ")
  disp(myTime)
  disp(" seconds")
endif
startTime = cputime();
trueSort = sort(A);
matlabTime = cputime() - startTime;
disp("Matlab sort took ")
disp(matlabTime)
disp(" seconds")

# data collection part

myTimes = [];
matTimes = [];
sizes = [];
n = 50
for i = 1:7
  A = rand(1,n);
  startTime = cputime();
  mySort = insertionsort(A);
  myTime = cputime() - startTime;
  myTimes(i) = myTime;
  startTime = time();
  matSort = sort(A);
  matTime = time() - startTime;
  matTimes(i) = matTime*100000;
  sizes(i) = n;
  n = n*2;
endfor

figure
hold
plot(sizes, myTimes);
plot(sizes, matTimes);
title("Insertion Sort Octave");
xlabel("n");
ylabel("time(s)");
legend("My insertion sort in s", "Octave's sort * 10^5");
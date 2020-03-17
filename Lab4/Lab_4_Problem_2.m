clc
clear all
close all


n=500;
A = 10*randn(n,n);
tic;
LU = LUDecomposition(A);
decompTime1 = toc;
solveTimes1 = zeros(10,1);
for i = 1:10
  b = 10*randn(n,1);
  x1 = A\b;
  tic;
  x2=LUSolve(LU,b);
  temp = toc;

  if(isequal(round(x1*100),round(x2*100)))
    solveTimes1(i) = temp;
  endif
endfor


N = 8;
solveTimes = zeros(N,1);
decompTimes = zeros(N,1);
sizes = zeros(N,1);
startingSize = 4;
for i = 1:N;
  A = 10*randn(startingSize,startingSize);
  b = 10*randn(startingSize,1);
  x1 = A\b;
  tic;
  LU = LUDecomposition(A);
  decompTime = toc;
  
  x2=LUSolve(LU,b);
  solveTime = toc;
  if(isequal(round(x1*100),round(x2*100)))
    solveTimes(i) = solveTime-decompTime;
    decompTimes(i) = decompTime;
    sizes(i) = startingSize;
  endif 
  startingSize = startingSize*2;

endfor
theoryTimesSolve = 1:N;
theoryTimesDecomp = 1:N;
theoryTimesSolve = theoryTimesSolve.^2;
theoryTimesDecomp = theoryTimesDecomp.^3;

figure

semilogx(sizes, solveTimes);
hold on
semilogx(sizes, theoryTimesSolve*0.0005);
hold off
title("Solving Time")
xlabel("Size of system");
ylabel("Time (s)");
legend("Actual Times", "Theoretical values cn^2");
saveas(gcf, "solveGraph.png");
figure
semilogx(sizes, decompTimes);
hold on
semilogx(sizes, theoryTimesDecomp*0.005);
hold off
title("Decomposition Time")
xlabel("Size of system");
ylabel("Time (s)");
legend("Actual Times", "Theoretical values cn^3");
saveas(gcf, "decompGraph.png");




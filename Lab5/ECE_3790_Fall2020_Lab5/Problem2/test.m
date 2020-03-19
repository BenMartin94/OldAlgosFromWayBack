close all
clear all
clc
f = @(x) -(x-2).^2 + 3;
[froot, ferror, froots, ferrors] = bisectionmethod(f, 0, 5, -1, 30);
figure
subplot(2,1,1);
plot(froots);
title("Root Approximations");
xlabel("Iteration");
ylabel("Root Approximation")
subplot(2,1,2);
plot(ferrors);
title("Root Approximation Errors");
xlabel("Iteration");
ylabel("Relative Error (%)")
saveas(gcf, "rootsFunction.png");
[fargmax, ferror, fargmaxs, ferrors] = goldensectionsearch(f, 0, 5, -1, 30);
figure
subplot(3,1,1);
plot(fargmaxs);
title("ArgMinimum Approximations");
xlabel("Iteration");
ylabel("ArgMinimum Approximation")
subplot(3,1,2);
plot(f(fargmaxs));
title("Minimum Approximations");
xlabel("Iteration");
ylabel("Minimum Approximation")
subplot(3,1,3);
plot(ferrors);
title("Minimum Approximation Errors");
xlabel("Iteration");
ylabel("Relative Error(%)")
saveas(gcf, "maxFunction.png");
%%%Attempts to find roots of spaceship, will not work because no roots exist
[spaceroot, spaceerror, spaceroots, spaceerrors] = bisectionmethod(@spaceship, 0, 180, -1, 5);
figure
subplot(2,1,1);
plot(froots);
title("Root Approximations for Space Ship");
xlabel("Iteration");
ylabel("Root Approximation")
subplot(2,1,2);
plot(ferrors);
title("Root Approximation Errors for Space Ship");
xlabel("Iteration");
ylabel("Relative Error (%)")
saveas(gcf, "rootsFunctionspace.png");

[answer, error, estimates, errors] = goldensectionsearch(@spaceship, 0, 270, 0.0001, 20);
%spaceshipWPlot(answer);
figure
subplot(3,1,1);
plot(estimates);
title("ArgMinimum Approximations");
xlabel("Iteration");
ylabel("ArgMinimum Approximation")
subplot(3,1,2);
evals = zeros(1, length(estimates));
for i = 1:length(estimates)
  evals(i) = spaceship(estimates(i));
endfor
plot(abs(evals));
title("Minimum Approximations");
xlabel("Iteration");
ylabel("Minimum Approximation")
subplot(3,1,3);
plot(errors);
title("Minimum Approximation Errors");
xlabel("Iteration");
ylabel("Relative Error(%)")
saveas(gcf, "maxFunctionspace.png");




figure

samples = zeros(1, 360);
for i = 1:360;
  samples(i) = spaceship(i);
  disp(i)
endfor
plot(abs(samples));
title("Sample space of Space Ship function");
xlabel("Launch Angle in degrees");
ylabel("Minimum Distance");
saveas(gcf, "samplespace.png");


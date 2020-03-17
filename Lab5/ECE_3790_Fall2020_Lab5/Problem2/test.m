f = @(x) -(x-2.2)^2+33;
fdev = @(x) 2*(x-2);
bisectionmethod(f, 0.1, 5, 1, 100);
goldensectionsearch(f, 0, 5.1, -1, 100);
[answer, error, estimates, errors] = goldensectionsearch(@spaceship, 0, 270, 0.0001, 20);
%spaceshipWPlot(answer);
figure
plot(errors);
title("Error");
xlabel("Iteration");
ylabel("Percentage");

figure
plot(estimates);
title("Estimate");
xlabel("Iteration");
ylabel("Degrees");


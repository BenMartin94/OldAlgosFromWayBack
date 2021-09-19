close all
clear all
clc

data = dlmread('./Lab5_Problem1_Data.csv');


days = data(:,1);
world = data(:,2);
china = data(:,3);
usa = data(:,4);
usaRange = usa(48:length(usa));
usaDays = days(48:length(days));

chinaRange = china(1:20);
chinaDays = days(1:20);

worldRange = world(28:length(world));
worldDays = days(28:length(days));
figure;
hold on
plot(days, world);
plot(days, china);
plot(days, usa);
legend("World", "China", "USA");
xlabel("Days");
ylabel("Cases");
title("Unselected");

figure;
hold on
plot(worldDays, worldRange);
plot(chinaDays, chinaRange);
plot(usaDays, usaRange);
legend("World", "China", "USA");
title("Ranges");
xlabel("Days");
ylabel("Cases");


%%%%%%%%%%%VALIDATION%%%%%%%%%%%%%%%%%
a0 = 0.5;
a1 = 0.1;
a2 = 0.9;
a3 = 0.8;
y = @(x) a0+a1*x+a2*x.^2+a3*x.^3;
x = 1:100;
yout = y(x);
[plys, r2] = polynomialregression(yout, 3);
assert(r2==1);

%exp val
a0 = 1.1;
a1 = 0.3;
y = @(x) a0*exp(a1*x);
x = 1:100;
yout = y(x);
figure
scatter(x,yout);
hold on
[plys, r2] = exponentialregression(yout);
fplot(@(x) plys(1)*exp(plys(2)*(x)), [min(x), max(x)]);
assert(r2==1);


%%%now with the real data
%%china
[plys, r2] = polynomialregression(chinaRange, 3)
chinaCFH = @(x) plys(1)+plys(2)*x+plys(3)*x.^2+plys(4)*x^3;
figure;
subplot(2,1,1);
hold on
fplot(chinaCFH, [0, max(chinaDays)]);
scatter(chinaDays, chinaRange);
legend()
title(strcat("China Polynomial Model r^2 = ", mat2str(r2)(1:4)));
xlabel("Days");
ylabel("Confirmed cases");

[plys, r2] = exponentialregression(chinaRange)
chinaEFH = @(x) plys(1)*exp(plys(2)*x);
subplot(2,1,2);
hold on
fplot(chinaEFH, [0, max(chinaDays)]);
scatter(chinaDays, chinaRange);
legend()
title(strcat("China Exponential Model r^2 = ", mat2str(r2)(1:4)));
xlabel("Days");
ylabel("Confirmed cases");
saveas(gcf, "ChinaRegression.png");
%USA
startingDay = min(usaDays);
[plys, r2] = polynomialregression(usaRange, 3)
usaCFH = @(x) plys(1)+plys(2)*(x)+plys(3)*(x).^2+plys(4)*(x)^3;
figure;
subplot(2,1,1);
hold on
fplot(usaCFH, [0, length(usaDays)]);
scatter(usaDays-startingDay+1,usaRange);
legend()
title(strcat("USA Polynomial Model r^2 = ", mat2str(r2)(1:4)));
xlabel("Days after day 47");
ylabel("Confirmed cases");

[plys, r2] = exponentialregression(usaRange)
usaEFH = @(x) plys(1)*exp(plys(2)*(x));
subplot(2,1,2);
hold on
fplot(usaEFH, [1, length(usaDays)]);
scatter(usaDays-startingDay+1, usaRange);
legend()
title(strcat("USA Exponential Model r^2 = ", mat2str(r2)(1:4)));
xlabel("Days after day 47");
ylabel("Confirmed cases");

saveas(gcf, "USARegression.png");
%%%WORLD

[plys, r2] = polynomialregression(worldRange, 3)
startingDay = min(worldDays);
worldCFH = @(x) plys(1)+plys(2)*(x)+plys(3)*(x).^2+plys(4)*(x)^3;
figure;
subplot(2,1,1);
hold on
fplot(worldCFH, [1, length(worldDays)]);
scatter(worldDays-startingDay+1, worldRange);
legend()
title(strcat("World Polynomial Model r^2 = ", mat2str(r2)(1:4)));

xlabel("Days after day 27");
ylabel("Confirmed cases");

[plys, r2] = exponentialregression(worldRange)
worldEFH = @(x) plys(1)*exp(plys(2)*(x));
subplot(2,1,2);

hold on
fplot(worldEFH, [1, length(worldDays)]);
scatter(worldDays-startingDay+1, worldRange);
legend()
title(strcat("World Exponential Model r^2 = ", mat2str(r2)(1:4)));

xlabel("Days after day 27");
ylabel("Confirmed cases");
saveas(gcf, "WorldRegression.png");




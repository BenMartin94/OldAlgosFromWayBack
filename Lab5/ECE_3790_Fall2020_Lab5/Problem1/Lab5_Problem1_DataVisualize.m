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

%testing linar
data = zeros(length(chinaDays), 3);
data(:, 1) = ones(length(chinaDays),1)
data(:,2) = chinaDays;
data(:,3) = chinaRange;
[asc, r2] = linearregression(data)

chinaLFH = @(x) asc(1) + asc(2)*x;
figure;
hold on
fplot(chinaLFH, [0, max(chinaDays)]);
plot(chinaDays, chinaRange);
legend()

% testing poly
[plys, r2] = polynomialregression(chinaRange, 3)
chinaCFH = @(x) plys(1)+plys(2)*x+plys(3)*x.^2+plys(4)*x^3;
figure;
hold on
fplot(chinaCFH, [0, max(chinaDays)]);
plot(chinaDays, chinaRange);
legend()

%testing exp
[plys, r2] = exponentialregression(chinaRange)
chinaEFH = @(x) plys(1)*exp(plys(2)*x);
figure;
hold on
fplot(chinaEFH, [0, max(chinaDays)]);
plot(chinaDays, chinaRange);
legend()

%%%%%%%%%%%VALIDATION%%%%%%%%%%%%%%%%%
a0 = 0.5;
a1 = 0.1;
a2 = 0.9;
a3 = 0.8;
y = @(x) a0+a1*x+a2*x.^2+a3*x.^3;
x = 10:100;
yout = y(x);
[plys, r2] = polynomialregression(yout, 3);
assert(r2==1);

%exp val
a0 = 1.1;
a1 = 0.3;
y = @(x) a0*exp(a1*x);
x = 10:100;
yout = y(x);
[plys, r2] = exponentialregression(yout);
assert(r2==1);


%%%now with the real data
%%china
[plys, r2] = polynomialregression(chinaRange, 3)
chinaCFH = @(x) plys(1)+plys(2)*x+plys(3)*x.^2+plys(4)*x^3;
figure;
hold on
fplot(chinaCFH, [0, max(chinaDays)]);
plot(chinaDays, chinaRange);
legend()
title("China");
xlabel("Days");
ylabel("Confirmed cases");

[plys, r2] = exponentialregression(chinaRange)
chinaEFH = @(x) plys(1)*exp(plys(2)*x);
figure;
hold on
fplot(chinaEFH, [0, max(chinaDays)]);
plot(chinaDays, chinaRange);
legend()
title("China");
xlabel("Days");
ylabel("Confirmed cases");
%USA
startingDay = min(usaDays);
[plys, r2] = polynomialregression(usaRange, 3)
usaCFH = @(x) plys(1)+plys(2)*(x-startingDay)+plys(3)*(x-startingDay).^2+plys(4)*(x-startingDay)^3;
figure;
hold on
fplot(usaCFH, [min(usaDays), max(usaDays)]);
plot(usaDays, usaRange);
legend()
title("USA");
xlabel("Days");
ylabel("Confirmed cases");

[plys, r2] = exponentialregression(usaRange)
usaEFH = @(x) plys(1)*exp(plys(2)*(x-startingDay));
figure;
hold on
fplot(usaEFH, [min(usaDays), max(usaDays)]);
plot(usaDays, usaRange);
legend()
title("USA");
xlabel("Days");
ylabel("Confirmed cases");

%%%WORLD

[plys, r2] = polynomialregression(worldRange, 3)
startingDay = min(worldDays);
worldCFH = @(x) plys(1)+plys(2)*(x-startingDay)+plys(3)*(x-startingDay).^2+plys(4)*(x-startingDay)^3;
figure;
hold on
fplot(worldCFH, [min(worldDays), max(worldDays)]);
plot(worldDays, worldRange);
legend()
title("World");
xlabel("Days");
ylabel("Confirmed cases");

[plys, r2] = exponentialregression(worldRange)
worldEFH = @(x) plys(1)*exp(plys(2)*(x-startingDay));
figure;
hold on
fplot(worldEFH, [min(worldDays), max(worldDays)]);
plot(worldDays, worldRange);
legend()
title("World");
xlabel("Days");
ylabel("Confirmed cases");




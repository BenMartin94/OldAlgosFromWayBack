clc
clear all
close all
A = [1 2 3;
     2 5 2;
     3 7 4];

b = [1;2;1];

n=1000
A = 10*randn(n,n);
b = 10*randn(n,1);
x1 = A\b;
LU = LUDecomposition(A);

x2=LUSolve(LU,b);
x1-x2
isequal(x1,x2)

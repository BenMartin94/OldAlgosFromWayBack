f = @(x) -(x-2.2)^2+33;
fdev = @(x) 2*(x-2);
bisectionmethod(f, 0.1, 5, 1, 100);
goldensectionsearch(f, 0, 5.1, -1, 100)

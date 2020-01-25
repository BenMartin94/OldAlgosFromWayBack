import numpy
import math

def mergeSortFunc(n):
    if(n <= 1):
        return 1
    else:
        return 1+2*mergeSortFunc(math.floor(n/2))+n




def hanoi(n):
    if(n<=1):
        return 1
    else:
        return 2*hanoi(n-1)+1

def f3(n):
    if (n <= 1):
        return 1
    else:
        return 9*f3(n/3)+n

def f4(n):
    if (n <= 1):
        return 1
    else:
        return 2*f4(n/4)+n*n
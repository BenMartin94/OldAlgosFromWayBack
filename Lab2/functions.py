import numpy
import math

def mergeSortFunc(n):
    if(n <= 1):
        return 1
    else:
        return 1+2*mergeSortFunc((n/2))+n

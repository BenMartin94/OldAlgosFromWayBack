import time
import numpy as np
import matplotlib.pyplot as plt

def recursiveLCS(x, y):
    if(len(x)==0 or len(y)==0):
        return 0
    xF = x[len(x)-1]
    yF = y[len(y)-1]
    if(xF == yF):
        return (recursiveLCS(x[0:len(x)-1], y[0:len(y)-1]))+1
    elif(xF != yF):
        return max(recursiveLCS(x[0:len(x)-1], y), recursiveLCS(x, y[0:len(y)-1]))

def bottomUp(x, y):

    maxSoFar = np.zeros((len(x), len(y)))
    for i in range(len(x)):
        for j in range(len(y)):
            if(i==0 or j == 0):
                maxSoFar[i, j] = 0
            elif(x[i-1] == y[j-1]):
                maxSoFar[i,j] = maxSoFar[i-1, j-1]+1
            else:
                maxSoFar[i,j] = max(maxSoFar[i, j-1], maxSoFar[i-1, j])
    return np.max(maxSoFar)

    '''
    k = min(len(x), len(y))
    maxSoFar = np.zeros(k)
    i = 0
    j = 0
    while i<len(i):
        while j<len(y):
            
    '''
x = [3,6,4, 2, 1, 5, 6, 8]
y = [1,3,5,4]

print(recursiveLCS(x, y))
print(bottomUp(x, y))
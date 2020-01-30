import numpy as np
import matplotlib.pyplot as plt

import functions

# function to evaluate the cost of given function for a given vector of n values
def evaluaterecurrence(myfunction, nvec):
    costs = []
    for n in nvec:
        costs.append(funcDict[myfunction](n))
    return costs

# function to generate linear vector of values from given starting number, with given interval
def genNvec(startingN, samples, rateOfChange):
    toRet = []
    n = startingN
    for i in range(samples):
        toRet.append(n)
        n = n+rateOfChange
    return toRet

# function to generate powers of 2 of given starting number
def genNvecPowersOf2(startingN, samples):
    toRet = []
    n = startingN
    for i in range(samples):
        toRet.append(n)
        n = n *2
    return toRet

# dictionary to retrieve function names
funcDict = {
    'mergeSortFunc': functions.mergeSortFunc,
    'f2': functions.hanoi,
    'f3': functions.f3,
    'f4': functions.f4
}

# generate vectors
nvec1 = genNvecPowersOf2(200, 100)
nvec2 = genNvec(350, 20, 1)
nvec3 = genNvecPowersOf2(35, 50)
nvec4 = genNvec(350, 100, 1)


# Plot results
plt.plot(nvec1, evaluaterecurrence('mergeSortFunc', nvec1))
plt.xlabel("n value")
plt.ylabel("cost")
plt.title("Merge Sort Cost")
plt.show()

plt.figure()
plt.plot(nvec2, evaluaterecurrence('f2', nvec2))
plt.xlabel("n value")
plt.ylabel("cost")
plt.title("Tower of Hanoi Cost")
plt.show()

plt.figure()
plt.plot(nvec3, evaluaterecurrence('f3', nvec3))
plt.xlabel("n value")
plt.ylabel("cost")
plt.title("Recursive Algorithm 3 Cost")
plt.show()

plt.figure()
plt.plot(nvec4, evaluaterecurrence('f4', nvec4))
plt.xlabel("n value")
plt.ylabel("cost")
plt.title("Recursive Algorithm 4 Cost")
plt.show()

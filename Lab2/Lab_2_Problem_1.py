import numpy as np
import matplotlib.pyplot as plt

import functions


def evaluaterecurrence(myfunction, nvec):
    costs = []
    for n in nvec:
        costs.append(funcDict[myfunction](n))
    return costs

def genNvec(startingN, samples, rateOfChange):
    toRet = []
    n = startingN
    for i in range(samples):
        toRet.append(n)
        n = n+rateOfChange
    return toRet

def genNvecPowersOf2(startingN, samples):
    toRet = []
    n = startingN
    for i in range(samples):
        toRet.append(n)
        n = n *2
    return toRet


funcDict = {
    'mergeSortFunc': functions.mergeSortFunc,
    'f2': functions.hanoi,
    'f3': functions.f3,
    'f4': functions.f4
}

nvec1 = genNvecPowersOf2(200, 100)
nvec2 = genNvec(350, 20, 1)
nvec3 = genNvec(350, 100, 1)
nvec4 = genNvec(350, 100, 1)


plt.plot(nvec1, evaluaterecurrence('mergeSortFunc', nvec1))
plt.show()
plt.figure()
plt.plot(nvec2, evaluaterecurrence('f2', nvec2))
plt.show()
plt.figure()
plt.plot(nvec3, evaluaterecurrence('f3', nvec3))
plt.show()
plt.figure()
plt.plot(nvec4, evaluaterecurrence('f4', nvec4))
plt.show()

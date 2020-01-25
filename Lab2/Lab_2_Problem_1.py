import numpy as np
import matplotlib.pyplot as plt

import functions


def evaluaterecurrence(myfunction, nvec):
    costs = []
    for n in nvec:
        costs.append(funcDict[myfunction](n))
    return costs


funcDict = {
    'mergeSortFunc': functions.mergeSortFunc
}

plt.plot(evaluaterecurrence('mergeSortFunc', np.linspace(1, 20)))
plt.show()


import numpy as np
import time
import matplotlib.pyplot as plt


# bad top down recursion to find and return given nth fibonacci number.
def recursiveFibonacci(nth):

    if (nth <=2):
        return 1
    else:
        return (recursiveFibonacci(nth-1) + recursiveFibonacci(nth-2))
#recursiveFibonacci



# memoized recursive Fibonacci method wrapper method
def memoizedFibonacci(nth):
    # catch fringe/base cases
    if (nth <=2):
        return 1

    #call memoized method to do the work
    else:
        nMinus1, nMinus2 =  memoizedFibonacci2(nth-1)
        return (nMinus1 + nMinus2)
#memoizedFibonacci



# memoized recursive Fibonacci method. Calculates nth value, and 
# returns n value and n-1 value to prevent redundant calculations
def memoizedFibonacci2(nth):
    #base case
    if (nth == 2):
        nMinus1 = 1
        nMinus2 = 0
    else: #recursive call 
        nMinus1, nMinus2 = memoizedFibonacci2(nth-1)
    
    #calculate n value, and return n-1 value for efficiency
    nVal = nMinus1 + nMinus2
    return nVal, nMinus1
#memoizedFibonacci2



# returns value of given nth fibonacci number
def bottomupFibonacci(nth):
    current = 1      # highest calculated fibonacci number
    currentVal = 1   # value of highest calculated number
    lastVal = 0      # value of the current-1 number

    # calculation loop
    while current < nth:
        holder = currentVal
        currentVal = currentVal + lastVal
        lastVal = holder
        current = current +1

    return currentVal
#bottomupFibonacci    




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






#that's right
def main():

    # make n vectors
    nVecA = genNvec(1, 25, 1)
    nVecB = genNvec(1, 300, 2)
    nVecC = genNvec(1, 300, 2)

    # arrays to hold times for algorithms
    aTimes = []
    bTimes = []
    cTimes = []

    #holds results because its sad that they just get thrown away
    aresults = []
    bresults = []
    cresults = []

    # dooda kaka layyyshuns
    for n in nVecA:
        startTime = time.time()
        aresults.append(recursiveFibonacci(n))  
        aTimes.append(time.time() - startTime)

    for n in nVecB:
        startTime = time.time()
        bresults.append(memoizedFibonacci(n))  
        bTimes.append(time.time() - startTime)

    for n in nVecC:
        startTime = time.time()
        cresults.append(bottomupFibonacci(n))  
        cTimes.append(time.time() - startTime)



    # plot results of recursive Fibonacci method
    plt.figure()
    plt.plot(nVecA, aTimes)
    plt.xlabel("n")
    plt.ylabel("time (s)")
    plt.title("Recursive Fibonacci Method Calculation Times")
    plt.show()

    # plot results of memoized and bottom up methods
    plt.figure()
    plt.plot(nVecB, bTimes)
    plt.xlabel("n")
    plt.ylabel("time (s)")
    plt.title("Memoized Fibonacci Method Calculation Times")
    plt.show()

    plt.figure()
    plt.plot(nVecC, cTimes)
    plt.xlabel("n")
    plt.ylabel("time (s)")
    plt.title("Bottom-Up Fibonacci Method Calculation Times")
    plt.show()
    
    return "\n Program ended sucessfully"
#main





# just DO it
print(main())
 





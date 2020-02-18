import time
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D



# Recursive method to find LCS
def recursiveLCS(x, y):
    if(len(x)==0 or len(y)==0):
        return 0
    xF = x[len(x)-1]
    yF = y[len(y)-1]
    if(xF == yF):
        return (recursiveLCS(x[0:len(x)-1], y[0:len(y)-1]))+1
    elif(xF != yF):
        return max(recursiveLCS(x[0:len(x)-1], y), recursiveLCS(x, y[0:len(y)-1]))
#recursiveLCS




# Bottom-up method to find LCS ***DOES NOT WORK***
def bottomUp(x, y):

    maxSoFar = np.zeros((len(x), len(y)))
    for i in range(len(x)):
        for j in range(len(y)):
            if(i==0 or j == 0):
                maxSoFar[i, j] = 0
            elif(x[i-1] == y[j-1]):##cant be 0 because that was handled above
                maxSoFar[i,j] = maxSoFar[i-1, j-1]+1
            else:
                maxSoFar[i,j] = max(maxSoFar[i, j-1], maxSoFar[i-1, j])
    return np.max(maxSoFar)
#bottomUp




#helper method for recursiveMemoLCS
def memoizedLCS(x, y):
    memoy = np.zeros((len(x), len(y)), int) #build memo
    return recursiveMemoLCS(x, y, memoy)
#memoizedLCS




# The main recursive method. uses memo to prevent recalculating values
def recursiveMemoLCS(x,y, memo):
    if(len(x) == 0 or len(y) == 0):
        memo[len(x)-1, len(y)-1] = 0
    xF = x[len(x) - 1]
    yF = y[len(y) - 1]
    if (memo[len(x)-1, len(y)-1] > 0):
        return memo[len(x)-1, len(y)-1]
    elif(xF == yF):
        memo[len(x)-1, len(y)-1] = (recursiveLCS(x[0:len(x)-1], y[0:len(y)-1]))+1
        return(memo[len(x)-1, len(y)-1])
    else:
        memo[len(x)-2, len(y)-1] = recursiveLCS(x[0:len(x)-1], y)
        memo[len(x)-1, len(y)-2] = recursiveLCS(x, y[0:len(y)-1])
        return max(memo[len(x)-1, len(y)-2], memo[len(x)-2, len(y)-1])
#recursiveMemoLcs





# finds the LCS by checking evey possible sub array of x, against every possible subarray of y
# returns both the length of the LCS, and the LCS itself
def bruteForceLCS(x, y):

    # binary values of each item in arrays, 1 represents an item to compare
    xBin = BinaryEncodedVal(x)
    yBin = BinaryEncodedVal(y)
    
    # holds binary value to represent array of LCS
    xMax = 0b0
    yMax = 0b0

    # maximum length of LCS
    maxLCS = 0 

    #check all possibilities
    while xBin > 0 :
        xOnes = binaryOnes(xBin) # number of how many elements are in the array to check
        yBin2 = yBin
        # check all subarrays of y
        while yBin2 > 0:
            # if y sub array is the same length as x sub array, check it
            if (binaryOnes(yBin2) == xOnes):
                equal = compareSubarrays(x, y, xBin, yBin2)

                if equal:
                    if xOnes > maxLCS:  # check if max is found
                        maxLCS = xOnes  # update max
                        xMax = xBin     # store binary encoded value for LCS in x
                        yMax = yBin     # store binary encoded value for LCS in y
                    # else, we dont care
            # try next sub array of y
            yBin2 = yBin2 -1
        xBin = xBin - 1      #move to next subarray of x

    return maxLCS , getSubArray(x, xMax) #return longest LCS
#bruteForceLCS
    


# returns an binary encoded value to represent each slot in an array
# starts wih a 1 for each position
def BinaryEncodedVal(givenArray):
    binaryVal = 0b0

    for item in givenArray:
        binaryVal = (binaryVal << 1) | 1

    return binaryVal
#BinaryEncodedVal



#returns a count of all ones in given binary number
def binaryOnes(BinaryNum):
    check = BinaryNum
    onesSum = 0 # counter for each 1 found in the binary number
    
    while check > 0:
        if (check & 1): #if it a 1, add it to the sum
            onesSum = onesSum + 1
        check = check >> 1

    return onesSum
#binaryOnes



# returns boolean of whether given subarrays are equal
def compareSubarrays(arrayx, arrayy, xBin, yBin):

    xSub = getSubArray(arrayx, xBin)
    ySub = getSubArray(arrayy, yBin)

    return (xSub == ySub)



# returns the sub array defined by xBin
def getSubArray(arrayx, xBin):
    subx = []
    for item in arrayx:
        if (xBin & 1): # if item is supposed to be in array, add it
            subx.append(item)
        xBin = xBin >> 1
    return subx
#getSubArray





# function to generate linear vector of values from given starting number, with given interval
def genNvec(startingN, samples, rateOfChange):
    toRet = []
    n = startingN
    for i in range(samples):
        toRet.append(n)
        n = n+rateOfChange
    return toRet
#genNvec





###~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


def main():

    n = 12                      # value to determine lengths n and m of arrays x and y
    allMatch = True             # boolean to keep track of if all results agree
    nVec = genNvec(1, n, 1)     # generate vector of n values

    # arrays to hold times for calculations
    recursiveTimes = []
    memoizedTimes = []
    bruteForceTimes = []

    # Title for output
    print("\nRecursive\tMemoized\tBrute Force\t LCS")
    print("------------------------------------------------------")

    # run calculations
    for length in nVec:
        x = np.random.randint(0, 9, length) 
        y = np.random.randint(0, 9, length) 

        startTime = time.time()
        recursiveVal = recursiveLCS(x, y)
        recursiveTimes.append(time.time() - startTime)

        startTime = time.time()
        memoizedVal = memoizedLCS(x, y)
        memoizedTimes.append(time.time() - startTime)

        startTime = time.time()
        bruteForceVal = bruteForceLCS(x, y)
        bruteForceTimes.append(time.time() - startTime)

        # output results, and check if all results match
        print(recursiveVal, "\t\t", memoizedVal, "\t\t", bruteForceVal[0], "\t\t", bruteForceVal[1])
        allMatch = allMatch and (recursiveVal == memoizedVal == bruteForceVal[0])

    # output summary
    if allMatch:
        print("\nAll results are correct")
    else:
        print("\nERROR: Not all results are equal")



    # exponential plots for comparison
    exp = []
    for val in nVec:
        exp.append((val**(val/3))/(100000))

    exp2 = exp

    exp3 = []
    for val in nVec:
        exp3.append((val**(val/2))/(100000))



    # plot results of recursive method
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.set_yscale('log')
    plt.plot(nVec, recursiveTimes)
    plt.plot(nVec, exp)
    plt.xlabel("x, y array length")
    plt.ylabel("time (s)")
    plt.title("Recursive LCS Method Calculation Times")
    plt.legend(['Recursive Method Times', '(n^n/3)x10^-6'])
    plt.show()
    
    # plot results of memoized method
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    plt.plot(nVec, memoizedTimes)
    ax.set_yscale('log')
    plt.plot(nVec, exp2)
    plt.xlabel("x, y array length")
    plt.ylabel("time (s)")
    plt.title("Memoized LCS Method Calculation Times")
    plt.legend(['Memoized Method Times', '(n^n/3)x10^-6'])
    plt.show()

    # plot results of brute force method
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    plt.plot(nVec, bruteForceTimes)
    ax.set_yscale('log')
    plt.plot(nVec, exp3)
    plt.xlabel("x, y array length")
    plt.ylabel("time (s)")
    plt.title("Brute Force LCS Method Calculation Times")
    plt.legend(['Brute Force Method Times', '(n^n/2)x10^-6'])
    plt.show()


    return("Program Ended Successfully")
#main




print(main())


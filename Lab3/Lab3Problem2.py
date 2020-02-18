import time
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D



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
            elif(x[i-1] == y[j-1]):##cant be 0 because that was handled above
                maxSoFar[i,j] = maxSoFar[i-1, j-1]+1
            else:
                maxSoFar[i,j] = max(maxSoFar[i, j-1], maxSoFar[i-1, j])
    return np.max(maxSoFar)



#helper method for recursiveMemoLCS
def memoizedLCS(x, y):
    memoy = np.zeros((len(x), len(y))) #build memo
    return recursiveMemoLCS(x, y, memoy)
    
#memoizedLCS





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






def main():

    n = 12

    #generate vectors on n values (starting n, how many n, step)
    nVec1 = genNvec(1, n, 1)
    nVec2 = genNvec(1, n, 1)


    recursiveTimes = []
    for length in nVec1:
        x = np.random.randint(0, 9, length) 
        y = np.random.randint(0, 9, length) 

        startTime = time.time()
        recuresiveVal = recursiveLCS(x, y)
        recursiveTimes.append(time.time() - startTime)


    memoizedTimes = []
    for length in nVec2:
        x = np.random.randint(0, 9, length) 
        y = np.random.randint(0, 9, length) 

        startTime = time.time()
        memoizedVal = memoizedLCS(x, y)
        memoizedTimes.append(time.time() - startTime)

    exp = []
    for val in nVec1:
        exp.append((val**(val/3))/(100000))



    # plot results of recursive method
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.set_yscale('log')
    plt.plot(nVec1, recursiveTimes)
    plt.plot(nVec1, exp)
    plt.xlabel("x, y array length")
    plt.ylabel("time (s)")
    plt.title("Recursive LCS Method Calculation Times")
    plt.legend(['Recursive Method Times', '(n^n/3)x10^-6'])
    plt.show()
    
    # plot results of rmemoized method
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    plt.plot(nVec1, memoizedTimes)
    ax.set_yscale('log')
    plt.plot(nVec1, exp)
    plt.xlabel("x, y array length")
    plt.ylabel("time (s)")
    plt.title("Memoized LCS Method Calculation Times")
    plt.legend(['Memoized Method Times', '(n^n/3)x10^-6'])
    plt.show()

    x = np.random.randint(0, 9, 5)
    y = np.random.randint(0, 9, 5)
    startTime = time.time()
    bruteVal = bruteForceLCS(x, y)
    bruteTime = time.time() - startTime




    # output results
    print("Brute Force:\t", bruteVal[0], "\t", bruteTime)
    print("\nLCS:\t", bruteVal[1])


    # # 3d plot 

    # nVec1 = genNvec(0, 10, 1)

    # mVec1 = genNvec(0, 20, 1)

    # times = np.zeros((len(nVec1), len(mVec1)))
    # nVec = []
    # mVec = []

    # for n in nVec1:
    #     nVec.append([])
    #     mVec.append([])
    #     for m in mVec1:
    #             x = np.random.randint(0, 5, m) 
    #             y = np.random.randint(0, 5, n) 
    #             startTime = time.time()
    #             recuresiveVal = recursiveLCS(x, y)
    #             recursiveTime = time.time() - startTime
    #             times[n][m] = recursiveTime
    #             nVec[n].append(n)
    #             mVec[n].append(m)


    # # plot results of bottom up method


    # fig = plt.figure()
    # fig.patch.set_facecolor('white')
    # recursivePlot = fig.add_subplot(111, projection='3d')
    # recursivePlot.plot_surface(nVec, mVec, times, cmap='viridis', edgecolor='none')
    # recursivePlot.set_xlabel("n")
    # recursivePlot.set_ylabel("m")
    # recursivePlot.set_zlabel("time (s)")
    # recursivePlot.set_title("Recursive LCS Method Calculation Times")
    # #recursivePlot.legend(['Recursive Method Times'])
    # plt.show()
    


    return("Program Ended Successfully")
#main




print(main())


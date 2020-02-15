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
            elif(x[i-1] == y[j-1]):##cant be 0 because that was handled above
                maxSoFar[i,j] = maxSoFar[i-1, j-1]+1
            else:
                maxSoFar[i,j] = max(maxSoFar[i, j-1], maxSoFar[i-1, j])
    return np.max(maxSoFar)

def recursiveMemoLCS(x,y, memo):
    if(len(x) == 0 or len(y) == 0):
        memo[len(x)-1, len(y)-1] = 0
    xF = x[len(x) - 1]
    yF = y[len(y) - 1]
    if (memo[len(x)-1, len(y)-1] > 0):
        return memo[len(x), len(y)]
    elif(xF == yF):
        memo[len(x)-1, len(y)-1] = (recursiveLCS(x[0:len(x)-1], y[0:len(y)-1]))+1
        return(memo[len(x)-1, len(y)-1])
    else:
        memo[len(x)-2, len(y)-1] = recursiveLCS(x[0:len(x)-1], y)
        memo[len(x)-1, len(y)-2] = recursiveLCS(x, y[0:len(y)-1])
        return max(memo[len(x)-1, len(y)-2], memo[len(x)-2, len(y)-1])


# finds the LCS by checking evey possible sub array of x, against every possible subarray of y
def bruteForceLCS(x, y):

    # binary values length of arrays, 1 represents string to compare
    xBin = BinaryEncodedVal(x)
    yBin = BinaryEncodedVal(y)
    
    # holds binary value to represent array
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
        xBin = xBin - 1      #move to next x subarray


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

# returns boolean of if arrays are equal
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


x = np.random.randint(0, 5, 7) #[3,6,4, 2, 1, 5, 6, 8]
y = np.random.randint(0, 5, 7) #[1,3,5,4]
print(x)
print(y)


print(recursiveLCS(x, y))
#print(bottomUp(x, y)) doesn't work
print(bruteForceLCS(x, y))
memoy = np.zeros((len(x), len(y)))
print(recursiveMemoLCS(x, y, memoy))
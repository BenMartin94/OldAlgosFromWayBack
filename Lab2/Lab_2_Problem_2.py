import numpy as np
import time
import matplotlib.pyplot as plt
def recursivematrixmultiply(a, b):

    #Check that matrix shapes are valid for multiplication
    if (np.shape(a)[1] != np.shape(b)[0]):
        print("Given matrix sizes cannot be multiplied together")
        SystemExit()

    aRows, aCols = np.shape(a)
    bRows, bCols = np.shape(b)
    C = np.matrix(np.zeros((aRows, bCols)))
    cRows = aRows
    cCols= bCols
    if(np.size(a) == 1):
        C = a[0,0]*b
        return C
    elif(np.size(b)==1):
        C = a*b[0,0]
    elif(aRows * aCols * bRows * bCols == 0):
        return C
    else:
        aSplitX = aRows // 2 # what value to divide by rows for
        aSplitY = aCols // 2 # the value to divide columns into
        bSplitX = aSplitY    # where to divide matrix B columns
        bSplitY = aSplitX    # where to divide matrix B rows
        cSplitX, cSplitY = aSplitX, bSplitY # where the splits match up with the multiplied matrix

        # divide input matrices into sub problems
        A11 = a[0:aSplitX, 0:aSplitY]
        A12 = a[0:aSplitX, aSplitY:aCols]
        A21 = a[aSplitX:aRows, 0:aSplitY]
        A22 = a[aSplitX:aRows, aSplitY:aCols]

        B11 = b[0:bSplitX, 0:bSplitY]
        B12 = b[0:bSplitX, bSplitY:bCols]
        B21 = b[bSplitX:bRows, 0:bSplitY]
        B22 = b[bSplitX:bRows, bSplitY:bCols]

        # Call recursive function to solve each subsection of the matrix
        C[0:cSplitX, 0:cSplitY] = recursivematrixmultiply(A11, B11) + recursivematrixmultiply(A12, B21)
        C[0:cSplitX, cSplitY:cCols] = recursivematrixmultiply(A11, B12) + recursivematrixmultiply(A12, B22)
        C[cSplitX:cRows, 0:cSplitY] = recursivematrixmultiply(A21, B11) + recursivematrixmultiply(A22, B21)
        C[cSplitX:cRows, cSplitY:cCols] = recursivematrixmultiply(A21, B12) + recursivematrixmultiply(A22, B22)

    # return solved matrix
    return C


def recursiveMatrixMultiply(a, b, aRowStart, aRowEnd, aColStart, aColEnd,
                            bRowStart, bRowEnd, bColStart, bColEnd):

    # Check that matrix shapes are valid for multiplication
    if (np.shape(a)[1] != np.shape(b)[0]):
        print("Given matrix sizes cannot be multiplied together")
        SystemExit()

    # number of rows and columns in each given matrix
    aRows = aRowEnd-aRowStart
    aCols = aColEnd-aColStart
    bRows = bRowEnd-bRowStart
    bCols = bColEnd-bColStart

    #create matrix for result
    C = np.matrix(np.zeros((aRowEnd-aRowStart, bColEnd-bColStart)))
    cRows = aRowEnd-aRowStart
    cCols= bColEnd-bColStart

    # if one matrix is 1x1, find result (base case)
    if(aRows == aCols == 1):
        C = a[aRowStart, aColStart]*b[bRowStart:bRowEnd, bColStart:bColEnd]
        return C

    elif (bRows == bCols == 1):
        C = a[aRowStart:aRowEnd, aColStart:aColEnd]*b[bRowStart, bColStart]
        return C

    # catch occurrence of 0x0 matrix multiplication
    elif(aRows * aCols * bRows * bCols == 0):
        return C

    # Divide Step
    else:
        aRowSplit = (aRows // 2) + aRowStart # index to divide matrix A rows
        aColSplit = (aCols // 2) + aColStart # index to divide matrix A columns
        bRowSplit = (aCols // 2) + bRowStart # where to divide matrix B columns
        bColSplit = (aRows // 2) + bColStart # where to divide matrix B rows
        cRowSplit, cColSplit = (aRows // 2), (bCols // 2) # where the splits match up with the resultant matrix

        # Call recursive function to solve each subsection of the matrix
        C[0:cRowSplit, 0:cColSplit] = recursiveMatrixMultiply(a, b, aRowStart, aRowSplit, aColStart, aColSplit, bRowStart, bRowSplit, bColStart, bColSplit) \
                                      + recursiveMatrixMultiply(a, b, aRowStart, aRowSplit, aColSplit, aColEnd, bRowSplit, bRowEnd, bColStart, bColSplit)
        C[0:cRowSplit, cColSplit:cCols] = recursiveMatrixMultiply(a, b, aRowStart, aRowSplit, aColStart, aColSplit, bRowStart, bRowSplit, bColSplit, bColEnd) \
                                          + recursiveMatrixMultiply(a, b, aRowStart, aRowSplit, aColSplit, aColEnd, bRowSplit, bRowEnd, bColSplit, bColEnd)
        C[cRowSplit:cRows, 0:cColSplit] = recursiveMatrixMultiply(a, b, aRowSplit, aRowEnd, aColStart, aColSplit, bRowStart, bRowSplit, bColStart, bColSplit) \
                                          + recursiveMatrixMultiply(a, b, aRowSplit, aRowEnd, aColSplit, aColEnd, bRowSplit, bRowEnd, bColStart, bColSplit)
        C[cRowSplit:cRows, cColSplit:cCols] = recursiveMatrixMultiply(a, b, aRowSplit, aRowEnd, aColStart, aColSplit, bRowStart, bRowSplit, bColSplit, bColEnd) \
                                              + recursiveMatrixMultiply(a, b, aRowSplit, aRowEnd, aColSplit, aColEnd, bRowSplit, bRowEnd, bColSplit, bColEnd)

    # return solved matrix
    return C


# Brute-Force Matrix Multiplication used in Lab 1
def lab1MM(A, B):
    if (np.shape(A)[1] != np.shape(B)[0]):
        return -1
    Ar, Ac = np.shape(A)
    Br, Bc = np.shape(B)
    C = np.zeros((Ar, Bc))
    for row in range(Ar):
        for col in range(Bc):
            sum_ = 0
            for i in range(Ac):
                sum_ += A[row, i] * B[i, col]
            C[row, col] = sum_
    return C


def MM(A, B):
    if(np.shape(A)[1]!=np.shape(B)[0]):
        return -1
    C = np.zeros((np.shape(A)[0], np.shape(B)[1]))
    #return recursivematrixmultiply(A, B)
    return recursiveMatrixMultiply(A, B, 0, np.shape(A)[0], 0, np.shape(A)[1], 0, np.shape(B)[0], 0, np.shape(B)[1])

#prompt user for matrix sizes
ar = int(input("Enter A rows value\n"))
ac = int(input("Enter A columns value (also B rows value)\n"))
br = ar #int(input("Enter B rows value\n"))
bc = int(input("Enter B columns value\n"))

#generate random matrices for given matrix sizes
a = np.matrix(np.random.rand(ar,ac))
b = np.matrix(np.random.rand(br,bc))

# Run each method and record the time taken
startTime = time.time()
myP = MM(a, b)
myTime = time.time() -startTime
startTime = time.time()
npP = a*b
npTime = time.time()-startTime

#output time taken
if (np.array_equal(np.round(myP * 1000), np.round(npP * 1000))):
    print("Mine took: " + str(myTime) + " seconds")
    print("Numpy took: " + str(npTime) + " seconds")
else:
    print("Product was incorrect")


# run problem for matrix sizes 1x1 through n^nxn^n (128x128)
n = 1
myTimes = []
npTimes = []
sizes = []
for i in range(8):

    # Generate new matrices
    a = np.matrix(np.random.rand(n, n))
    b = np.matrix(np.random.rand(n, n))

    # Perform multiplication and record times
    startTime = time.time()
    myP = MM(a, b)
    myTime = time.time()-startTime
    startTime = time.time()
    npP = a * b
    npTime = time.time() - startTime
    if not np.array_equal(np.round(npP*1000), np.round(myP*1000)):
        continue
    myTimes.append(myTime)
    npTimes.append(npTime)
    sizes.append(n)
    print("finished size " + str(n))
    n = n * 2

    # output time taken
    if (np.array_equal(np.round(myP * 1000), np.round(npP * 1000))):
        print("Mine took: " + str(myTime) + " seconds")
        print("Numpy took: " + str(npTime) + " seconds")
    else:
        print("Product was incorrect")


# plot results
plt.plot(sizes, myTimes)
#plt.plot(sizes, npTimes)
plt.xlabel("n")
plt.ylabel("time(s)")
plt.title("My recursive Matrix Multiplication in python")
plt.show()
plt.figure()

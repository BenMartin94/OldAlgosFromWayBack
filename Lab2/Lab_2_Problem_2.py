import numpy as np
import time
import matplotlib.pyplot as plt
def recursivematrixmultiply(a, b):
    if (np.shape(a)[1] != np.shape(b)[0]):
        SystemExit()

    ar, ac = np.shape(a)
    br, bc = np.shape(b)
    C = np.matrix(np.zeros((ar, bc)))
    Cr = ar
    CC= bc
    if(np.size(a) == 1):
        C = a[0,0]*b
        return C
    elif(np.size(b)==1):
        C = a*b[0,0]
    elif(ar*ac*br*bc == 0):
        return C
    else:
        aSplitX = ar//2 # what value to divide by rows for
        aSplitY = ac//2 # the value to
        bSplitX = aSplitY
        bSplitY = aSplitX
        cSplitX, cSplitY = aSplitX, bSplitY
        A11 = a[0:aSplitX, 0:aSplitY]
        A12 = a[0:aSplitX, aSplitY:ac]
        A21 = a[aSplitX:ar, 0:aSplitY]
        A22 = a[aSplitX:ar, aSplitY:ac]
        B11 = b[0:bSplitX, 0:bSplitY]
        B12 = b[0:bSplitX, bSplitY:bc]
        B21 = b[bSplitX:br, 0:bSplitY]
        B22 = b[bSplitX:br, bSplitY:bc]

        C[0:cSplitX, 0:cSplitY] = recursivematrixmultiply(A11, B11)+recursivematrixmultiply(A12, B21)
        C[0:cSplitX, cSplitY:CC] = recursivematrixmultiply(A11, B12) + recursivematrixmultiply(A12, B22)
        C[cSplitX:Cr, 0:cSplitY] = recursivematrixmultiply(A21, B11) + recursivematrixmultiply(A22, B21)
        C[cSplitX:Cr, cSplitY:CC] = recursivematrixmultiply(A21, B12) + recursivematrixmultiply(A22, B22)
    return C

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
    return recursivematrixmultiply(A, B)

ar = int(input("Enter A rows value\n"))
ac = int(input("Enter A columns value\n"))
br = int(input("Enter B rows value\n"))
bc = int(input("Enter B columns value\n"))


a = np.matrix(np.random.rand(ar,ac))

b = np.matrix(np.random.rand(br,bc))

startTime = time.time()
myP = MM(a, b)
myTime = time.time() -startTime
startTime = time.time()
npP = a*b
npTime = time.time()-startTime


if(np.array_equal(np.round(myP*1000),np.round(npP*1000))):
    print("Mine took: " + str(myTime)+" seconds")
    print("Numpy took: " + str(npTime) + " seconds")
else:
    print("Product was incorrect")

n = 1
myTimes = []
npTimes = []
sizes = []
for i in range(8):
    a = np.matrix(np.random.rand(n, n))

    b = np.matrix(np.random.rand(n, n))

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

plt.plot(sizes, myTimes)
#plt.plot(sizes, npTimes)
plt.xlabel("n")
plt.ylabel("time(s)")
plt.title("My recursive Matrix Multiplication in python")
plt.show()
plt.figure()

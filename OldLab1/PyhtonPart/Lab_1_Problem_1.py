import numpy as np
import time
import matplotlib.pyplot as plt
'''
def matrixmultiply(Ar, Ac, Br, Bc):
    M1 = np.random.random((Ar,Ac))
    M2 = np.random.random((Br,Bc))
    if(Ac!=Br):
        return (-1,0)
    P = np.zeros((Ar, Bc))
    startTime = time.time()
    for row in range(Ar):
        for col in range(Bc):
            sum_ = 0
            for i in range(Ac):
                sum_ += M1[row, i]*M2[i, col]
            P[row, col] = sum_
    myTime = time.time()-startTime
    startTime = time.time()
    trueAns = np.matmul(M1, M2)
    npTime = time.time() - startTime
    if not np.array_equal(np.round(trueAns), np.round(P)):
        return (-2, 0)
    return (myTime, npTime)
'''
def matrixmultiply(A, B):
    if(np.shape(A)[1]!=np.shape(B)[0]):
        return -1
    Ar, Ac = np.shape(A)
    Br, Bc = np.shape(B)
    C = np.zeros((Ar,Bc))
    for row in range(Ar):
        for col in range(Bc):
            sum_ = 0
            for i in range(Ac):
                sum_ += A[row, i] * B[i, col]
            C[row, col] = sum_
    return C
n = int(input("Starting Sizes\n"))
A = np.random.rand(n,n)
B = np.random.rand(n,n)
startTime = time.time()
C = matrixmultiply(A, B)
myTime = time.time() - startTime
startTime = time.time()
trueAns = np.matmul(A, B)
numpyTime = time.time()-startTime
if np.array_equal(np.round(trueAns), np.round(C)):
    print("My MM took " + str(myTime)+ " seconds")
    print("Numpys MM took "+str(numpyTime)+ " seconds")
n = 4
myTimes = []
npTimes = []
sizes = []
for i in range(7):
    A = np.random.rand(n, n)
    B = np.random.rand(n, n)
    startTime = time.time()
    C = matrixmultiply(A, B)
    myTime = time.time() - startTime
    startTime = time.time()
    trueAns = np.matmul(A, B)
    numpyTime = time.time() - startTime
    if not np.array_equal(np.round(trueAns), np.round(C)):
        continue
    myTimes.append(myTime)
    npTimes.append(numpyTime)
    sizes.append(n)
    print("finished size " + str(n))
    n = n*2

plt.plot(sizes, myTimes)
#plt.plot(sizes, npTimes)
plt.xlabel("n")
plt.ylabel("time(s)")
plt.title("My Matrix Multiplication in python")
plt.show()
plt.figure()

n = 100
myTimes = []
npTimes = []
sizes = []
for i in range(7):
    A = np.random.rand(n, n)
    B = np.random.rand(n, n)
    #startTime = time.time()
    #C = matrixmultiply(A, B)
    #myTime = time.time() - startTime
    startTime = time.time()
    trueAns = np.matmul(A, B)
    numpyTime = time.time() - startTime

    myTimes.append(myTime)
    npTimes.append(numpyTime)
    sizes.append(n)
    print("finished size " + str(n))
    n = n*2

#plt.plot(sizes, myTimes)
plt.plot(sizes, npTimes)
plt.xlabel("n")
plt.ylabel("time(s)")
plt.title("Numpys Matrix Multiplication in python")
plt.show()
plt.figure()
import numpy as np
import matplotlib.pyplot as plt
import time
def insertionsort(A):
    dataType = A.dtype
    Asorted = np.copy(A)
    if not str(dataType) == "float64":
        return -1
    for i in range(1, len(A)):
        key = Asorted[i]
        j = i-1
        while j>=0 and Asorted[j]>key:
            Asorted[j+1] = Asorted[j]
            j-=1
        Asorted[j+1] = key
    return Asorted
def verifySorted(A):
    for i in range(1, len(A)):
        if(A[i]<A[i-1]):
            return False

    return True
n = int(input("Enter the size of the array to sort\n"))
A = np.random.random(n)
startTime = time.time()
mySort = insertionsort(A)
myTime = time.time()-startTime
if(verifySorted(mySort)):
    print("My sort took "+ str(myTime)+ " seconds")
trueSort = np.copy(A)
startTime = time.time()
trueSort = np.sort(trueSort)
numpyTime = time.time() - startTime
print("Numpy sort took " + str(numpyTime)+ " seconds")

### data stuff now

myTimes = []
npTimes = []
sizes = []
n = 200
for i in range(7):
    A = np.random.random(n)
    startTime = time.time()
    mySort = insertionsort(A)
    myTime = time.time()-startTime
    if not(verifySorted(mySort)):
        continue
    myTimes.append(myTime)
    startTime = time.time()
    numpySort = np.sort(A)
    npTimes.append(time.time()-startTime)
    sizes.append(n)
    print("finished size " + str(n))

    n=n*2

plt.plot(sizes, myTimes)
#plt.plot(sizes, npTimes)
plt.xlabel("n")
plt.ylabel("time(s)")
plt.title("My insertion sort in python")
plt.show()

myTimes = []
npTimes = []
sizes = []
n = 2000
for i in range(7):
    A = np.random.random(n)
    startTime = time.time()
    #mySort = insertionsort(A)
    #myTime = time.time()-startTime
    myTimes.append(myTime)
    startTime = time.time()
    numpySort = np.sort(A)
    npTimes.append(time.time()-startTime)
    sizes.append(n)
    print("finished size " + str(n))

    n=n*2

#plt.plot(sizes, myTimes)
plt.plot(sizes, npTimes)
plt.xlabel("n")
plt.ylabel("time(s)")
plt.title("Numpy's sort in python")
plt.show()
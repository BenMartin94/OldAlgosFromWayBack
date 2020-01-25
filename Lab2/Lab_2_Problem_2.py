import numpy as np
def recursivematrixmultiply(C, a, b):
    if (np.size(a)[1] != np.size(a)[0]):
        SystemExit()

    ar, ac = np.size(a)
    br, bc = np.size(b)
    if(ar*ac*br*bc == 1):
        C = a[0,0]*b[0,0]
    aSplitX = ar//2 # what value to divide by rows for
    aSplitY = ac//2 # the value to
    bSplitX = aSplitY
    bSplitY = aSplitX
    cSplitX, cSplitY = aSplitX, bSplitY
    A11 = a[0:aSplitX, 0:aSplitY]
    A12 = a[0:aSplitX, aSplitY:ac]
    A21 = a[aSplitX:ar, 0:aSplitY]
    A22 = a[aSplitX:ar, aSplitY:ac]
    B11 =



def MM(A, B):
    if(np.size(A)[1]!=np.size(B)[0]):
        return -1
    C = np.zeros((np.size(A)[0], np.size(B)[1]))


a = np.random.rand(3,3)

b=1
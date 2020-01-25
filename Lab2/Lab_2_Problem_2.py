import numpy as np
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



def MM(A, B):
    if(np.shape(A)[1]!=np.shape(B)[0]):
        return -1
    C = np.zeros((np.shape(A)[0], np.shape(B)[1]))
    return recursivematrixmultiply(A, B)

a = np.random.rand(4,4)
a = np.matrix(np.random.rand(13,41))
b = np.random.rand(4,4)
b = np.matrix(np.random.rand(41,11))
print(MM(a, b))
print(a*b);
print(np.array_equal(np.round(MM(a, b)),np.round(a*b)))
b=1
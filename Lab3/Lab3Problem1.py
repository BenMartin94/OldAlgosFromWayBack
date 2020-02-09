


# bad top down recursion to find and return given nth fibonacci number.
def recursiveFibonacci(nth):

    if (nth <=2 ):
        return 1
    else:
        return (recursiveFibonacci(nth-1) + recursiveFibonacci(nth-2))
#recursiveFibonacci



# memoized recursive Fibonacci method wrapper method
def memoizedFibonacci(nth):
    if (nth <=2):
        return 1
    else:
        nMinus1, nMinus2 =  memoizedFibonacci2(nth-1)
        return (nMinus1 + nMinus2)



# memoized recursive Fibonacci method. Calculates n-1, and 
# returns n-1 and n-2 to prevent redundant calculations
def memoizedFibonacci2(nth):
    if (nth == 2):
        return 1, 1
    elif (nth == 3):
        nMinus2 , nMinus3 = memoizedFibonacci2(2)
        nMinus3 = 1
    else:
        nMinus2, nMinus3 = memoizedFibonacci2(nth-1)
    
    nMinus1 = nMinus2 + nMinus3
    return nMinus1, nMinus2



for i in range(1,15):
    print(i,  " : ",  recursiveFibonacci(i))
    print(i,  " : ",  memoizedFibonacci(i))
    


print("\n Program finished sucessfully")


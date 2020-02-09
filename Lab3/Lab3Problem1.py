


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
#memoizedFibonacci



# memoized recursive Fibonacci method. Calculates nth value, and 
# returns n value and n-1 value to prevent redundant calculations
def memoizedFibonacci2(nth):
    if (nth == 2):
        nMinus1 = 1
        nMinus2 = 0
    else:
        nMinus1, nMinus2 = memoizedFibonacci2(nth-1)
    
    nVal = nMinus1 + nMinus2
    return nVal, nMinus1
#memoizedFibonacci2



# returns value of given nth fibonacci number
def bottomupFibonacci(nth):
    current = 1      # highest calculated fibonacci number
    currentVal = 1   # value of highest calculated number
    lastVal = 0      # value of the current-1 number

    # calculation loop
    while current < nth:
        holder = currentVal
        currentVal = currentVal + lastVal
        lastVal = holder
        current = current +1

    return currentVal
#bottomupFibonacci    






for i in range(1,15):
    print(i, ":", recursiveFibonacci(i), memoizedFibonacci(i), bottomupFibonacci(i))
    


print("\n Program finished sucessfully")


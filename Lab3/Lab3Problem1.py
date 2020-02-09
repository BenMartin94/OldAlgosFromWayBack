


# bad top down recursion to find and return given nth fibonacci number.
def recursiveFibonacci(nth):

    if (nth <=2 ):
        return 1

    else:
        return (recursiveFibonacci(nth-1) + recursiveFibonacci(nth-2))
#recursiveFibonacci



for i in range(1,15):
    print(i,  " : ",  recursiveFibonacci(i))



print("\n Program finished sucessfully")


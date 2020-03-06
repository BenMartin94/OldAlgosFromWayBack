import numpy as np
import sys
import math
import copy
import operator  # for sorting lists of classes by attribute
from timeit import default_timer as timer
from datetime import timedelta


class ItemOfValue:
    """An item of value"""
    # class representing an item with value, weight and per-unit value
    value = -1
    weight = -1
    per_unit_value = -1

    def __init__(self, value, weight):
        self.value = int(value);
        self.weight = int(weight);
        self.per_unit_value = float(value) / float(weight)

    def __repr__(self):
        return str("Item of Value") + str(self.__dict__)

    def __str__(self):
        return str("Item of Value") + str(self.__dict__)


def RecursiveGreedyKnapsack(items, k, n, remaining_weight, knapsack):
    """ Greedy 0-1 Knapsack using Recursion

        We assume that items are sorted by decreasing per-unit-value

        """

    m = k  # we start looking at item k, n is the total number of items

    # move through items until we find one that fits in the bag
    while (m < n and items[m].weight > remaining_weight):
        m += 1

    # as long as we haven't gone off the end of the list,
    # add the item to the bag
    if m < n:
        knapsack.append(items[m])
        RecursiveGreedyKnapsack(items, m + 1, n, remaining_weight - items[m].weight, knapsack)


def DynamicKnapsack(items, W):
    """Dynamic Knapsack Problem Point of Entry to recursive function"""
    knapsack_value = []
    n = len(items)
    c = np.zeros(W) - 1  # optimal values of all problems (integer weights W)
    knapsack_value = DyanmicKnapsackRecursive(items, W, c)  # make the recursive call
    return knapsack_value


def DyanmicKnapsackRecursive(items, W, c):
    """Dynamic Knapsack Problem Recursive Call"""
    if (c[W - 1] >= 0):
        return c[W - 1]
    else:
        n = len(items)
        for k in range(0, n):  # for all remaining items
            new_weight = W - items[k].weight  # if we add this item the new weight would be this
            if (new_weight >= 0):  # if the item could be added
                new_items = copy.copy(items)  # (not at all efficient) create an updated list
                del new_items[k]  # by removing item k

                # recursive call
                c[W - 1] = max(c[W - 1], items[k].value + DyanmicKnapsackRecursive(new_items, new_weight, c))
        # c[W-1] = max(c[W-1],0) #create the memo from the maximum value
    return c[W - 1]

def bottomUp(items, W):
    c = np.zeros((W))-1
    for p in range(0, W+1):
        n = len(items)
        for k in range(0,n):
            new_weight = p-items[k].weight
            if(new_weight>=0):
                new_items = copy.copy(items)  # (not at all efficient) create an updated list
                del new_items[k]
                c[W-1] = max(c[W-1], items[k].value+c[p-new_weight])

    return c[W-1]


def getknapsackinfo(knapsack):
    """ Get weight and value of a 0-1 knapsack of ItemOfvalue entitites"""

    n = len(knapsack)
    weight = 0
    value = 0

    for i in range(0, n):
        weight += knapsack[i].weight
        value += knapsack[i].value

    per_unit_value = value / weight

    return n, weight, value, per_unit_value


# main point of entry
num_items = 20
item_set = [];
knapsack_max_weight = 500  # must be an integer

# create a list of random items
for i in range(0, num_items):
    item_value = float(np.random.randint(1, 100, 1))
    item_weight = float(np.random.randint(1, 100, 1))
    item_set.append(ItemOfValue(item_value, item_weight))

# sort the list by per-unit-value
item_set.sort(key=operator.attrgetter('per_unit_value'), reverse=True)

# display the list of items
# for i in item_set:
#    print(i)
# run and time the recursive greedy approach
print(DynamicKnapsack(item_set, knapsack_max_weight))
print(bottomUp(item_set, knapsack_max_weight))
start_time = timer()
greedy_knapsack = [];  # start with an empty knapsack
RecursiveGreedyKnapsack(item_set, 0, num_items, knapsack_max_weight, greedy_knapsack)
end_time = timer();
time_greedy = timedelta(seconds=end_time - start_time)
print("Recursive Greedy Knapsack time =", time_greedy)
n_items_in_knapsack, knapsack_weight, knapsack_value, knapsack_per_unit_value = getknapsackinfo(greedy_knapsack)
print("Greedy Knapsack has", n_items_in_knapsack, "items valued at", knapsack_value, "at a total weight of",
      knapsack_weight)

# run and time the dynamic programming approach
start_time = timer()
dynamic_knapsack = [];  # start with an empty knapsack
dynamic_knapsack_value = DynamicKnapsack(item_set, knapsack_max_weight)
end_time = timer();
time_dynamic = timedelta(seconds=end_time - start_time)
print("Dynamic Knapsack time =", time_dynamic)
print("Dynamic Knapsack has value", dynamic_knapsack_value)


import numpy as np
import sys
import math
import copy
import heapq
import operator #for sorting lists of classes by attribute
import bitstring
from timeit import default_timer as timer
from datetime import timedelta

class HuffmanNode:
    """Huffman encoding tree node"""
    
    character = -1      #the character represented
    index = -1          #index of character (integer)
    count = -1          #character frequency (count) in the file
    left = []           #left child node
    right = []          #right child node
    code = bitstring.BitString()    #bitstring code for the character
    
    #constructor
    def __init__(self,character):
        self.character = character;
        self.index = int.from_bytes(self.character, sys.byteorder)
        self.count = 0
    
    #for printing
    def __repr__(self):
        return str("Huffman Node") + str(self.__dict__)
        
    #for printing
    def __str__(self):
        return str("Huffman Node") + str(self.__dict__)
        
    #comparison operator required for heapq comparison
    def __lt__(self, other):
        return self.count < other.count

    #print function (non-recursive)
    def print(self):
        print("Huffman Node: ")
        print("\tCharacter = ", self.character)
        print("\tIndex = ", self.index)
        print("\tCount = ", self.count)
        has_left = (self.left != [])
        has_right = (self.right != [])
        print("\tHas left child = ", has_left)
        print("\tHas right child = ", has_right)
        print("\tCode = ", self.code)


def getfilecharactercounts(filename):
    """ Read a file and count characters """

    f = open(filename,"rb")
    nodes = [];
    
    #for every character of interest (and then some) create a huffman node
    for i in range(0,256):
        nodes.append(HuffmanNode(bytes([i]))) #works in python 3
    
    #loop over file reading a character at a time and increment the count
    #of that character in the list of huffman nodes
    while True:
        c = f.read(1)
        if c:
            index = int.from_bytes(c, sys.byteorder)
            nodes[index].count += 1
        else:
            #print("End of file")
            break

    f.close()
    
    #Code Missing: mark and delete any characters that don't occur in the file
    #i.e., nodes should be as long as the number of unique characters in the file (not 256 things long)
    #Hint: Eliminate zero counts, sorting may help.


    return nodes


def createhuffmantree(huffmannodes):
    """ Create the huffman tree
        Using heapq functionality to build the tree from a priority queue"""

    node_heap = copy.deepcopy(huffmannodes)  #first create a copy
    heapq.heapify(node_heap)                 #create heap

    #Code Missing: Create the Huffman Node Tree using the Min Priority Queue (heap)

    return heapq.heappop(node_heap) #final node is the tree we want


def codehuffmantree(huffmantreenode, nodecode):
    """ Traverse Huffman Tree to produce Prefix Codes"""
    #huffmantreenode.print()
    #print("Nodecode = ", nodecode)
    
    if (huffmantreenode.left == [] and huffmantreenode.right == []):
        huffmantreenode.code = nodecode     #no children - assign code
    else:
        leftcode = copy.copy(nodecode)      #append 0 to left
        leftcode.append(bitstring.Bits('0b0'))
        codehuffmantree(huffmantreenode.left,leftcode)
        rightcode = copy.copy(nodecode)     #append 1 to right
        rightcode.append(bitstring.Bits('0b1'))
        codehuffmantree(huffmantreenode.right,rightcode)


def listhuffmancodes(huffmantreenode, codelist):
    """ Create a list of Prefix Codes from the Huffman Tree"""
    if (huffmantreenode.left == [] and huffmantreenode.right == []):
        codelist[huffmantreenode.index] = huffmantreenode.code
    else:
        listhuffmancodes(huffmantreenode.left,codelist)
        listhuffmancodes(huffmantreenode.right,codelist)


def huffmanencodefile(filename):
    """ Read and Encode a File using Huffman Codes"""
        
    counts = getfilecharactercounts(filename) #get the counts from the file
        
    huffmantree = createhuffmantree(counts) #create and encode the characters
    codehuffmantree(huffmantree,bitstring.BitString())

    codelist = [None]*256
    listhuffmancodes(huffmantree, codelist) #get the codes for each character

    for i in range(0,256):
        if codelist[i] != None:
            print("character ", chr(i), " maps to code ", codelist[i].bin)
    
    
    #encode the file
    with open(filename, 'rb') as f:
        filecode = bitstring.BitString()
        while True:
            c = f.read(1)
            index = int.from_bytes(c, sys.byteorder)
            if c:
                filecode.append(codelist[index])
            else:
                break #eof


    #write the file
    with open(filename + ".huf", 'wb') as coded_file:
        #Code Missing: Write the bitstring (and any additional information necessary) to file


def huffmandecodefile(filename):
    """ Decode a Huffman-Coded File"""
    #Code Missing:

#main
filename="./LoremIpsumLong.rtf"
huffmanencodefile(filename)

#huffmandecodefile(filename + ".huf") #uncomment once this file is written






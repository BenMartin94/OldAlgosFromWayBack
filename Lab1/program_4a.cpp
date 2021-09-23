#include <iostream>
#include <mpi.h>                    //Use C-version of MPI, include mpi++.h for C++ bindings.
#include <math.h>
#include <vector>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv)
{
    int rank = 0;
    int nproc = 1;
    int namelength;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);                                 //Initialize MPI
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);                  //Get the number of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);                   //Get my rank

    /*
    MPI_Comm_split is the new function that I am calling.

    MPI_Comm_split(Group, colour, key, *newgroup)

    It will take a given comm group and split it into different sections
    based on the colour value that is given. The new ordering
    is based on the key value that is given. The lowest key value of
    a given group will get rank 0

    It is important that every processor in a group that is being
    split calls the split function or execution just hangs... ask
    me how i know that :/
    */

    if(rank != 0){
        MPI_Comm newGroup;
        int newRank;
        MPI_Comm_split(MPI_COMM_WORLD, 10, rank, &newGroup);
        MPI_Comm_rank(newGroup, &newRank);
        
        cout << "I was rank " << rank << " but now I am rank " << newRank << " in the new comm group" << endl;
        MPI_Comm_free(&newGroup);
    }
    else{
        MPI_Comm newGroup;
        int newRank;
        MPI_Comm_split(MPI_COMM_WORLD, 9, rank, &newGroup);
        MPI_Comm_rank(newGroup, &newRank);
        
        cout << "I was rank " << rank << " but now I am rank " << newRank << " in a very special comm group" << endl;
        MPI_Comm_free(&newGroup);
    }
    
    MPI_Finalize();

    return 0;
}
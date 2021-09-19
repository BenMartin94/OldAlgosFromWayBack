#include <iostream>
#include <mpi.h>					                    //Use C-version of MPI, include mpi++.h for C++ bindings.

#include <string.h>

using namespace std;

int main(int argc, char** argv)
{
	int rank = 0;
	int nproc = 1;
	int namelength;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Request request;                                    //Might be important...

    int work = 0;

	MPI_Init(&argc, &argv);					                //Initialize MPI
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);			        //Get the number of processors
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);			        //Get my rank
	MPI_Get_processor_name(processor_name, &namelength);	//Get processor name

    if(rank == 0) 
    {
        std::cout << "Number of processors is " << nproc << std::endl;
        std::cout << "Node " << rank << " is processor " << processor_name << std::endl;
        MPI_Request foo2;
        char host[MPI_MAX_PROCESSOR_NAME];
        for (int irank = 1; irank < nproc; irank++)
        {
            MPI_Irecv(host, MPI_MAX_PROCESSOR_NAME, MPI_BYTE, irank, 0, MPI_COMM_WORLD, &foo2);
            //Receive name from processor with rank irank - store in "host"
            MPI_Wait(&foo2, MPI_STATUS_IGNORE);

            std::cout << "Node " << irank << " is processor " << host << std::endl;
        }
        std::cout << work << std::endl;
    }
    else
    {
        MPI_Request foo;
        //Send processor_name to processor 0
        MPI_Isend(processor_name, MPI_MAX_PROCESSOR_NAME, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &foo);
    }
	MPI_Finalize();

	return 0;
}

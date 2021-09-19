#include <iostream>
#ifdef HAVEMPI
#include <mpi.h>						                    //Use C-version of MPI, include mpi++.h for C++ bindings.
#endif

using namespace std;

int main(int argc, char** argv)
{
	int rank = 0;
	int nproc = 1;
	int namelength;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);					                //Initialize MPI
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);			        //Get the number of processors
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);			        //Get my rank
	MPI_Get_processor_name(processor_name, &namelength);	//Get processor name

	//Output
	std::cout << "Hello, my name is " << processor_name << " and I am number " << rank << " of " << nproc << std::endl;

	MPI_Finalize();

	return 0;
}

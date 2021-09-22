#include <iostream>
#include <mpi.h>						                    //Use C-version of MPI, include mpi++.h for C++ bindings.
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char** argv)
{
	int rank = 0;
	int nproc = 1;
	int namelength;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

    int work = 0;

	MPI_Init(&argc, &argv);					                //Initialize MPI
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);			        //Get the number of processors
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);			        //Get my rank
	MPI_Get_processor_name(processor_name, &namelength);	//Get processor name

    ofstream out_to_file;                                   //You'll need an include for this

	out_to_file.open(to_string(rank) + "of" + to_string(nproc) + "processes");
    
    //Open the output file HERE using a unique name based on rank and nproc
    //Hint: you can use ostringstream to produce the filename as a combination of
    //numbers and trings pretty easily (you'll need an include)

    out_to_file << "My name is " << processor_name << " and I am processor " << rank << " of " << nproc << std::endl;

    out_to_file.close();

    MPI_Finalize();

	return 0;
}

#include <iostream>
#include <mpi.h>                    //Use C-version of MPI, include mpi++.h for C++ bindings.
#include <math.h>
#include <vector>
#include <stdlib.h>

using namespace std;

void generate_randoms(vector<double>*, double);             //Prototypes for helper functions
vector<double> subvec(vector<double>, int, int);

int main(int argc, char** argv)
{
    int rank = 0;
    int nproc = 1;
    int namelength;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);                                 //Initialize MPI
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);                  //Get the number of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);                   //Get my rank
    MPI_Get_processor_name(processor_name, &namelength);    //Get processor name

    int nvalues_per_proc = 0;

    double local_sum = 0;
    double total_sum_reduce = 0;
	double total_sum_manual = 0;

    if (rank == 0)
    {
		
        std::cout << "Enter the number of random values to be summed by each processor:" << std::endl;
        std::cin >> nvalues_per_proc;
		
        double time1, time2;
       
        srand(clock());										//Seed RNG

        vector<double> full_list(nvalues_per_proc*nproc);
        
        //Generate and distribute the full_list of values to all other processors
        //REMEMBER: the other processors don't know what nvalues_per_proc is

        //Once all the processors have calculated their local sums, use MPI_REDUCE to generate a total sum on rank 0
        
        //Receive all partial sums from the other processors
        //Produce the total sum from the partial sums locally
        //Compare to the sum of MPI_Reduce
        double partial_sum;
        for(irank = 1; irank < nproc; irank++)
        {
            std::cout << "Partial Sum on Processor " << irank << ": " << partial_sum << std::endl;
        }

        std::cout << "Total Sum on all Processors (MPI_Reduce): " << total_sum_reduce << std::endl;
		std::cout << "Total Sum on all Processors (Manual sum): " << total_sum_manual << std::endl;

        std::cout << "Total time to compute: " << time2-time1 << " seconds" << std::endl;
    }
    else
    {
        //Collect the subvector from rank 0
        //Calculate the local sum
        //Use MPI_REDUCE to contribute to the total sum on rank 0
        //Send the partial sum to rank 0 for displaying and verifying
    }

    MPI_Finalize();

    return 0;

}

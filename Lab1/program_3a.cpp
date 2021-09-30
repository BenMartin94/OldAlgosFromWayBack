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
		
        double time1Serial, time2Serial;
        double time1Parallel, time2Parallel;
    
       
        srand(clock());										//Seed RNG

        vector<double> full_list(nvalues_per_proc*nproc);
        generate_randoms(&full_list, nvalues_per_proc*nproc);
        
        time1Parallel = MPI_Wtime();
        for(int irank = 1; irank<nproc; irank++){
            vector<double> toSend = subvec(full_list, (irank) * nvalues_per_proc, (irank+1) * nvalues_per_proc);
            MPI_Send(&toSend[0], nvalues_per_proc, MPI_DOUBLE, irank, 9, MPI_COMM_WORLD);
        }
        //Generate and distribute the full_list of values to all other processors
        //REMEMBER: the other processors don't know what nvalues_per_proc is

        //Once all the processors have calculated their local sums, use MPI_REDUCE to generate a total sum on rank 0
        
        //Receive all partial sums from the other processors
        //Produce the total sum from the partial sums locally
        //Compare to the sum of MPI_Reduce
        double partial_sum = 0;
        double myPartialSum=0;
        // now compute the masters part since this is a fair world :)
        for(int i = 0; i<nvalues_per_proc;i++){
            partial_sum += full_list[i];
        }
        myPartialSum = partial_sum;
        for(int irank = 0; irank < nproc; irank++)
        {
            if(irank != 0)
                MPI_Recv(&partial_sum, 1, MPI_DOUBLE, irank, 9, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            std::cout << "Partial Sum on Processor " << irank << ": " << partial_sum << std::endl;
            partial_sum = 0;
        }

        double total_sum_reduce;
        MPI_Reduce(&myPartialSum, &total_sum_reduce, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


        std::cout << "Total Sum on all Processors (MPI_Reduce): " << total_sum_reduce << std::endl;

        time2Parallel = MPI_Wtime();

        time1Serial = MPI_Wtime();
        double total_sum_manual = 0;
        for(int i = 0;i<full_list.size(); i++){
            //cout << full_list[i] << endl;
            total_sum_manual += full_list[i];
        }
        time2Serial = MPI_Wtime();

		std::cout << "Total Sum on all Processors (Manual sum): " << total_sum_manual << std::endl;
        
        std::cout << "Total time to compute in parallel: " << time2Parallel-time1Parallel << " seconds" << std::endl;
        std::cout << "Total time to compute in serial: " << time2Serial-time1Serial << " seconds" << std::endl;
        std::cout << "Speedup Factor: " << (time2Serial-time1Serial)/(time2Parallel-time1Parallel) << std::endl;

    }
    else
    {
        MPI_Status status;
		MPI_Probe(0,9,MPI_COMM_WORLD,&status);
        int numValuesToSum;

        MPI_Get_count(&status, MPI_DOUBLE, &numValuesToSum);

        vector<double> myPortion(numValuesToSum);

        MPI_Recv(&myPortion[0], numValuesToSum, MPI_DOUBLE, 0, 9, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	

        double sum = 0;
        for(int i = 0;i<myPortion.size(); i++){
            sum += myPortion[i];
        }

        MPI_Send(&sum, 1, MPI_DOUBLE, 0, 9, MPI_COMM_WORLD);
        MPI_Reduce(&sum, NULL, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


        //Collect the subvector from rank 0
        //Calculate the local sum
        //Use MPI_REDUCE to contribute to the total sum on rank 0
        //Send the partial sum to rank 0 for displaying and verifying
        
    }

    MPI_Finalize();

    return 0;

}

void generate_randoms(vector<double> *list, double num_rands){
    for(int i = 0;i<num_rands; i++){
        list->at(i) = rand();
    }
}

vector<double> subvec(vector<double> list, int slice_start, int slice_end){
    vector<double>::iterator first = list.begin() + slice_start;
    vector<double>::iterator last = list.begin() + slice_end;
    vector<double> subvector(first, last);
    return subvector;
}

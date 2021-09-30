#include <iostream>
#include <mpi.h> //Use C-version of MPI, include mpi++.h for C++ bindings.
#include <math.h>
#include <vector>
#include <stdlib.h>
 
using namespace std;
 
void generate_randoms(vector<double>*, double); //Prototypes for helper functions
vector<double> subvec(vector<double>, int, int);
 
int main(int argc, char** argv)
{
 int rank = 0;
 int nproc = 1;
 int namelength;
 char processor_name[MPI_MAX_PROCESSOR_NAME];
 
 MPI_Init(&argc, &argv); //Initialize MPI
 MPI_Comm_size(MPI_COMM_WORLD, &nproc); //Get the number of processors
 MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Get my rank
 MPI_Get_processor_name(processor_name, &namelength); //Get processor name
 
 int nvalues_per_proc = 0;
 
 double local_sum = 0;
 double total_sum_reduce = 0;
 double total_sum_manual = 0;
 
 if (rank == 0)
 {
 
 std::cout << "Enter the number of random values to be summed by each processor:" << std::endl;
 std::cin >> nvalues_per_proc;
 
 double time1, time2;
 
 srand(clock()); //Seed RNG
 
 int full_length = nvalues_per_proc*nproc;
 vector<double> full_list(full_length);
 
 //Generate and distribute the full_list of values to all other processors
 
 //Generate random list of values
 for (int i=0; i<full_length; i++){
 full_list[i] = rand();
 }
 //REMEMBER: the other processors don't know what nvalues_per_proc is
 //Send number of values expected to each process
 MPI_Bcast(&nvalues_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
 
 //Send segment of list to each process
 for (int irank=1; irank < nproc; irank++){
 int pos = (irank-1)*nvalues_per_proc;
 vector<double> toSend = {&full_list[pos], &full_list[pos+nvalues_per_proc]};
 
 MPI_Send(&toSend[0], nvalues_per_proc, MPI_DOUBLE, irank, 0, MPI_COMM_WORLD);
 cout << "sent" <<endl;
 } 
 
 //Once all the processors have calculated their local sums, use MPI_REDUCE to generate a total sum on rank 0
 
 //Receive all partial sums from the other processors
 //Produce the total sum from the partial sums locally
 //Compare to the sum of MPI_Reduce
 double partial_sum;
 MPI_Status stat;
 
 // for(int irank = 1; irank < nproc; irank++)
 // {
 // MPI_Recv(partial_sum, 1, MPI_INT, irank, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
 // std::cout << "Partial Sum on Processor " << irank << ": " << partial_sum << std::endl;
 // total_sum_manual += partial_sum;
 // }
 
 // std::cout << "Total Sum on all Processors (MPI_Reduce): " << total_sum_reduce << std::endl;
 //std::cout << "Total Sum on all Processors (Manual sum): " << total_sum_manual << std::endl;
 
 // std::cout << "Total time to compute: " << time2-time1 << " seconds" << std::endl;
 }
 else //non-root processes
 {
 
 MPI_Status stat;
 
 //receive int of list segment length (nvalues_per_proc)
 MPI_Bcast(&nvalues_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
 std::cout << "SegLen Rxd: " << nvalues_per_proc <<std::endl;
 
 vector<double> segment(nvalues_per_proc); //segment of fullvector to sum
 //Collect the subvector from rank 0
 
 MPI_Recv(&segment[0], nvalues_per_proc, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
 //Calculate the local sum
 //Use MPI_REDUCE to contribute to the total sum on rank 0
 std::cout << "segment rxd." << std::endl;
 //Send the partial sum to rank 0 for displaying and verifying
 }
 int summ;
 int one =1;
 cout << "gonna reduce: " << rank <<endl;
 MPI_Reduce(&one, &summ, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
 if (rank==0){
 std::cout << "final sum: " << summ << std::endl;
 }
 MPI_Finalize();
 
 return 0;
 
}
#include "Utilities.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;


void dumpVectorContents(string name, vector<double> vecToDump){
    ofstream outToFile;
    outToFile.open(name);
    for(int i = 0;i<vecToDump.size();i++){
        outToFile << to_string(vecToDump[i]) << std::endl;
    }
    outToFile.close();
}

template<class T>
vector<T> bucketSort(int a, int b, int rank, int p, vector<T> numsToSort){
    vector<vector<T>> smallBuckets = vector<vector<T>>();
    vector<vector<T>> myBigBucket = vector<vector<T>>();
    for(int i = 0; i<p; i++){
        smallBuckets.push_back(vector<T>());
        
    }
    double delta = (b-a)/double(p);
    for(int i = 0; i<numsToSort.size(); i++){
        // the bucket to go into is
        // floor(numsToSort[i] / delta)
        smallBuckets[(int)floor(numsToSort[i]/delta)].push_back(numsToSort[i]);
        
    }
    MPI_Alltoall_vecvecT(smallBuckets, myBigBucket);

    
    // Next just make the big bucket a single vector and sort it
    vector<T> linearBigBucket = vector<T>();
    for(int bucket = 0; bucket<myBigBucket.size(); bucket++){
        for(int i = 0; i<myBigBucket[bucket].size(); i++){
            
            linearBigBucket.push_back(myBigBucket[bucket][i]);
            
        }
    }
    std::sort(linearBigBucket.begin(), linearBigBucket.begin()+linearBigBucket.size());
    //dumpVectorContents("Rank" + to_string(rank), linearBigBucket);

    return linearBigBucket;
}

template<class T>
T getAt(int m, vector<T>localList){
    int rank = 0;
    int nproc = 1;

    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    

    // lets find out if i have the mth value
    int myListLength = localList.size();
    int listLengths[nproc];
    MPI_Gather(&myListLength, 1, MPI_INT, &listLengths[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int indexesChecked = -1;
    int rankThatHasM = -1;
    if(rank == 0){
        for(int i = 0; i<nproc; i++){
            // loop over the lengths of the list and do some math
            // to see if the ith processor has the mth index
            if(m<=indexesChecked + listLengths[i]){
                // so m is the ith processor
                rankThatHasM = i;
                i = nproc;
            }
            else{
                indexesChecked += listLengths[i];
            }
        }
        assert(rankThatHasM>=0);
    }
    // if it isnt, the index is prob out of bounds
    
    
    // now broadcast who has m
    MPI_Bcast(&rankThatHasM, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //int rankThatHasM = m/localList.size();
    T val;
    if(rankThatHasM == rank){
        // we have it, so lets broadcast it
        int localM = m%localList.size();
        val = localList[localM];
        //MPI_Bcast(&val, sizeof(T), MPI_BYTE, rankThatHasM, MPI_COMM_WORLD)
    }
    else{
        T val;
        //MPI_Bcast(&val, sizeof(T), MPI_BYTE, rankThatHasM, MPI_COMM_WORLD);
    }

    MPI_Bcast(&val, sizeof(T), MPI_BYTE, rankThatHasM, MPI_COMM_WORLD);
    return val;

}
 

int main(int argc, char** argv){
    int rank = 0;
    int nproc = 1;
    int a = 0;
    int b = 100;
    assert(argc == 2);
    int M = atoi(argv[1]);
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double startTime = 0;
    double endTime = 0;

    if(rank == 0){
        startTime = MPI_Wtime();
    }
    assert(rank>=0);
    srand(rank+10);
    vector<double> randNums = std::vector<double>();
    
    for(int i = 0; i<M/nproc; i++){
        double randNum = b*(double)rand()/(double)RAND_MAX;
        
        randNums.push_back(randNum);
    }
    vector<double> sortedList = bucketSort(a, b, rank, nproc, randNums);
    dumpVectorContents("Rank" + to_string(rank), sortedList);

    if(rank == 0){
        endTime = MPI_Wtime();
    }
    if(rank == 0)
        std::cout << "Sorting took " << endTime - startTime << " seconds" <<std::endl;
    double myVal = getAt(23, sortedList);
    std::cout<<"Rank " << rank << " got the value " << myVal<<std::endl;
    MPI_Finalize();
    return 0;
}
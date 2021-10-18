#include "Utilities.h"
#include <fstream>
#include <sstream>

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
        for(int m = 0; m<p; m++){
            if(numsToSort[i]>=a+m*delta && numsToSort[i] < a+(m+1)*delta){
                
                smallBuckets[m].push_back(numsToSort[i]);
            }
        }
    }
    for(int bucket = 0; bucket<smallBuckets.size(); bucket++){
        dumpVectorContents("PRE-Rank" + to_string(rank) + "bucket#" + to_string(bucket), smallBuckets[bucket]);
    }
    MPI_Alltoall_vecvecT(smallBuckets, myBigBucket);
    std::cout<<"Dumping rank " << rank << " which has " << myBigBucket[rank].size() << " numbers" <<std::endl;
    for(int bucket = 0; bucket<myBigBucket.size(); bucket++){
        dumpVectorContents("Rank" + to_string(rank) + "bucket#" + to_string(bucket), myBigBucket[bucket]);
    }
    
    // Next just make the big bucket a single vector and sort it

    return numsToSort;
}

int main(int argc, char** argv){
    int rank = 0;
    int nproc = 1;
    int a = 0;
    int b = 100;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    assert(rank>=0);
    srand(rank+10);
    vector<double> randNums = std::vector<double>(100);
    
    for(int i = 0; i<100; i++){
        double randNum = b*(double)rand()/(double)RAND_MAX;
        
        randNums[i]=randNum;
    }
    bucketSort(a, b, rank, nproc, randNums);
    MPI_Finalize();
    return 0;
}
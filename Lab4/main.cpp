#include <iostream>
#include <opencv4/opencv2/core/core.hpp>      //changes may be required to opencv includes
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <mpi.h>

void parallelRange(int globalStart, int globalStop, int irank, int nproc, int &localStart, int &localStop, int &localCount){
    int nrows = globalStop - globalStart;
    int divisor = nrows/nproc;
    int remainder = nrows%nproc;
    int offset;
    if(irank<remainder) offset = irank;
    else offset = remainder;
    
    localStart = irank*divisor + globalStart + offset;
    localStop = localStart + divisor - 1;
    if (remainder>irank) localStop += 1;
    localCount = localStop-localStart+1;
}

// Performs the rules of brians brain
void iterateBrain(cv::Mat &region, int start, int end){ // end is not inclusive
    assert(start == 1 || start == 0);
    assert(end == region.rows-1 || end == region.rows);
    cv::Mat oldRegion = region.clone();
    for(int row=start; row<end; row++){
        for(int col = 0; col<region.cols; col++){
            cv::Vec3b cell = oldRegion.at<cv::Vec3b>(row, col);
            if(cell.val[0] == 255){
                // it is on, so now it need to go to dying state
                cell.val[0] = 55;
                cell.val[1] = 55;
                cell.val[2] = 55;
            }
            else if(cell.val[2] == 55){
                // it was dying, so now kill it
                cell.val[0] = 0;
                cell.val[1] = 0;
                cell.val[2] = 0;
            }
            else{
                int liveCells = 0;
                for(int nRow = -1; nRow<=1; nRow++){
                    for(int nCol = - 1; nCol<=1; nCol++){
                        int neighborRow = row + nRow;
                        if(neighborRow < 0 || neighborRow >= region.rows) continue; // the neighbor is outside the domain

                        int neighborCol = col + nCol;
                        if(neighborCol < 0 || neighborCol >= region.cols) continue; // the neighbor is outside the domain

                        liveCells+= oldRegion.at<cv::Vec3b>(neighborRow, neighborCol).val[0] == 255;
                    }
                }
                if(liveCells == 2){
                    // make this cell alive
                    cell[0] = 255;
                    cell[1] = 255;
                    cell[2] = 254; // I want it white, but not to trigger a dying check
                }
            }
            region.at<cv::Vec3b>(row, col) = cell;
            
        }
    }
}

// proves a serail frame equals a parallel one
void briansConfrimation(cv::Mat &prevState, cv::Mat &currentState){
    int rows = currentState.rows;
    int cols = currentState.cols;
    assert(rows == prevState.rows);
    assert(cols == prevState.cols);
    iterateBrain(prevState, 0, rows);
    // now prevState should be the same as currentState
    for(int i = 0; i<rows; i++){
        for(int j = 0; j < cols; j++){
            for(int k = 0; k<prevState.channels(); k++){
                assert(prevState.at<cv::Vec3b>(i, j).val[k] == currentState.at<cv::Vec3b>(i, j).val[k]);
            }
        }
    }

}

// Used for showing ghost rows are working
void iterateDriver(cv::Mat &region, int start, int end, int haloLevels){ // end is not inclusive
    cv::Mat oldRegion = region.clone();

    for(int row = start; row < end; row++){
        for(int col = 0; col < region.cols; col++){
            cv::Vec3b cell = oldRegion.at<cv::Vec3b>(row, col);
            if(cell[0] != 0){
                // it was active, so kill it 
                cell[0] = 50;
                cell[2] = 50;
                region.at<cv::Vec3b>(row, col) = cell;

                continue;
            }
            int above = row+1;
            if(row == end) continue;
            cv::Vec3b aboveCell = oldRegion.at<cv::Vec3b>(above, col);
            if(cell[0] == 0 && aboveCell[0] == 255){
                cell[0] = 255;
            }

            region.at<cv::Vec3b>(row, col) = cell;
            
        }
    }
}



int main(int argc, char **argv){

    int rank = 0;
    int nproc = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    assert(argc == 3);
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    MPI_Datatype vec3b;
    MPI_Type_contiguous(3, MPI_UNSIGNED_CHAR, &vec3b);
    MPI_Type_commit(&vec3b);

    MPI_Datatype rowOfN;
    MPI_Type_contiguous(cols, vec3b, &rowOfN);
    MPI_Type_commit(&rowOfN);

    int localStart;
    int localStop;
    int localCount;

    parallelRange(0, rows, rank, nproc, localStart, localStop, localCount);
    double startTime = MPI_Wtime();
    srand(20+rank);
    int upperGhost = localStart != 0;
    int lowerGhost = localStop != rows - 1;
    cv::Mat region(localCount+upperGhost+lowerGhost, cols, CV_8UC3);
    cv::Mat lastWorld(rows, cols, CV_8UC3); // the last iteration of brians brain used for verification
    /*
    if(rank == nproc-1){
        int someCounter = 0;
        for(int i=0; i<cols; i++){
            region.at<cv::Vec3b>(someCounter, i).val[0] = 255;
            someCounter = (someCounter + 1)%localCount;
        }
    } */ // initial condition for driver code ^^^^

    for(int i = 0; i < localCount; i++){
        for(int j = 0; j < cols; j++){
            if(rand() % rows < 5){
                region.at<cv::Vec3b>(i, j).val[0] = 255;
                region.at<cv::Vec3b>(i, j).val[1] = 255;
                region.at<cv::Vec3b>(i, j).val[2] = 254;
            }
        }
    }
   

    int indexBottomGhost = (region.rows - 1)*(cols * 3);
    for(int iteration = 0; iteration < 500; iteration++){
        // have this processor sendrecv a ghost row with its neighbor processors
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank % 2 == 1){
            // lets handle the upwards neighbor first
            int neighborUp = rank - 1;
            if(neighborUp>=0){
                cv::Mat toSend = region.row(1);
                MPI_Sendrecv(&toSend.data[0], 1, rowOfN, neighborUp, 99, &region.data[0], 1, rowOfN, neighborUp, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else{
                // lets make sure that this was rank 0
                assert(rank==0);
            }
            // now handle the down neighbor second
            int neighborDown = (rank + 1);
            if(neighborDown<nproc){
                cv::Mat toSend = region.row(region.rows-2);
                MPI_Sendrecv(&toSend.data[0], 1, rowOfN, neighborDown, 99, &region.data[indexBottomGhost], 1, rowOfN, neighborDown, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            }
            else{
                assert(rank == nproc-1);
            }
        }
        else{
            // handle the downwards neighbor first
            int neighborDown = (rank + 1);
            if(neighborDown<nproc){
                cv::Mat toSend = region.row(region.rows-2);
                MPI_Sendrecv(&toSend.data[0], 1, rowOfN, neighborDown, 99, &region.data[indexBottomGhost], 1, rowOfN, neighborDown, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            }
            else{
                assert(rank == nproc-1);
            }
            //up
            int neighborUp = rank - 1;
            if(neighborUp>=0){
                cv::Mat toSend = region.row(1);
                MPI_Sendrecv(&toSend.data[0], 1, rowOfN, neighborUp, 99, &region.data[0], 1, rowOfN, neighborUp, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else{
                // lets make sure that this was rank 0
                assert(rank==0);
            }
        }
       
        iterateBrain(region, upperGhost, region.rows-lowerGhost);

        // index for sending the data to 0
        int startSendingIndex = (upperGhost)*cols*3; // if no upperghost, start sending from the 0th row

        if(rank == 0){
            // collect everything and show this iteration
            cv::Mat world(rows, cols, CV_8UC3);

            int recvCounts[nproc];
            int recvDispls[nproc];

            int count = 0;
            for(int i = 0; i<nproc; i++){
                int theStart;
                int theStop;
                int theCount;
                parallelRange(0, rows, i, nproc, theStart, theStop, theCount);
                recvCounts[i] = theCount; // in rowsOfN
                recvDispls[i] = count; // cumulative counter of rows of n
                count += theCount;
                // wow that is some confusing variable names... i feel like im in a comp 1012 midterm reading that

            }
            
            
            MPI_Gatherv(&region.data[startSendingIndex], localCount, rowOfN, 
                            &world.data[0], recvCounts, recvDispls, rowOfN, 0, MPI_COMM_WORLD);
            
            #ifdef VERIFY
            if(iteration != 0){
                // old world is empty, so just copy it
                briansConfrimation(lastWorld, world);
            }
            lastWorld = world.clone();
            #endif
            cv::imshow("Brain", world);

            cv::waitKey(2);
        }
        else{
            MPI_Gatherv(&region.data[startSendingIndex], localCount, rowOfN, NULL, NULL, NULL, rowOfN, 0, MPI_COMM_WORLD);
        }

        
    }

    double endTime = MPI_Wtime();
    if(rank == 0){
        std::cout << "Time taken for simulation is " << endTime - startTime << " seconds" << std::endl;
    }
    /*
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(rand() % rows < 1){
                region.at<cv::Vec3b>(i, j).val[0] = 255;
                region.at<cv::Vec3b>(i, j).val[1] = 255;
                region.at<cv::Vec3b>(i, j).val[2] = 254;
            }
        }
    }
    //region.at<cv::Vec3b>(0, 0).val[0] = 255;
    //region.at<cv::Vec3b>(0, 1).val[0] = 255;
    
    for(int i = 0; i < 1000; i++){
        iterateBrain(region, 1, rows-1);
        cv::imshow("Brain", region);
        cv::waitKey(1);
    }*/
    
    MPI_Type_free(&rowOfN);
    MPI_Type_free(&vec3b);
    MPI_Finalize();
    

}

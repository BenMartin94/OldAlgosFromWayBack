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

void iterateBrain(cv::Mat &region, int start, int end){ // end is not inclusive
    assert(start == 1);
    assert(end == region.rows-1);
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
                        int neighborRow = (row + nRow)%end;
                        if(neighborRow < 0) neighborRow = end-1;
                        int neighborCol = (col + nCol)%region.cols;
                        if(neighborCol < 0) neighborCol = region.cols - 1;
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

void iterateDriver(cv::Mat &region, int start, int end, int haloLevels){ // end is not inclusive
    cv::Mat oldRegion = region.clone();

    for(int row = start; row < end; row++){
        for(int col = 0; col < region.cols; col++){
            cv::Vec3b cell = oldRegion.at<cv::Vec3b>(row, col);
            if(cell[0] != 0){
                // it was active, so kill it 
                cell[0] = 0;
                region.at<cv::Vec3b>(row, col) = cell;

                continue;
            }
            int above = row-1;
            if(row == 0) continue;
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
    //assuming always 1 ghost row
    parallelRange(0, rows, rank, nproc, localStart, localStop, localCount);
    std::cout<<MPI_Wtime()<<std::endl;
    srand(clock());
    int upperGhost = localStart != 0;
    int lowerGhost = localStop != rows - 1;
    cv::Mat region(localCount+upperGhost+lowerGhost, cols, CV_8UC3);
    if(rank == 0)
        for(int i=0; i<cols; i++){
            region.at<cv::Vec3b>(0, i).val[0] = 255;
        }
    

    int indexBottomGhost = (region.rows - 1)*(cols * 3);
    for(int i = 0; i < 1000; i++){
        // have this processor sendrecv a ghost row with its neighbor processors
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
       
        iterateDriver(region, upperGhost, region.rows-lowerGhost, 0);
        cv::imshow("Brain", region);
        cv::waitKey(1);
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
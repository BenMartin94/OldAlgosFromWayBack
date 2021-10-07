#include <iostream>
#include <opencv4/opencv2/core/core.hpp>      //changes may be required to opencv includes
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <mpi.h>

using namespace std;

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

int getLowerBound(int localStart, int h, int globalRows){
    // given the boundaries compute the row to start on after the halo row
    int toRet = localStart + h;
    // this seems easy, prob dont even need it
    // if you are reading this, i just forgot to delete it.
    return 0;
}

int main(int argc, char**argv){

    int rank = 0;
    int nproc = 1;
    int n = 1000;
    int m = 200;
    int h = 10;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype vec3b;
    MPI_Type_contiguous(3, MPI_UNSIGNED_CHAR, &vec3b);
    MPI_Type_commit(&vec3b);

    MPI_Datatype rowOfN;
    MPI_Type_contiguous(n, vec3b, &rowOfN);
    MPI_Type_commit(&rowOfN);

    


    if(rank == 0){
        cv::Mat blackImage(m, n, CV_8UC3);
       
        cv::Vec3b black;
        black.val[0] = 0;
        black.val[1] = 0;
        black.val[2] = 0;
        
        for(int irow = 0; irow<blackImage.rows; irow++){
            black.val[0] = irow;
            for(int icol = 0; icol<blackImage.cols; icol++){
                blackImage.at<cv::Vec3b>(irow, icol) = black;
            }
        }
        int myStart;
        int myStop;
        int myCount;
        for(int irank = 0; irank<nproc; irank++){
            int localStart;
            int localStop;
            int localCount;

            parallelRange(0, m, irank, nproc, localStart, localStop, localCount);
            localStart -= h;
            localStop += h;
        
            if(localStart < 0){
                localStart = 0;
            }
            if(localStop >= m){
                localStop = m-1;
            }
            localCount = localStop - localStart + 1;
            if(irank == 0){
                myStart = localStart;
                myStop = localStop;
                myCount = localCount;
                cout<<"Rank: " << rank << " starts at " << myStart << " ends at " << myStop << " and has halo value " << h << endl;

            }
            else{
                int dataBuf[3] = {localStart, localStop, h};
                MPI_Send(dataBuf, 3, MPI_INT, irank, 99, MPI_COMM_WORLD);
                vector<cv::Vec3b> rows(0);
                for(int irow = localStart; irow<=localStop; irow++){
                    for(int icol = 0; icol<blackImage.cols; icol++){
                        cv::Vec3b pixel = blackImage.at<cv::Vec3b>(irow, icol);
                        //cout<<pixel<<endl;
                        rows.push_back(pixel);
                        //cout<<blackImage.at<cv::Vec3b>(irow, icol)<<endl;
                    }
                }
                
                for(int i = 0; i<localCount*n; i++){
                    cout<<(int)blackImage.data[localStart*n*3 + i] << " ";
                }
                
                
                MPI_Send(&blackImage.data[localStart*n*3], localCount, rowOfN, irank, 99, MPI_COMM_WORLD);

            }
        }
        cv::Mat alteredImage = blackImage.clone();
        
        cv::namedWindow("Full image", cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow("Full image", blackImage);      // Show our image inside it.
    
        cv::waitKey(10000);	

    }
    else{
        int dataBuf[3];
        MPI_Recv(dataBuf, 3, MPI_INT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout<<"Rank: " << rank << " starts at " << dataBuf[0] << " ends at " << dataBuf[1] << " and has halo value " << dataBuf[2] << endl;
        int localCount = dataBuf[1] - dataBuf[0] + 1;
        vector<cv::Vec3b> rows(localCount*n);
        cv::Mat myImage(localCount, n, CV_8UC3);

        MPI_Recv(&myImage.data[0], localCount*n, rowOfN, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /*
        for(int irow = 0; irow<localCount; irow++){
            for(int icol = 0; icol<n; icol++){
                myImage.at<cv::Vec3b>(irow, icol) = rows[irow*n + icol];
            }
        }*/
        cv::String name("ummmm " + (char)rank);
        
        cv::namedWindow(name, cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow(name, myImage);      // Show our image inside it.
    
        cv::waitKey(10000);

    }

    MPI_Type_free(&rowOfN);
    MPI_Type_free(&vec3b);
    MPI_Finalize();
}
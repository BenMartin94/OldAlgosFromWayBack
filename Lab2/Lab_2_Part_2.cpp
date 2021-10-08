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
            //black.val[0] = irow;
            for(int icol = 0; icol<blackImage.cols; icol++){
                blackImage.at<cv::Vec3b>(irow, icol) = black;
            }
        }
        int myStart;
        int myStop;
        int myCount;
        int myWorkStart;
        int myWorkStop;
        for(int irank = 0; irank<nproc; irank++){
            int localStart;
            int localStop;
            int localCount;
            int localWorkStart;
            int localWorkStop; // the work means excluding halos

            parallelRange(0, m, irank, nproc, localStart, localStop, localCount);
            localWorkStart = localStart;
            localWorkStop = localStop;
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
                myWorkStart = localWorkStart;
                myWorkStop = localWorkStop;
                cout<<"Rank: " << rank << " starts at " << myStart << " ends at " << myStop << " and is processing " << myWorkStart << "-" << myWorkStop << endl;

            }
            else{
                int dataBuf[4] = {localStart, localStop, localWorkStart, localWorkStop};
                MPI_Send(dataBuf, 4, MPI_INT, irank, 99, MPI_COMM_WORLD);      
                MPI_Send(&blackImage.data[localStart*n*3], localCount, rowOfN, irank, 99, MPI_COMM_WORLD);

            }
        }
        cv::Mat alteredImage = blackImage.clone();
        
        for(int irow = myWorkStart; irow<myWorkStop; irow++){
            for(int icol = 0; icol<alteredImage.cols; icol++){
                alteredImage.at<cv::Vec3b>(irow, icol).val[rank] = 255;
            }
        }

        cv::namedWindow("Partial image rank 0", cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow("Partial image rank 0", alteredImage);      // Show our image inside it.
    
        cv::waitKey(10000);	
        int positioning[2];
        
        for(int i = 1; i<nproc; i++){
            cout<<"Getting the image from " << i << endl;
            MPI_Recv(positioning, 2, MPI_INT, i, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // postioning[0] is the starting place to put the data
            int amountToRead = positioning[1] - positioning[0] + 1;
            //cout<<"rank 0 received "<<amountToRead<<" rows"<<endl;
            MPI_Recv(&alteredImage.data[positioning[0]*n*3], amountToRead, rowOfN, i, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        cv::namedWindow("Full image", cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow("Full image", alteredImage);      // Show our image inside it.
    
        cv::waitKey(10000);	

    }
    else{
        int dataBuf[4];
        MPI_Recv(dataBuf, 4, MPI_INT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout<<"Rank: " << rank << " starts at " << dataBuf[0] << " ends at " << dataBuf[1] << " and is processing " << dataBuf[2] << "-" << dataBuf[3] << endl;
        int localCount = dataBuf[1] - dataBuf[0] + 1;
        int localWorkCount = dataBuf[3] - dataBuf[2] + 1;
        cout<<"but locally on this processor I will actually start processing at " << dataBuf[2] - dataBuf[0]<< " and go for " << localWorkCount << endl;


        
        //assert(localWorkCount == (dataBuf[3] - dataBuf[1]) - (dataBuf[2] - dataBuf[0]));

        // the values given are in global coordinates but I need local to this processors image.
        // the offset is workStart - start
        int rowOffset = dataBuf[2] - dataBuf[0];

        cv::Mat myImage(localCount, n, CV_8UC3);

        cout<<"Rank: " << rank << " has an image " << myImage.rows << "rows" <<endl;

        MPI_Recv(&myImage.data[0], localCount*n, rowOfN, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        for(int irow = 0; irow<localWorkCount; irow++){
            //cout<<"Rank: " << rank << " has an image with " << myImage.rows << "rows and I am currently handling row " << irow + rowOffset <<endl;

            for(int icol = 0; icol<myImage.cols; icol++){
                myImage.at<cv::Vec3b>(irow + rowOffset, icol).val[rank] = 255;
            }
        }        

        cv::String name("ummmm");
        name.push_back('0' + rank);
        cv::namedWindow(name, cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow(name, myImage);      // Show our image inside it.

        cv::waitKey(10000);

        int positioning[2] = {dataBuf[2], dataBuf[3]};
        //cout<<"about to send "<<localWorkCount<<endl;
        MPI_Send(positioning, 2, MPI_INT, 0, 99, MPI_COMM_WORLD);
        MPI_Send(&myImage.data[rowOffset*n*3], localWorkCount, rowOfN, 0, 99, MPI_COMM_WORLD);

    }

    MPI_Type_free(&rowOfN);
    MPI_Type_free(&vec3b);
    MPI_Finalize();
}
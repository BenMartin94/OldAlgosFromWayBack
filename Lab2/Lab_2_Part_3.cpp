#include <iostream>
#include <opencv4/opencv2/core/core.hpp>      //changes may be required to opencv includes
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <mpi.h>

using namespace std;

#define WAIT_TIME 5000

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

enum IMAGE_PROCESS_TYPE{
    IMAGE_PROCESS_TYPE_BLUR = 0,
    IMAGE_PROCESS_TYPE_RETAIN,
    IMAGE_PROCESS_TYPE_SATURATE,
};

void imageColor(const cv::Mat &in, cv::Mat &out, int retainRank, int rowStart, int rowStop){
    assert(in.type() == CV_8UC3);
    out = in.clone();
    for (int irow = rowStart; irow < rowStop+1; irow++){
		for (int icol = 0; icol < out.cols; icol++){
            //bgr
            uchar red = out.at<cv::Vec3b>(irow, icol).val[2];
            uchar green = out.at<cv::Vec3b>(irow, icol).val[1];
            uchar blue = out.at<cv::Vec3b>(irow, icol).val[0];

            if(retainRank == 0){
                // get the minimum channel and keep that one
                if(red == min({red, green, blue})){
                    green = 128;
                    blue = 128;
                }
                else if(green == min({red, green, blue})){
                    red = 128;
                    blue = 128;
                }
                else{
                    green = 128;
                    blue = 128;
                }
            }
            else if(retainRank == 1){
                // get the median rank
                uchar maximum = max({red, green, blue});
                uchar minimum = min({red, green, blue});
                if(red != maximum && red != minimum){
                    green = 128;
                    blue = 128;
                }
                else if(green != maximum && green != minimum){
                    red = 128;
                    blue = 128;
                }
                else{
                    green = 128;
                    blue = 128;
                }
            
            }
            else if(retainRank == 2){
                // get the maximum
                //cout << "Finding the maxiums ";
                if(red == max({red, green, blue})){
                    //cout << "it was red" <<endl;
                    green = 128;
                    blue = 128;
                }
                else if(green == max({red, green, blue})){
                    //cout << "it was green" <<endl;

                    red = 128;
                    blue = 128;
                }
                else{
                    //cout << "it was blue" <<endl;

                    green = 128;
                    blue = 128;
                }
            }

            out.at<cv::Vec3b>(irow, icol).val[2] = red;
            out.at<cv::Vec3b>(irow, icol).val[1] = green;
            out.at<cv::Vec3b>(irow, icol).val[0] = blue;


        }
    }


}



void imageBlur(const cv::Mat &in, cv::Mat &out, int level, int rowStart, int rowStop){
    assert(in.type() == CV_8UC3);
    out = in.clone();
    double pixels[(2*level+1)*(2*level+1)];
    int i = 0; // just tracks the next index to use in pixels
    for(int channel = 0; channel<in.channels();channel++){
        for (int irow = rowStart; irow < rowStop+1; irow++){
            for (int icol = 0; icol < out.cols; icol++){
                // now i need to get the surrounding pixels and the centre one
                for(int localRow = irow-level; localRow<=irow+level; localRow++){
                    for (int localCol = icol-level; localCol<=icol+level; localCol++){
                        
                        if(localCol>=0 && localRow >= 0 && localCol<in.cols && localRow<=in.rows )
                            pixels[i++] =  (double)in.at<cv::Vec3b>(localRow, localCol).val[channel]; // get the fresh images value
                    }
                }
                // now get the average here
                double sum = 0;
                for(int index = 0; index<i;index++){
                    sum += pixels[index];
                }
                assert(i>0);
                double avg = sum/i;
                assert(avg<=255);
                uchar byteAvg = (uchar)avg;
                // clean up for the next loop
                i = 0;
                // now that we have the avg, set in the out image
                out.at<cv::Vec3b>(irow, icol).val[channel] = byteAvg;
                
            }
        }
    }
}

void adjustSaturation(cv::Vec3b &in, cv::Vec3b &out, double newSat){
    double Bprime = in[0]/255.0;
    double Gprime = in[1]/255.0;
    double Rprime = in[2]/255.0;
    double cmin = min({Bprime, Gprime, Rprime});
    double cmax = max({Bprime, Gprime, Rprime});
    double delta = cmax-cmin;
    double h;
    if(delta == 0) {
        h = 0;
        
    }
    else if(cmax==Rprime) {
        h = 60 * fmod(((Gprime-Bprime)/delta), 6);
    }
    else if(cmax==Gprime) {
        h = 60 * (((Bprime-Rprime)/delta)+2);
    }
    else h = 60 * (((Rprime-Gprime)/delta)+4);

    double s;   
    if(cmax == 0) s = 0;
    else s = delta/cmax;

    double v = cmax;

    if(h<0){
        //gotta wrap it back now
        h += 360;
    }
    else if(h >= 360){
        h-=360;
    }

    //cout<<h<<endl<<s<<endl<<v<<endl;
    assert(h>=0 && h<360);
    assert(s>=0);
    assert(v<=1);
    s = newSat;

    double C = v*s;
    double X = C*(1-abs(((int)(h/60)%2)-1));
    double m = v-C;

    if(h<60){
        Rprime = C;
        Gprime = X;
        Bprime = 0;
    }
    else if(h<120){
        Rprime = X;
        Gprime = C;
        Bprime = 0;
    }
    else if(h<180){
        Rprime = 0;
        Gprime = C;
        Bprime = X;
    }
    else if(h<240){
        Rprime = 0;
        Gprime = X;
        Bprime = C;
    }
    else if(h<300){
        Rprime = X;
        Gprime = 0;
        Bprime = C;
    }
    else if(h<360){
        Rprime = C;
        Gprime = 0;
        Bprime = X;
    }

    out[2] = (Rprime+m)*255;
    out[1] = (Gprime+m)*255;
    out[0] = (Bprime+m)*255;

}

void imageSaturation(const cv::Mat &in, cv::Mat &out, double newSat, int rowStart, int rowStop){
    out = in.clone();
    for(int irow = rowStart; irow<=rowStop; irow++){
        for(int icol = 0; icol<out.cols; icol++){
            cv::Vec3b changed;
            cv::Vec3b current = in.at<cv::Vec3b>(irow, icol);
            adjustSaturation(current, changed, newSat);
            out.at<cv::Vec3b>(irow, icol) = changed;
        }
    }
}

int main(int argc, char**argv){

    int rank = 0;
    int nproc = 1;
   
    assert(argc == 4);   
    char *fname = argv[1];
    int processType = atoi(argv[2]);
    assert(processType <= IMAGE_PROCESS_TYPE_SATURATE);
    char *endPtr;
    double param = strtod(argv[3], &endPtr);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype vec3b;
    MPI_Type_contiguous(3, MPI_UNSIGNED_CHAR, &vec3b);
    MPI_Type_commit(&vec3b);
    cout<<fname<<endl;
    cv::Mat image = cv::imread(fname,1); // just read it to get the dimensions

    int n = image.cols;
    int m = image.rows;
    int h = 10;
    MPI_Datatype rowOfN;
    MPI_Type_contiguous(n, vec3b, &rowOfN);
    MPI_Type_commit(&rowOfN);
    // get the command line params now


    if(rank == 0){
        
        double serialTime1, serialTime2, parallelTime1, parallelTime2;
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
                MPI_Send(&image.data[localStart*n*3], localCount, rowOfN, irank, 99, MPI_COMM_WORLD);

            }
        }
        cv::Mat alteredImage;
        
        // do the work here
        parallelTime1 = MPI_Wtime();
        if(processType == IMAGE_PROCESS_TYPE_BLUR){
            assert(h>=param);
            imageBlur(image, alteredImage, param, myWorkStart, myWorkStop); // -1 since inclusive of the rowstop
            
        }
        if(processType == IMAGE_PROCESS_TYPE_RETAIN){
            imageColor(image, alteredImage, param, myWorkStart, myWorkStop);
        }
        if(processType == IMAGE_PROCESS_TYPE_SATURATE){
            imageSaturation(image, alteredImage, param, myWorkStart, myWorkStop);
        }
        #ifdef SHOW
        cv::namedWindow("Partial image rank 0", cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow("Partial image rank 0", alteredImage);      // Show our image inside it.
    
        cv::waitKey(WAIT_TIME);	
        #endif
        int positioning[2];
        
        for(int i = 1; i<nproc; i++){
            cout<<"Getting the image from " << i << endl;
            MPI_Recv(positioning, 2, MPI_INT, i, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // postioning[0] is the starting place to put the data
            int amountToRead = positioning[1] - positioning[0] + 1;
            //cout<<"rank 0 received "<<amountToRead<<" rows"<<endl;
            MPI_Recv(&alteredImage.data[positioning[0]*n*3], amountToRead, rowOfN, i, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        #ifdef SHOW
        cv::namedWindow("Full image", cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow("Full image", alteredImage);      // Show our image inside it.
    
        cv::waitKey(WAIT_TIME);	
        #endif
        parallelTime2 = MPI_Wtime();

        serialTime1 = MPI_Wtime();

        // now do the thing in serial and compare
        cv::Mat alteredImageSerial;
        if(processType == IMAGE_PROCESS_TYPE_BLUR){
            assert(h>=param);
            imageBlur(image, alteredImageSerial, param, 0, image.rows-1); // -1 since inclusive of the rowstop
            
        }
        if(processType == IMAGE_PROCESS_TYPE_RETAIN){
            imageColor(image, alteredImageSerial, param, 0, image.rows-1);
        }
        if(processType == IMAGE_PROCESS_TYPE_SATURATE){
            imageSaturation(image, alteredImageSerial, param, 0, image.rows-1);
        }
        #ifdef SHOW
        cv::namedWindow("Full image Serial", cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow("Full image Serial", alteredImageSerial);      // Show our image inside it.
        cv::waitKey(WAIT_TIME);	
        #endif
        serialTime2 = MPI_Wtime();
        // now loop through both images and assert every pixel is equal
        for(int irow = 0; irow<image.rows;irow++){
            for(int icol = 0; icol<image.cols; icol++){
                for(int channel = 0; channel<image.channels(); channel++){
                    assert(alteredImage.at<cv::Vec3b>().val[channel] == alteredImageSerial.at<cv::Vec3b>().val[channel]);
                }
            }
        }
        cout<<"Images are Identical!!!!" << endl;

        cout<<"Time taken in parallel " << parallelTime2 - parallelTime1 << endl;
        cout<<"Time taken in serial " << serialTime2 - serialTime1 <<endl;
        cout<<"Speedup is " << (serialTime2 - serialTime1)/(parallelTime2 - parallelTime1)<<endl;

 
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
        cv::Mat myProcessedImage;
        cout<<"Rank: " << rank << " has an image " << myImage.rows << "rows" <<endl;

        MPI_Recv(&myImage.data[0], localCount*n, rowOfN, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // do the work here
        if(processType == IMAGE_PROCESS_TYPE_BLUR){
            assert(h>=param);
            imageBlur(myImage, myProcessedImage, param, rowOffset, rowOffset+localWorkCount-1); // -1 since inclusive of the rowstop
            
        }
        if(processType == IMAGE_PROCESS_TYPE_RETAIN){
            imageColor(myImage, myProcessedImage, param, rowOffset, rowOffset+localWorkCount-1);
        }
        if(processType == IMAGE_PROCESS_TYPE_SATURATE){
            imageSaturation(myImage, myProcessedImage, param, rowOffset, rowOffset+localWorkCount-1);
        }
        #ifdef SHOW
        cv::String name("Processor: ");
        name.push_back('0' + rank);
        cv::namedWindow(name, cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow(name, myImage);      // Show our image inside it.

        cv::waitKey(WAIT_TIME);

        name.push_back('0' + rank);
        cv::namedWindow(name, cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow(name, myProcessedImage);      // Show our image inside it.

        cv::waitKey(WAIT_TIME);
        #endif

        int positioning[2] = {dataBuf[2], dataBuf[3]};
        //cout<<"about to send "<<localWorkCount<<endl;
        MPI_Send(positioning, 2, MPI_INT, 0, 99, MPI_COMM_WORLD);
        MPI_Send(&myProcessedImage.data[rowOffset*n*3], localWorkCount, rowOfN, 0, 99, MPI_COMM_WORLD);

    }

    MPI_Type_free(&rowOfN);
    MPI_Type_free(&vec3b);
    MPI_Finalize();
}
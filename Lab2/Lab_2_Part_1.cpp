#include <iostream>
#include <opencv4/opencv2/core/core.hpp>      //changes may be required to opencv includes
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <mpi.h>

using namespace std;


typedef struct{
    int rows, cols;
    int process_type;
}imageProcessingDescriptorT;

enum IMAGE_PROCESS_TYPE{
    IMAGE_PROCESS_TYPE_MIN = 0,
    IMAGE_PROCESS_TYPE_BLUR,
    IMAGE_PROCESS_TYPE_RETAIN,
    IMAGE_PROCESS_TYPE_SATURATE,
    IMAGE_PROCESS_TYPE_MAX
};

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
                        if(localCol>=0 && localRow >= rowStart && localCol<in.cols && localRow<=rowStop )
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

int main(int argc, char** argv)
{
    cv::Mat image;
    image = cv::imread("./space.jpg", 1);
    if(! image.data ) // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
    }
    else{

        cv::Mat processed_image;
        //imageColor(image, processed_image, 1, 0, image.rows/2);
        //imageBlur(image, processed_image, 1, 0, image.rows/2);
        imageSaturation(image, processed_image, 0.5, 0, image.rows/2);
        /*cv::Vec3b color;
        cv::Vec3b idc;
        color.val[0] = 1;
        color.val[1] = 13;
        color.val[2] = 200;
        adjustSaturation(color, idc, 0);*/

        cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE );	// Create a window for display.
        cv::imshow("Original Image", image);                   				// Show our image inside it.
        
        cv::namedWindow("Zero Channel Image", cv::WINDOW_AUTOSIZE );	// Create display window.
        cv::imshow("Zero Channel Image", processed_image);      // Show our image inside it.
        
        
        cv::waitKey(10000);	//wait 10 seconds before closing image (or a keypress to close)
  

    }
}
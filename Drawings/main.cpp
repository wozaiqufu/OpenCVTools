#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;
using namespace std;

const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;
Mat src1;
Mat src2;
Mat dist;
static void on_trackBar(int,void *)
{
    alpha = (double)alpha_slider/alpha_slider_max;
    beta = (1.0 - alpha);
    addWeighted(src1,alpha,src2,beta,0.0,dist);
    imshow("Linear Blend",dist);
}
int main()
{
    std::cout << "This is AddingTrackBar Program!" << std::endl;

    src1 = imread("../data/LinuxLogo.jpg");
    src2 = imread("../data/WindowsLogo.jpg");
    if(src1.empty())
    {
        cout<<"Error loading src1"<<endl;
    }
    if(src2.empty())
    {
        cout<<"Error loading src2"<<endl;
    }

    alpha_slider = 0;

    namedWindow("Linear Blend",WINDOW_AUTOSIZE);
    char trackBarName[50];
    sprintf(trackBarName,"Alpha x %d",alpha_slider_max);
    createTrackbar(trackBarName,"Linear Blend",&alpha_slider,alpha_slider_max,on_trackBar);
    on_trackBar(alpha_slider,0);
    waitKey(0);

    return 0;
}
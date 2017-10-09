#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include "EigenFaces.h"

using namespace cv;
using namespace std;

int main()
{
//    VideoCapture cap(0);
//    if(!cap.isOpened())
//    {
//        cout<<"no camera device!"<<endl;
//        return -1;
//    }
//    Mat img;
//    namedWindow("MacCamera",1);
//    for(;;)
//    {
//        cap>>img;
//        imshow("MacCamera",img);
//        if(waitKey(30)>0)
//        {
//            break;
//        }
//    }
//    return 0;

    EigenFaces eigenface;
    eigenface.loadData("ew");



}

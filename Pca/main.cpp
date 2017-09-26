#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include "cvui.h"

using namespace cv;
using namespace std;

#define WINDOW_NAME "PCA"

const char* keys =
        {
                "{filename |../imgList.txt | image list file}"
        };

static void readImgList(const string& filename,vector<Mat>& images)
{
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line;
    while (getline(file, line)) {
        images.push_back(imread(line, 0));
    }
}

static Mat formatImagesForPCA(const vector<Mat>& data)
{
    Mat dst(static_cast<int>(data.size()),data[0].rows*data[0].cols,CV_32F);
    for(unsigned int i=0;i<data.size();i++)
    {
        Mat image_row = data[i].clone().reshape(1,1);
        Mat row_i = dst.row(i);
        image_row.convertTo(row_i,CV_32F);
    }
    return dst;
}

static Mat toGrayscale(InputArray _src)
{
    Mat src = _src.getMat();
    //only allow one channel
    if(src.channels()!=1)
    {
        CV_Error(Error::StsBadArg,"Only matrices with one channel are supported!");
    }
    //create and return normolized image
    Mat dst;
    cv::normalize(_src,dst,0,255,NORM_MINMAX,CV_8UC1);
    return dst;
}

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv, keys);
    string file = parser.get<string>("filename");
    vector<Mat> imgs;
    try
    {
        readImgList(file,imgs);
    }
    catch(cv::Exception& e)
    {
        cerr<<"Error opening file \""<<file<<"\". Reason:"<<e.msg<<endl;
        exit(1);
    }
    //this demo needs at least 2 images
    if(imgs.size()<=1)
    {
        string error_message = "This demo needs at least 2 images to work! Please add more image to your data set!";
        CV_Error(Error::StsError,error_message);
    }
    //reshape and stack images into a row matrix
    Mat data = formatImagesForPCA(imgs);
    //perform pca
    PCA pca(data,cv::Mat(),PCA::DATA_AS_ROW,0.95);
    //demostration of the effect of retainedVaraince on the first image
    Mat point = pca.project(data.row(0));//project into the eigenspace,thus the image becomes a point with each element
    //representing the linear combination coefficients
    Mat reconstruction = pca.backProject(point);//recreate the image from the "point"
    reconstruction = reconstruction.reshape(imgs[0].channels(),imgs[0].rows);
    reconstruction = toGrayscale(reconstruction);

    Mat frame = Mat(80,300,CV_8UC3);
    double trackBarValue = 100.0;
    bool checked1 = false;
    bool checked2 = true;
    namedWindow(WINDOW_NAME);
    cvui::init(WINDOW_NAME);
    namedWindow("Face",WINDOW_AUTOSIZE);
    for(;;)
    {
        //fill the frame with a nice color-RGB
        frame = Scalar(40,50,40);
        //show some text
        cvui::text(frame,10,5,"This is PCA demostration");
        //trackBar component
        cvui::trackbar(frame,50,30,200,&trackBarValue,0.,100.);

        PCA pca_realTime(data,cv::Mat(),PCA::DATA_AS_ROW,trackBarValue/100.0);
        //demostration of the effect of retainedVaraince on the first image
        Mat point_realTime = pca_realTime.project(data.row(0));//project into the eigenspace,thus the image becomes a point with each element
        //representing the linear combination coefficients
        Mat reconstruction_realTime = pca_realTime.backProject(point_realTime);//recreate the image from the "point"
        reconstruction_realTime = reconstruction_realTime.reshape(imgs[0].channels(),imgs[0].rows);
        reconstruction_realTime = toGrayscale(reconstruction_realTime);
        //update must be called after all components
        //it handles mouse click and others behind the scene with magic!
        cvui::update();
        imshow(WINDOW_NAME,frame);
        imshow("Face",reconstruction_realTime);
        if(waitKey(20)==27)//press escp to exit the process
        {
            break;
        }
    }

    return 0;
}
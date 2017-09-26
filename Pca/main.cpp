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

    Mat frame = Mat(100,500,CV_8UC3);
    int count = 0;
    double counterFloat = 0.0;
    double trackBarValue = 0.0;
    bool checked1 = false;
    bool checked2 = true;
    namedWindow(WINDOW_NAME);
    cvui::init(WINDOW_NAME);
    for(;;)
    {
        //fill the frame with a nice color-RGB
        frame = Scalar(40,50,40);
        //show some text
        cvui::text(frame,10,5,"This is PCA demostration");
        //show text with what some algorithms' output
        cvui::printf(frame,10,20,0.4,0x00ff00,"use printf formatting:%d + %.2f = %.2f",2,3.1,5.1);
        //click button and on_button_clicked verified by printf
        if(cvui::button(frame,10,40,"Button"))
        {
            count++;
        }

        cvui::printf(frame,100,48,0.4,0xff0000,"button click count: %d",count);
        //window component
        cvui::window(frame,10,80,200,180,"Window");
        //counter component:int
        cvui::counter(frame,10,300,&count);
        //counter used as doubles default step:0.5,"%.2f"
        cvui::counter(frame,10,320,&counterFloat,0.1);
        //trackBar component
        cvui::trackbar(frame,350,20,300,&trackBarValue,0.,100.);
        cvui::printf(frame,350,90,0.4,0xff000,"current trackbar value is:%.2f",trackBarValue);
        //checkboxes
        cvui::checkbox(frame,350,130,"checkbox",&checked1);
        cvui::checkbox(frame,350,170,"a checked box",&checked2);
        //update must be called after all components
        //it handles mouse click and others behind the scene with magic!
        cvui::update();
        imshow(WINDOW_NAME,frame);
        if(waitKey(20)==27)//press escp to exit the process
        {
            break;
        }
    }

    return 0;
}
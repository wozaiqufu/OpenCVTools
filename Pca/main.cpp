#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

struct params
{
    Mat data;
    int ch;
    int rows;
    PCA pca;
    string winName;
};

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
//each image M*N,then the output Mat is numberof(images)*(M*N) flat matrix
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
static void onTrackbar(int pos,void* ptr)
{
    cout<<"Retained Variance:"<<pos<<" %"<<endl;
    cout<<"recalculating pca.."<<std::flush;
    double var = pos/100.0;
    struct params* p = (struct params *)ptr;
    p->pca = PCA(p->data,cv::Mat(),PCA::DATA_AS_ROW,var);

    Mat point = p->pca.project(p->data.row(0));
    Mat reconstruction = p->pca.backProject(point);
    reconstruction = reconstruction.reshape(p->ch,p->rows);
    reconstruction = toGrayscale(reconstruction);

    imshow(p->winName,reconstruction);
    cout<<"done! * of principle components:"<<p->pca.eigenvectors.rows<<endl;
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

    //init high GUI window
    string windowname = "Reconstruction|press 'q' to quit!";
    namedWindow(windowname,WINDOW_FULLSCREEN);
    //param
    params p;
    p.data = data;
    p.ch = imgs[0].channels();
    p.rows = imgs[0].rows;
    p.pca = pca;
    p.winName = windowname;

    //create the trackbar
    int pos = 95;
    createTrackbar("Retained Variance (%)",windowname,&pos,100,onTrackbar,(void*)&p);
    //display until user press 'q'
    imshow(windowname,reconstruction);

    char key = 0;
    while(key!='q')
    {
        key = (char)waitKey();
    }
//    cout<<file<<endl;
//    parser.printMessage();
//    cout<<"number of images:"<<imgs.size();
    return 0;
}
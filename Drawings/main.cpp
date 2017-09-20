#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

#define w 400
using namespace cv;
using namespace std;


void drawEllipse(Mat img,double angle);
void drawFilledCircle(Mat img,Point center);
void drawPolygon(Mat img);
void drawLine(Mat img,Point start,Point end);

int main(void)
{
    cout<<"This is Drawing project!"<<endl;
    char atomWindow[] = "Drawing 1:Atom";
    char rookWindow[] = "Drawing 2:Rook";
    Mat atomImage = Mat::zeros(w,w,CV_8UC3);
    Mat rookImage = Mat::zeros(w,w,CV_8UC3);

    drawEllipse(atomImage,90);
    drawEllipse(atomImage,0);
    drawEllipse(atomImage,45);
    drawEllipse(atomImage,-45);

    drawFilledCircle(atomImage,Point(w/2,w/2));

    drawPolygon(rookImage);
    rectangle(rookImage,
              Point(0,7*w/8),
              Point(w,w),
              Scalar(0,255,255),
              FILLED,
              LINE_8);
    drawLine( rookImage, Point( 0, 15*w/16 ), Point( w, 15*w/16 ) );
    drawLine( rookImage, Point( w/4, 7*w/8 ), Point( w/4, w ) );
    drawLine( rookImage, Point( w/2, 7*w/8 ), Point( w/2, w ) );
    drawLine( rookImage, Point( 3*w/4, 7*w/8 ), Point( 3*w/4, w ) );

    imshow(atomWindow,atomImage);
    imshow(rookWindow,rookImage);
    moveWindow( rookWindow, w, 200 );
    waitKey(0);
    return 0;
}

void drawEllipse(Mat img,double angle)
{
    int thickness = 2;
    int lineType = 8;

    ellipse(img,
            Point(w/2,w/2),
            Size(w/4,w/16),
            angle,
            0,
            360,
            Scalar(255,0,0),
            thickness,
            lineType);
}

void drawFilledCircle(Mat img,Point center)
{
    circle(img,
    center,
    w/32,
    Scalar(0,0,255),
    FILLED,
    LINE_8);
}

void drawPolygon(Mat img)
{
    Point rook_points[1][20];
    rook_points[0][0]  = Point(    w/4,   7*w/8 );
    rook_points[0][1]  = Point(  3*w/4,   7*w/8 );
    rook_points[0][2]  = Point(  3*w/4,  13*w/16 );
    rook_points[0][3]  = Point( 11*w/16, 13*w/16 );
    rook_points[0][4]  = Point( 19*w/32,  3*w/8 );
    rook_points[0][5]  = Point(  3*w/4,   3*w/8 );
    rook_points[0][6]  = Point(  3*w/4,     w/8 );
    rook_points[0][7]  = Point( 26*w/40,    w/8 );
    rook_points[0][8]  = Point( 26*w/40,    w/4 );
    rook_points[0][9]  = Point( 22*w/40,    w/4 );
    rook_points[0][10] = Point( 22*w/40,    w/8 );
    rook_points[0][11] = Point( 18*w/40,    w/8 );
    rook_points[0][12] = Point( 18*w/40,    w/4 );
    rook_points[0][13] = Point( 14*w/40,    w/4 );
    rook_points[0][14] = Point( 14*w/40,    w/8 );
    rook_points[0][15] = Point(    w/4,     w/8 );
    rook_points[0][16] = Point(    w/4,   3*w/8 );
    rook_points[0][17] = Point( 13*w/32,  3*w/8 );
    rook_points[0][18] = Point(  5*w/16, 13*w/16 );
    rook_points[0][19] = Point(    w/4,  13*w/16 );
    const Point* ppt[1] = { rook_points[0] };
    int npt[] = { 20 };
    fillPoly( img,
              ppt,
              npt,
              1,
              Scalar( 255, 255, 255 ),
              LINE_8 );
}

void drawLine(Mat img,Point start,Point end)
{
    int thickness = 2;
    int lineType = LINE_8;

    line(img,
    start,
    end,
    Scalar(0,0,0),
    thickness,
    lineType);
}
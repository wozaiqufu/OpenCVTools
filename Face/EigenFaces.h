//
// Created by Kevin on 29/09/2017.
//

#ifndef CVROOTPROJECT_EIGENFACES_H
#define CVROOTPROJECT_EIGENFACES_H

#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace cv;
using namespace cv::face;
using namespace std;

class EigenFaces {
public:
    EigenFaces();
    ~EigenFaces();
    void loadData(const string filename);
    void process();
    void plot();

private:


};


#endif //CVROOTPROJECT_EIGENFACES_H

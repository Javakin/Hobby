//
// Created by daniel on 3/9/19.
//

#ifndef GEN_IMAGEHANDLE_H
#define GEN_IMAGEHANDLE_H

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

#define IMAGE_THIKNESS          0.005    // in m
#define IMAGE_MINIMUM_THIKNESS  0.0008   // in m
#define IMAGE_WIDTH             0.1      // in m


using namespace std;
using namespace cv;

class ImageHandle {
public:
    ImageHandle(string aFileName, double aWidth, double aThikness, double aMinThikness);
    void showImage(string aName);

    void generateSurface(vector<float>& aPlane, vector<float>& stepSizes);
    int getRows();
    int getCols();
    double getImageHeight();

private:
    Mat image;
    double width, thikness, minThikness;

};


#endif //GEN_IMAGEHANDLE_H

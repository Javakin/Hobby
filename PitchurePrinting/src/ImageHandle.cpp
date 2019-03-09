//
// Created by daniel on 3/9/19.
//

#include "ImageHandle.h"

ImageHandle::ImageHandle(string aFileName, double aWidth, double aThikness, double aMinThikness) {
    image = imread(aFileName, 0);
    width = aWidth;
    thikness = aThikness;
    minThikness = aMinThikness;
}

void ImageHandle::showImage(string aName) {
    // display the image
    namedWindow(aName, WINDOW_NORMAL);
    cv::resizeWindow(aName, 1000, 1000);
    imshow(aName, image);

    //save image
    //imwrite( "../Images/Generation"+ to_string(generations) +".png", mPopulation);

    waitKey(0);

}

void ImageHandle::generateSurface(vector<float>& aPlane, vector<float>& stepSizes) {
    // Preconditions
    aPlane.clear();
    stepSizes.clear();

    // Set stepSizes
    stepSizes = vector<float>(image.cols, width/image.cols);

    // Set the point values
    Scalar test;
    for (unsigned int c = 0; c < image.cols; c++){
        for (unsigned int r = 0; r < image.rows; r++){
            test = image.at<uchar>(r, c);

            // Set Y component
            aPlane.push_back((float)(1.0 - (test[0]/255.0)*(minThikness/thikness)));

            // Set Z component
            aPlane.push_back((float)((double)r/(double)image.rows));
        }
    }
}

int ImageHandle::getRows() {
    return image.rows;
}

int ImageHandle::getCols() {
    return image.cols;
}

double ImageHandle::getImageHeight() {
    return width*((double)image.rows/(double)image.cols);
}

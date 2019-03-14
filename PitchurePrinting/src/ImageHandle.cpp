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
    stepSizes = vector<float>((unsigned long)image.cols, (float)(width/image.cols));

    // Set the point values
    Scalar test;
    for (unsigned int c = 0; c < image.cols; c++){
        for (unsigned int r = 0; r < image.rows; r++){
            test = image.at<uchar>(image.rows - r, image.cols - c);

            // Set Y component
            aPlane.push_back((float)(1.0 - (test[0]/255.0)*((thikness - minThikness)/thikness)));

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

void ImageHandle::addBorders(double borderThikness) {
    double pixelPrM = image.cols/width;

    auto L = (unsigned int)ceil(borderThikness*pixelPrM);

    copyMakeBorder( image, image, L, L, L, L, BORDER_CONSTANT, Scalar(0,0,0));

}

void ImageHandle::normalize() {
    cv::normalize(image, image, 0, 255, NORM_MINMAX, CV_8UC3);
}

// Angle is in degrees
void ImageHandle::addSupport(double angle) {
    // Preconditions
    if(angle < 10 || angle > 90){
        cerr << "Angle must be in the range 0 to 80 degrees, " << angle << " was typed." << endl;
    }

    // For each point in pitchure
    Scalar pixel1, pixel2;
    double maxColGrad = maximumOverhang(angle);
    for(int r = 0; r < image.rows - 1; r++){
        for (int c = 0; c < image.cols; c++){
            pixel1 = image.at<uchar>(r, c);
            pixel2 = image.at<uchar>(r + 1, c);

            // Check if the slope is to steep
            if(pixel1[0]-pixel2[0] < -maxColGrad){
                pixel2[0] = pixel1[0] + floor(maxColGrad);
                image.at<uchar>(r + 1, c) = (uchar)pixel2[0];
            }

        }
    }

}

double ImageHandle::maximumOverhang(double angle) {
    double mPrPix = (width/(double)image.cols);
    double mPrDc = (thikness-minThikness)/255.0;
    return (mPrPix*tan(angle*PI/180.0))/mPrDc;
}

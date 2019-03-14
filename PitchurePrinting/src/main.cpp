#include <iostream>
#include <vector>
#include "FeederDesigner.h"

#include "ImageHandle.h"
#include <rw/RobWork.hpp>       // for EAA calculation
#include <rw/geometry/TriMesh.hpp>
#include <rw/loaders/model3d/STLFile.hpp>
#include <rw/geometry/IndexedTriMesh.cpp>


using namespace std;
using namespace rw::math;




int main(){
    //string aFileName = "/home/daniel/Desktop/git/Hobby/PitchurePrinting/guess_ill_die.png";
    string aFileName = "/home/daniel/Desktop/Github/Hobby/PitchurePrinting/guess_ill_die.png";
    //string aFileName = "/home/daniel/Desktop/git/Hobby/PitchurePrinting/cat_and_pancakes.jpg";

    ImageHandle testImage(aFileName, IMAGE_WIDTH, IMAGE_THIKNESS, IMAGE_MINIMUM_THIKNESS);

    vector<float> crossSection;
    vector<float> stepSizes;

    //testImage.addBorders(0.005);

    testImage.showImage("original");
    testImage.normalize();
    testImage.showImage("normalized");



    testImage.generateSurface(crossSection, stepSizes);


    // Generarte feeder
    FeederDesigner feeder(testImage.getRows(), INWARDS_ANGLE, MINIMUM_RESOLUTION);
    feeder.scaleFeeder(IMAGE_THIKNESS*1000, testImage.getImageHeight()*1000, 1000);


    feeder.addLineFeeder(crossSection, testImage.getRows(), stepSizes);

    cout << "Save feeder " << endl;
    feeder.save("/home/daniel/Desktop/git/Hobby/PitchurePrinting/TestImage.stl",  CLIMB_ANGLE, IS_PRINTABLE, IS_BOWL_FEEDER, BOWL_RADIUS*1000);
    //feeder.save("/home/daniel/Desktop/git/9.semester/hardwareConfigurator/data/AI4_Project/sceneFiles/STL-files/tracks/Gen0/TestBowl00.stl",  CLIMB_ANGLE, IS_PRINTABLE, IS_BOWL_FEEDER, BOWL_RADIUS);
    //feeder.save("/home/daniel/Desktop/Github/9.semester/Designer/TestBowl0.stl",  CLIMB_ANGLE, IS_PRINTABLE, IS_BOWL_FEEDER, BOWL_RADIUS);          //*/


    cout << "Terminating" << endl;
    return 0;
}
















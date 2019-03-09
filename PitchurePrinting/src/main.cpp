#include <iostream>
#include <vector>
#include "PartDesigner.h"
#include "FeederDesigner.h"


#include <rw/RobWork.hpp>       // for EAA calculation
#include <rw/geometry/TriMesh.hpp>
#include <rw/loaders/model3d/STLFile.hpp>
#include <rw/geometry/IndexedTriMesh.cpp>


using namespace std;
using namespace rw::math;




int main(){

    /*/ load trimesh
    PartDesigner partHandle("/home/daniel/Desktop/Github/9.semester/Designer/CodeTesitng.stl");


    // modify trimesh
    partHandle.mutatePart(0.75, 0.7);   // prob, +-range


    // save new trimesh
    partHandle.savePart("/home/daniel/Desktop/Github/9.semester/Designer/CodeTesitngNew.stl");   //*/

    // Generarte feeder
    FeederDesigner feeder(NUMBER_OF_POINTS, INWARDS_ANGLE, MINIMUM_RESOLUTION);
    feeder.scaleFeeder(MAX_TRACK_WITH, MAX_TRACK_HEIGHT, MAX_SEGMENT_LENGTH);

    //
    vector<float> crossSection={ 0.5, 1, 0.5, 0.833333, 0.5, 0.666667, 0.5, 0.5, 0.332027, 0.568474, 0.166667, 0.529163, 0, 0.539281,
                                 0.5, 1, 0.538576, 0.871165, 0.5, 0.666667, 0.5, 0.5, 0.333333, 0.514581, 0.163302, 0.494388, 0, 0.616976,
                                 0.482482, 0.970536, 0.5, 0.833333, 0.5, 0.666667, 0.577097, 0.513703, 0.333333, 0.514581, 0.166667, 0.529163, 0.0622251, 0.48913,
                                 0.5, 1, 0.5, 0.833333, 0.440466, 0.609326, 0.5, 0.5, 0.394569, 0.532624, 0.223691, 0.503821, 0, 0.563627,
                                 0.5, 1, 0.5, 0.833333, 0.5, 0.666667, 0.42108, 0.56365, 0.384572, 0.45754, 0.166667, 0.529163, 0.073187, 0.568758,
                                 0.5, 1, 0.5, 0.833333, 0.5, 0.666667, 0.5, 0.5, 0.333333, 0.514581, 0.19127, 0.554357, 0, 0.543744,
                                 0.506431, 1.01003, 0.483455, 0.898021, 0.5, 0.666667, 0.5, 0.5, 0.333333, 0.514581, 0.14238, 0.466848, 0, 0.543744,
                                 0.474977, 1.02705, 0.5, 0.833333, 0.5, 0.666667, 0.511625, 0.475548, 0.333333, 0.514581, 0.213673, 0.4725, 0, 0.543875
    }; //*/
    /*vector<float> crossSection={ 0, 0.539281, 0.5, 0.833333, 0.5, 0.666667, 0.5, 0.5, 0.332027, 0.568474, 0.166667, 0.529163, 0.5, 1,
                                 0.5, 1, 0.538576, 0.871165, 0.5, 0.666667, 0.5, 0.5, 0.333333, 0.514581, 0.163302, 0.494388, 0, 0.616976,
                                 0.482482, 0.970536, 0.5, 0.833333, 0.5, 0.666667, 0.577097, 0.513703, 0.333333, 0.514581, 0.166667, 0.529163, 0.0622251, 0.48913,
                                 0.5, 1, 0.5, 0.833333, 0.440466, 0.609326, 0.5, 0.5, 0.394569, 0.532624, 0.223691, 0.503821, 0, 0.563627,
                                 0.5, 1, 0.5, 0.833333, 0.5, 0.666667, 0.42108, 0.56365, 0.384572, 0.45754, 0.166667, 0.529163, 0.073187, 0.568758,
                                 0.5, 1, 0.5, 0.833333, 0.5, 0.666667, 0.5, 0.5, 0.333333, 0.514581, 0.19127, 0.554357, 0, 0.543744,
                                 0.506431, 1.01003, 0.483455, 0.898021, 0.5, 0.666667, 0.5, 0.5, 0.333333, 0.514581, 0.14238, 0.466848, 0, 0.543744,
                                 0.474977, 1.02705, 0.5, 0.833333, 0.5, 0.666667, 0.511625, 0.475548, 0.333333, 0.514581, 0.213673, 0.4725, 0, 0.543875
    };*/


    vector<float> stepSize = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.3, 0.2};

    //feeder.addFlatTrack(0.5, INWARDS_ANGLE);

    feeder.addLineFeeder(crossSection, NUMBER_OF_POINTS, stepSize);
    //feeder.addDublicateTrack(0.5);

    //Visuals
    feeder.mutate(0.9, 0.05);
    feeder.printPlanePoints();


    //feeder.save("/home/daniel/Desktop/git/9.semester/Designer/TestBowl0.stl",  CLIMB_ANGLE, IS_PRINTABLE, IS_BOWL_FEEDER, BOWL_RADIUS);
    //feeder.save("/home/daniel/Desktop/git/9.semester/hardwareConfigurator/data/AI4_Project/sceneFiles/STL-files/tracks/Gen0/TestBowl00.stl",  CLIMB_ANGLE, IS_PRINTABLE, IS_BOWL_FEEDER, BOWL_RADIUS);
    feeder.save("/home/daniel/Desktop/Github/9.semester/Designer/TestBowl0.stl",  CLIMB_ANGLE, IS_PRINTABLE, IS_BOWL_FEEDER, BOWL_RADIUS);          //*/


    cout << "Terminating" << endl;
    return 0;
}
















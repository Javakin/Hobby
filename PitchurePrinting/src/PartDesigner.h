//
// Created by daniel on 2/18/19.
//

#ifndef GEN_PARTDESIGNER_H
#define GEN_PARTDESIGNER_H

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


#include <rw/RobWork.hpp>       // for EAA calculation
#include <rw/geometry/TriMesh.hpp>
#include <rw/loaders/model3d/STLFile.hpp>

using namespace std;
using namespace rw::math;
using namespace rw::geometry;

class PartDesigner {
public:
    PartDesigner(string fileName);
    PartDesigner();

    void loadPart(string fileName);
    void savePart(string fileName);


    void mutatePart(double mutationProb, double range);

private:

    bool bgetRand(double threshhold){
        bool output;
        if((double)rand()/RAND_MAX <threshhold){
            output = 1;
        }
        else {
            output = 0;
        }
        return output;
    }

    double dgetRand(){ // get random number between 0 and 1
        return (double)rand()/RAND_MAX;
    }

    unsigned int addPoint(Vector3D<float> p);





    // Coupled data structure
    vector<Vector3D<float>> vertecies;
    vector<Vector3D<unsigned int>> triangles;


};


#endif //GEN_PARTDESIGNER_H

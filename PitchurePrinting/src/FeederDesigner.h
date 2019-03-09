//
// Created by daniel on 2/20/19.
//

#ifndef GEN_FEEDERDESIGNER_H
#define GEN_FEEDERDESIGNER_H


#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>       /* trigonometry */
#include "earcut.hpp"   /* OpenSource EarCut algorithm, : https://github.com/mapbox/earcut.hpp*/


#include <rw/RobWork.hpp>       // for EAA calculation
#include <rw/geometry/TriMesh.hpp>
#include <rw/loaders/model3d/STLFile.hpp>
//#include <rwlibs/proximitystrategies/ProximityStrategyPQP.hpp>
#include <PQP/PQP.h>    // for collision detection


#define PI 3.14159265

#define INWARDS_ANGLE       0.0
#define CLIMB_ANGLE         0.0
#define NUMBER_OF_POINTS    7
#define MAX_TRACK_WITH      0.04       // measurements in meters
#define MAX_TRACK_HEIGHT    0.02       // measurements in meters
#define MAX_SEGMENT_LENGTH  0.08       // measurements in meters
#define MINIMUM_RESOLUTION  0.005      // measurements in meters
#define BOWL_RADIUS         0.1       // measurements in meters from the center of the shape
#define IS_PRINTABLE        0
#define IS_BOWL_FEEDER      0


using namespace std;
using namespace rw::math;
using namespace rw::geometry;


class FeederDesigner {
public:
    FeederDesigner(unsigned int pointsPrSecment, double inwardsAngle, double dt);

    bool addLineFeeder(vector<float> vdParameters, unsigned int pointsPrSegment, vector<float> stepSize);
    bool addFlatTrack(float stepSize, double inwardsAngle);
    bool addDublicateTrack(float stepSize);

    void mutate(double mutationProb, double range);
    void scaleFeeder(double width, double height, double length);
    void climpAngle(vector<vector<Vector3D<float>>> &aPlane, double climpAngle);
    void addBorderPoint(vector<vector<Vector3D<float>>> &aPlane, bool isPrintable);


    bool save(string fileName, double climbAngle, bool isPrintable, bool isBowlFeeder, double bowlRadius = 0.1);
    void printPlanePoints();

private:
    double dgetRand(){
        // get random number between 0 and 1
        return (double)rand()/RAND_MAX;
    }

    vector<Vector3D<float>> flatSection(unsigned int numberOfPoints, double inwardsAngle);

    // Trianglulation scheme
    vector<vector<Vector3D<float>>> triangulatePlane(vector<vector<Vector3D<float>>> aPlane, bool wrapAround);
    vector<vector<Vector3D<float>>> triangulatePlane1(vector<vector<Vector3D<float>>> aPlane, bool wrapAround);
    vector<vector<Vector3D<float>>> triangulatePlane2(vector<vector<Vector3D<float>>> aPlane, bool wrapAround);
    vector<vector<Vector3D<float>>> triangulatePlane3(vector<vector<Vector3D<float>>> aPlane, bool wrapAround);
    vector<vector<Vector3D<float>>> triangulatePolygon(vector<Vector3D<float>> aPolygon, bool normalDirection);

    void lineToHelicalBowl(vector<vector<Vector3D<float>>> &aPlane, double minR);

    vector<vector<Vector3D<float>>> uniformInterpolation(vector<vector<Vector3D<float>>> aPlane, double dt);

    void scale(vector<vector<Vector3D<float>>> &aPlane);

    // Validate feeder
    bool isValidFeeder(bool isPrintable);
    bool isValidSegment(vector<vector<Vector3D<float>>> triangles);
    bool isConnected(vector<Vector3D<float>> t1, vector<Vector3D<float>> t2);
    bool inCollision(vector<Vector3D<float>> t1, vector<Vector3D<float>> t2);


    vector<vector<Vector3D<float>>> plane;
    double feederLength, feederWith, feederHeight;
    double dt;


};


#endif //GEN_FEEDERDESIGNER_H

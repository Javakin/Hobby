//
// Created by daniel on 2/20/19.
//

#include "FeederDesigner.h"

FeederDesigner::FeederDesigner(unsigned int pointsPrSecment, double inwardsAngle, double aDt){
    /* initialize random seed: */
    srand (time(NULL));
    plane.clear();

    // Add the initial section
    plane.push_back(flatSection(pointsPrSecment, inwardsAngle));

    // set feeder resolution (only used if the feeder is a bowlfeeder)
    dt = aDt;

}

bool FeederDesigner::addLineFeeder(vector<float> vfParameters, unsigned int pointsPrSegment, vector<float> stepsize) {
    // Preconditions
    if (vfParameters.size() < pointsPrSegment * 4) {
        cerr << "insufficient number of points" << endl;
    }

    if (vfParameters.size() / (pointsPrSegment * 2) != (stepsize.size())) {
        cerr << "vector sizes does not math" << endl << vfParameters.size() / pointsPrSegment * 2 << " " << stepsize.size() - 1 << endl;
    }

    // Store the plane data
    vector<Vector3D<float>> aSegment(pointsPrSegment, Vector3D<float>());
    float currentIndex = plane[plane.size() - 1][0][0];

    for (unsigned int i = 0; i < vfParameters.size(); i += pointsPrSegment * 2) {

        currentIndex += stepsize[i / (pointsPrSegment * 2)];
        for (unsigned int j = 0; j < aSegment.size(); j++) {
            aSegment[j][0] = currentIndex;
            aSegment[j][1] = vfParameters[i + j*2];
            aSegment[j][2] = vfParameters[i + j*2 + 1];
        }

        plane.push_back(aSegment);
    }

    return true;
}


bool FeederDesigner::addFlatTrack(float stepSizes, double inwardsAngle) {

    // Generate the segment
    vector<Vector3D<float>> points = flatSection((unsigned int)plane[0].size(), inwardsAngle);

    // place the segment at the end of the feeder
    for (unsigned int p = 0; p < points.size(); p++){
        points[p][0] = plane[plane.size() - 1][0][0] + stepSizes;
    }

    // add the segmet to the plane mesh
    plane.push_back(points);

    return true;
}

void FeederDesigner::printPlanePoints(){
    for (unsigned int i = 0; i < plane.size(); i++){
        for (unsigned int j = 0; j < plane[i].size(); j++){
            cout << plane[i][j][1] << ", " <<  plane[i][j][2] << ", ";
        }
        cout << endl;
    }
}


bool FeederDesigner::save(string fileName,  double climbAngle, bool isPrintable, bool isBowlFeeder, double bowlRadius){
    // Preconditions
    if(plane.size() < 2){
        cerr << "Not enough sections for the plane";
    }

    if(plane[0].size() < 2){
        cerr << "Not enough points pr. section" << endl;
    }

    // Test for the surfaces not overlapping  todo
    /*if(!isValidFeeder(isPrintable)){
        cout << "Collision detected" << endl;
        //return false;
    }else{
        cout << "Fine Build" << endl;
    }*/


    // Transform the feeder
    vector<vector<Vector3D<float>>> aPlane = plane;

    if(isPrintable)
        addBorderPoint(aPlane, isPrintable);
    scale(aPlane);
    climpAngle(aPlane, climbAngle);

    if(isBowlFeeder)
        aPlane = uniformInterpolation(aPlane, dt);


    // Generate triangles
    vector<vector<Vector3D<float>>> triangles = triangulatePlane(aPlane, isPrintable);



    // Add end caps to close the shape
    if(isPrintable){
        vector<vector<Vector3D<float>>> cap = triangulatePolygon(aPlane[0], 0);
        triangles.insert( triangles.end(), cap.begin(), cap.end() );

        cap = triangulatePolygon(aPlane[aPlane.size() - 1], 1);
        triangles.insert( triangles.end(), cap.begin(), cap.end() );
    }

    // Convert to bowl feeder if needed
    if(isBowlFeeder)
        lineToHelicalBowl(triangles, bowlRadius);


    // Transfere it to a PlainTriMesh datatype for export
    rw::common::Ptr<PlainTriMesh<TriangleN1<float>>> output(new PlainTriMesh<TriangleN1<float>>);

    TriangleN1<float> aTriangle;

    for(unsigned int i = 0; i < triangles.size(); i++){
        aTriangle[0] = triangles[i][0];
        aTriangle[1] = triangles[i][1];
        aTriangle[2] = triangles[i][2];

        output->add(aTriangle);
    }

    // Export as STL file
    rw::loaders::STLFile::save(*output, fileName);
    return 1;
}

vector<vector<Vector3D<float>>> FeederDesigner::triangulatePlane(vector<vector<Vector3D<float>>> aPlane, bool wrapAround){
    return triangulatePlane1(aPlane, wrapAround);
}

// Generate left leaning pattern
vector<vector<Vector3D<float>>> FeederDesigner::triangulatePlane1(vector<vector<Vector3D<float>>> aPlane, bool wrapAround) {
    vector<vector<Vector3D<float>>> triangles;
    vector<Vector3D<float>> triangle(3);

    for (unsigned int segment = 0; segment < aPlane.size() - 1; segment++){
        for(unsigned int window = 0; window < aPlane[segment].size() - 1 + wrapAround; window++){
            triangle[0] = aPlane[segment + 1 ][window];
            triangle[1] = aPlane[segment     ][window];
            triangle[2] = aPlane[segment     ][(window + 1) % aPlane[segment].size()];

            triangles.push_back(triangle);

            triangle[0] = aPlane[segment     ][(window + 1) % aPlane[segment].size()];
            triangle[1] = aPlane[segment + 1 ][(window + 1) % aPlane[segment].size()];
            triangle[2] = aPlane[segment + 1 ][ window];

            triangles.push_back(triangle);
        }
    }

    return triangles;
}

// Generate right leaning pattern
vector<vector<Vector3D<float>>> FeederDesigner::triangulatePlane2(vector<vector<Vector3D<float>>> aPlane, bool wrapAround) {
    vector<vector<Vector3D<float>>> triangles;
    vector<Vector3D<float>> triangle(3);

    for (unsigned int segment = 0; segment < aPlane.size() - 1; segment++){
        for(unsigned int window = 0; window < aPlane[segment].size() - 1 + wrapAround; window++){
            triangle[0] = aPlane[segment     ][window];
            triangle[1] = aPlane[segment + 1 ][(window + 1) % aPlane[segment].size()];
            triangle[2] = aPlane[segment + 1 ][window];

            triangles.push_back(triangle);

            triangle[0] = aPlane[segment     ][window];
            triangle[1] = aPlane[segment     ][(window + 1) % aPlane[segment].size()];
            triangle[2] = aPlane[segment + 1 ][(window + 1) % aPlane[segment].size()];

            triangles.push_back(triangle);
        }
    }

    return triangles;
}

// Generate zig-zag pattern
vector<vector<Vector3D<float>>> FeederDesigner::triangulatePlane3(vector<vector<Vector3D<float>>> aPlane, bool wrapAround) {
    vector<vector<Vector3D<float>>> triangles;
    vector<Vector3D<float>> triangle(3);

    for (unsigned int segment = 0; segment < aPlane.size() - 1; segment++){
        for(unsigned int window = 0; window < aPlane[segment].size() - 1 + wrapAround; window++){
            if((segment + window) % 2){
                triangle[0] = aPlane[segment + 1 ][window];
                triangle[1] = aPlane[segment     ][window];
                triangle[2] = aPlane[segment     ][(window + 1) % aPlane[segment].size()];

                triangles.push_back(triangle);

                triangle[0] = aPlane[segment     ][(window + 1) % aPlane[segment].size()];
                triangle[1] = aPlane[segment + 1 ][(window + 1) % aPlane[segment].size()];
                triangle[2] = aPlane[segment + 1 ][ window];

                triangles.push_back(triangle);
            }
            else{
                triangle[0] = aPlane[segment     ][window];
                triangle[1] = aPlane[segment + 1 ][(window + 1) % aPlane[segment].size()];
                triangle[2] = aPlane[segment + 1 ][window];

                triangles.push_back(triangle);

                triangle[0] = aPlane[segment     ][window];
                triangle[1] = aPlane[segment     ][(window + 1) % aPlane[segment].size()];
                triangle[2] = aPlane[segment + 1 ][(window + 1) % aPlane[segment].size()];

                triangles.push_back(triangle);
            }
        }
    }

    return triangles;
}

void FeederDesigner::mutate(double mutationProb, double range) {
    for (unsigned int segment = 0; segment < plane.size(); segment++){
        for(unsigned int point = 0; point < plane[segment].size(); point++){
            if(dgetRand() < mutationProb){
                plane[segment][point][1] += range*(dgetRand()-0.5)*2;
                plane[segment][point][2] += range*(dgetRand()-0.5)*2;
            }
        }
    }
}

vector<Vector3D<float>> FeederDesigner::flatSection(unsigned int numberOfPoints, double inwardsAngle) {
    // Precondition
    vector<Vector3D<float>> output;
    vector<Vector3D<float>> points = {{0, 1, 0}, {0, 1, 0.5}, {0, 1, 1}};

    vector<unsigned int> segmentSize(2, 1);

    if(numberOfPoints > 3){
        segmentSize[0] = (numberOfPoints - 1)/2;
        segmentSize[1] = (numberOfPoints - 1)/2 + ((numberOfPoints - 1) % 2);
    }



    // Descritize the lines
    Vector3D<float> aPoint = points[0];
    output.push_back(points[0]);
    for (unsigned int line = 1; line < points.size(); line++){
        Vector3D<float> dt = (points[line]-points[line-1])/segmentSize[line-1];
        for (unsigned int i = 0; i < segmentSize[line-1]; i++){
            aPoint += dt;
            output.push_back(aPoint);
        }
    }

    return output;
}

void FeederDesigner::scaleFeeder(double width, double height, double length) {
    feederWith = width;
    feederHeight = height;
    feederLength = length;



}

void FeederDesigner::climpAngle(vector<vector<Vector3D<float>>> &aPlane, double climpAngle) {

    for (unsigned int i = 0; i < aPlane.size(); i++){
        for (unsigned int j = 0; j < aPlane[i].size(); j++){
            aPlane[i][j][2] += aPlane[i][j][0]*tan(climpAngle * PI / 180.0);
        }
    }
}

void FeederDesigner::addBorderPoint(vector<vector<Vector3D<float>>> &aPlane, bool isPrintable) {

    vector<Vector3D<float>> segment(aPlane[0].size() + 3);
    Vector3D<float> p1 = {0,0,1};
    Vector3D<float> p2 = {0,0,0};
    //Vector3D<float> p3 = {0,1,1};

    // Add the border points
    for(unsigned int i = 0; i < aPlane.size(); i++){
        p1[0] = aPlane[i][0][0];
        p2[0] = aPlane[i][0][0];
        //p3[0] = aPlane[i][0][0];



        if (isPrintable){
            aPlane[i].push_back(p1);
            aPlane[i].push_back(p2);
        }

        //aPlane[i].push_back(p3);
    }

}

bool FeederDesigner::addDublicateTrack(float stepSize) {
    // Precondition
    if(plane.size() == 0)
        cerr << "Dublicate track can not be called if no track exists" << endl;

    vector<Vector3D<float>> segment = plane[plane.size() - 1];

    // Translate the part with the stepsize along x
    for(unsigned int i = 0; i < segment.size(); i++){
        segment[i][0] += stepSize;
    }

    // Add segment to plane
    plane.push_back(segment);

    return true;
}

vector<vector<Vector3D<float>>> FeederDesigner::triangulatePolygon(vector<Vector3D<float>> aPolygon, bool normalDirection) {
    // **********************************************
    // Source: https://github.com/mapbox/earcut.hpp
    // **********************************************


    // The number type to use for tessellation
    using Coord = float;

    // The index type. Defaults to uint32_t, but you can also pass uint16_t if you know that your
    // data won't have more than 65536 vertices.
    using N = uint32_t;

    // Create array
    using Point = std::array<Coord, 2>;
    std::vector<std::vector<Point>> polygon(1);

    // Fill polygon structure with actual data. Any winding order works.
    // The first polyline defines the main polygon.
    for (unsigned int i = 0; i < aPolygon.size(); i++ ){
        polygon[0].push_back({aPolygon[i][1],aPolygon[i][2]});
    }

    // Run tessellation
    // Returns array of indices that refer to the vertices of the input polygon.
    // e.g: the index 6 would refer to {25, 75} in this example.
    // Three subsequent indices form a triangle. Output triangles are clockwise.
    std::vector<N> indices = mapbox::earcut<N>(polygon);

    // Convert back to the used datatype
    vector<vector<Vector3D<float>>> triangles;
    vector<Vector3D<float>> triangle(3);
    for (unsigned int i = 0; i < indices.size(); i +=3){
        if(normalDirection){
            triangle[0] = aPolygon[indices[i + 0]];
            triangle[1] = aPolygon[indices[i + 1]];
            triangle[2] = aPolygon[indices[i + 2]];
        }else{
            triangle[0] = aPolygon[indices[i + 2]];
            triangle[1] = aPolygon[indices[i + 1]];
            triangle[2] = aPolygon[indices[i + 0]];
        }


        triangles.push_back(triangle);
    }


    return triangles;
}

void FeederDesigner::lineToHelicalBowl(vector<vector<Vector3D<float>>> &triangles, double minR) {


    // convert the points of the triangles to wrap around the
    Vector3D<float> aTriangle;
    for (unsigned int i = 0; i < triangles.size(); i++){
        for (unsigned int j = 0; j < 3; j++){
            aTriangle = triangles[i][j];
            triangles[i][j][0] = (float)(((double)aTriangle[1] - feederWith/2 + minR)*cos( ((double)aTriangle[0])/(minR)));
            triangles[i][j][1] = (float)(((double)aTriangle[1] - feederWith/2 + minR)*sin( -((double)aTriangle[0])/(minR)));
        }
    }

}

void FeederDesigner::scale(vector<vector<Vector3D<float>>> &aPlane) {
    for (unsigned int i = 0; i < aPlane.size(); i++){
        for (unsigned int j = 0; j < aPlane[i].size(); j++){
            aPlane[i][j][0] *= feederLength;
            aPlane[i][j][1] *= feederWith;
            aPlane[i][j][2] *= feederHeight;
        }
    }
}

vector<vector<Vector3D<float>>> FeederDesigner::uniformInterpolation(vector<vector<Vector3D<float>>> aPlane, double dt) {
    // Preconditions
    if(aPlane.size() < 2){
        cerr << "No interpolation can be performed on a plane consisting of less then 2 segments" << endl;
    }
    if(aPlane[0].size() < 2){
        cerr << "Not enough points per segment, must be at least 2" << endl;
    }
    if(dt <= 0){
        cerr << "The reselution must be a positive real number larger then 0, and not " << dt << endl;
    }


    vector<vector<Vector3D<float>>> output = {aPlane[0]};
    vector<Vector3D<float>> vDirs = aPlane[0];
    vector<Vector3D<float>> vCurrentSegment = aPlane[0];

    unsigned int N;

    // Interpolate between all the segments
    for(unsigned int segment = 1; segment < aPlane.size(); segment++){
        // Compute number of steps needed
        N = (unsigned int)(ceil((aPlane[segment][0][0]-aPlane[segment-1][0][0]) / dt));
        if(N == 0)  N = 1;

        // Compute directions
        for (unsigned int p = 0; p < vDirs.size(); p++){
            vDirs[p] = (aPlane[segment][p] - vCurrentSegment[p])/N;
        }


        // Interpolate segments
        for(unsigned int i = 0; i < N; i++){
            for(unsigned int p = 0; p<vCurrentSegment.size(); p++){
                vCurrentSegment[p] += vDirs[p];
            }
            output.push_back(vCurrentSegment);
        }

    }

    return output;


}

// the functional part of this code is in
// source: https://github.com/GammaUNC/PQP
bool FeederDesigner::isValidFeeder(bool isPrintable){   // todo:
    // Preconditions
    if(plane.size() < 2){
        cerr << "Not enough sections for the plane";
    }

    if(plane[0].size() < 2){
        cerr << "Not enough points pr. section" << endl;
    }

    // Transform the feeder
    vector<vector<Vector3D<float>>> aPlane = plane;

    if(isPrintable)
        addBorderPoint(aPlane, isPrintable);


    // Perform validation
    vector<vector<Vector3D<float>>> triangles;
    for(unsigned int segment = 0; segment < plane.size() - 1; segment++){
        // For each block between segments generate triangles
        triangles = triangulatePlane(vector<vector<Vector3D<float>>>{aPlane[segment], aPlane[segment+1]}, isPrintable);

        // Check for collisions
        if(!isValidSegment(triangles))
            return false;
    }

    // if no collisions detected the build must be valid
    return true;
}

bool FeederDesigner::isValidSegment(vector<vector<Vector3D<float>>> triangles) {

    // For each triangle
    vector<Vector3D<float>> triangleTarget(3);
    for(unsigned int i = 0; i < triangles.size(); i++){
        // Select the current triangle
        triangleTarget = triangles[i];

        // Select all triangles not sharing the same points todo: this is not a perfect solution to the problem, make it better using pointers instead
        for(unsigned int j = i; j < triangles.size(); j++){
            if(!isConnected(triangleTarget, triangles[j])){
                // Check for collision
                //triangleOthers.push_back(triangles[j]);
                if(inCollision(triangleTarget, triangles[j])){
                    return false;
                }
            }
        }
    }


    return true;
}

bool FeederDesigner::isConnected(vector<Vector3D<float>> t1, vector<Vector3D<float>> t2) {
    // check if the two triangles share at least one point
    for (unsigned int i = 0; i < 3; i++){
        for(unsigned int j = 0; j < 3; j++){
            if(t1[i]==t2[j]){
                return true;
            }
        }
    }

    // No points match and thus the triangles are not directly connected
    return false;
}

bool FeederDesigner::inCollision(vector<Vector3D<float>> t1, vector<Vector3D<float>> t2) {
    // Perform validation
    PQP::PQP_Model m1, m2;

    // Begin model
    m1.BeginModel();
    m2.BeginModel();

    // Create some triangles
    PQP::PQP_REAL p1[3], p2[3], p3[3];
    PQP::PQP_REAL q1[3], q2[3], q3[3];

    // Initialize the points
    p1[0] = t1[0][0]; p1[1] = t1[0][1];  p1[2] = t1[0][2];
    p2[0] = t1[1][0]; p2[1] = t1[1][1];  p2[2] = t1[1][2];
    p3[0] = t1[2][0]; p3[1] = t1[2][1];  p3[2] = t1[2][2];

    q1[0] = t2[0][0];    q1[1] = t2[0][1];      q1[2] = t2[0][2];
    q2[0] = t2[1][0];    q2[1] = t2[1][1];      q2[2] = t2[1][2];
    q3[0] = t2[2][0];    q3[1] = t2[2][1];      q3[2] = t2[2][2];

    // Add triangles
    m1.AddTri(p1, p2, p3, 0);
    m2.AddTri(q1, q2, q3, 0);

    // End models, which builds the model
    m1.EndModel();
    m2.EndModel();

    // Set some dummy rotation and translation matrices
    PQP::PQP_REAL T[3] = {0};
    PQP::PQP_REAL R[3][3] = {0};

    R[0][0] = 1;     R[1][1] = 1;     R[2][2] = 1;

    // Collision
    PQP::PQP_CollideResult cres;
    PQP::PQP_Collide(&cres,R,T,&m1,R,T,&m2);

    int test = cres.Colliding();
    return test;
}




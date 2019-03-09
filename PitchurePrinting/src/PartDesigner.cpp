//
// Created by daniel on 2/18/19.
//

#include <rw/geometry/IndexedTriMesh.hpp>
#include "PartDesigner.h"


PartDesigner::PartDesigner(string fileName){
    PartDesigner();


    loadPart(fileName);
}

PartDesigner::PartDesigner(){
    /* initialize random seed: */
    srand (time(NULL));

    vertecies.clear();
    triangles.clear();
}





void PartDesigner::loadPart(string fileName) {
    // preconditions
    vertecies.clear();
    triangles.clear();

    // Load the Triangle
    cout << "getting started" << endl;
    rw::common::Ptr<PlainTriMesh<TriangleN1<float>>> plainTriangles = rw::loaders::STLFile::load(fileName);


    // Convert to costum datatype
    vector<TriangleN1<float>> mesh =  plainTriangles->getTriangles();
    TriangleN1<float> aTriangle;
    Vector3D<unsigned int> indexedTriangle;

    for (unsigned int i = 0; i < mesh.size(); i++){
        // Add vertecies to the list
        aTriangle = mesh[i];

        indexedTriangle[0] = addPoint(aTriangle[0]);
        indexedTriangle[1] = addPoint(aTriangle[1]);
        indexedTriangle[2] = addPoint(aTriangle[2]);

        // Add triangle to the list
        triangles.push_back(indexedTriangle);
    }
}

void PartDesigner::savePart(string fileName) {
    rw::common::Ptr<PlainTriMesh<TriangleN1<float>>> output(new PlainTriMesh<TriangleN1<float>>(triangles.size()));

    TriangleN1<float> aTriangle;

    for(unsigned int i = 0; i < triangles.size(); i++){
        aTriangle[0] = vertecies[triangles[i][0]];
        aTriangle[1] = vertecies[triangles[i][1]];
        aTriangle[2] = vertecies[triangles[i][2]];


        output->add(aTriangle);
    }

    rw::loaders::STLFile::save(*output, fileName);
}

unsigned int PartDesigner::addPoint(Vector3D<float> p) {
    // find point in the list and return index
    for(unsigned int i = 0; i < vertecies.size(); i++){
        if (p == vertecies[i])
            return i;
    }

    // point not found add it to the list
    vertecies.push_back(p);

    return vertecies.size() - 1;
}

void PartDesigner::mutatePart(double mutationProb, double range) {
    for (unsigned int v = 0; v < vertecies.size(); v++){
        for(unsigned int c = 0; c < 3; c++){
            if(dgetRand() < mutationProb){
                vertecies[v][c] += range*(dgetRand()-0.5)*2;
            }
        }
    }

}







//
//  ofxTubePrimitive.cpp
//  Created by Lukasz Karluk on 11/12/2013.
//

#include "ofxTubePrimitive.h"

//------------------------------------------------------------------------------
ofxTubePrimitive::ofxTubePrimitive() {
    init();
}

ofxTubePrimitive::ofxTubePrimitive(ofPolyline & poly, float radius) {
    init();
    setup(poly, radius);
}

ofxTubePrimitive::ofxTubePrimitive(vector<ofVec3f> & points, float radius) {
    init();
    setup(points, radius);
}

void ofxTubePrimitive::init() {
    tubeResolution = 20;
}

//------------------------------------------------------------------------------
ofxTubePrimitive::~ofxTubePrimitive() {
    //
}

//------------------------------------------------------------------------------
void ofxTubePrimitive::setup(ofPolyline & poly, float radius) {
    tubePoly = poly;

    tubeRadius.clear();
    tubeRadius.resize(tubePoly.size(), radius);
}

void ofxTubePrimitive::setup(vector<ofVec3f> & points, float radius) {
    tubePoly.clear();
    tubePoly.addVertices(points);
    
    tubeRadius.clear();
    tubeRadius.resize(points.size(), radius);
}

void ofxTubePrimitive::addPoint(ofVec3f point, float radius) {
    tubePoly.addVertex(point);
    tubeRadius.push_back(radius);
}

void ofxTubePrimitive::setResolution(int resolution) {
    tubeResolution = resolution;
}

//------------------------------------------------------------------------------
void ofxTubePrimitive::update() {

    if(mesh.get() == NULL) {
        *mesh = ofMesh();
    } else {
        mesh->clear();
    }
    
    for(int i=0; i<tubePoly.size(); i++) {
        const ofVec3f & point = tubePoly.getVertices()[i];
        const ofVec3f & normal = tubePoly.getNormalAtIndex(i);
        const ofVec3f & tangent = tubePoly.getTangentAtIndex(i);
        float radius = tubeRadius[i];
        
        ofVec3f vert;
        for(int j=0; j<tubeResolution; j++) {
            float p = j / (float)tubeResolution;
            float a = p * 360;
            vert = normal.rotated(a, tangent);
            vert *= radius;
            vert += point;
            
            mesh->addVertex(vert);
        }
    }
}

//------------------------------------------------------------------------------
void ofxTubePrimitive::drawTubeSkeleton() {
    tubePoly.draw();
}

void ofxTubePrimitive::drawTubeNormals(float normalLenght) {
    for(int i=0; i<tubePoly.size(); i++) {
        const ofVec3f & normal = tubePoly.getNormalAtIndex(i);
        const ofVec3f & p1 = tubePoly.getVertices()[i];
        ofVec3f p2 = p1 + normal * normalLenght;
        ofLine(p1, p2);
    }
}

void ofxTubePrimitive::drawTubeTangents(float tangentLength) {
    for(int i=0; i<tubePoly.size(); i++) {
        const ofVec3f & tangent = tubePoly.getTangentAtIndex(i);
        const ofVec3f & p1 = tubePoly.getVertices()[i];
        ofVec3f p2 = p1 + tangent * tangentLength;
        ofLine(p1, p2);
    }
}

void ofxTubePrimitive::drawTubeRings(float ringRadius) {

    for(int i=0; i<tubePoly.size(); i++) {
        ofNoFill();
        ofBeginShape();
        
        vector<ofVec3f> & verts = mesh->getVertices();
        for(int j=0; j<tubeResolution; j++) {
            int k = i * tubeResolution + j;
            ofVec3f & vert = verts[k];
            ofVertex(vert);
        }
        
        ofEndShape(true);
        ofFill();
    }
}



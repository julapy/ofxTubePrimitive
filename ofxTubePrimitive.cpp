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
    
    mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    for(int i=0; i<tubePoly.size(); i++) {
        const ofVec3f & p0 = tubePoly.getVertices()[i];
        const ofVec3f & n0 = tubePoly.getNormalAtIndex(i);
        const ofVec3f & t0 = tubePoly.getTangentAtIndex(i);
        float r0 = tubeRadius[i];
        
        ofVec3f v0;
        for(int j=0; j<tubeResolution; j++) {
            float p = j / (float)tubeResolution;
            float a = p * 360;
            v0 = n0.rotated(a, t0);
            
            mesh->addNormal(v0);
            
            v0 *= r0;
            v0 += p0;
            
            mesh->addVertex(v0);
        }
    }

    //--------------------------------------------------------------------------
    vector<ofVec3f> & verts = mesh->getVertices();
    int numOfVerts = verts.size();
    bool bLeftToRight;
    bool bRingEnd;
    int i0, i1;
    int k;
    
    int numOfTubeSections = tubePoly.size();
    for(int i=0; i<numOfTubeSections; i++) {

        bLeftToRight = (i % 2 == 0);
        k = 0;
        
        for(int j=0; j<tubeResolution+1; j++) {
            
            i0 = (i + 0) * tubeResolution + k;
            i1 = (i + 1) * tubeResolution + k;
            
            if(bLeftToRight == true) {
                k += 1;
                if(k > tubeResolution) {
                    k -= tubeResolution;
                }
            } else {
                k -= 1;
                if(k < 0) {
                    k += tubeResolution;
                }
            }

            if(i0 > numOfVerts - 1) {
                i0 -= numOfVerts;
            }
            if(i1 > numOfVerts - 1) {
                i1 -= numOfVerts;
            }
            
            mesh->addIndex(i0);
            mesh->addIndex(i1);
            
            bRingEnd = (j == tubeResolution);
            if(bRingEnd == true) {
                mesh->addIndex(i1);
            }
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

void ofxTubePrimitive::drawTubeRings() {

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



//
//  TrackedBlob.h
//  KinectDepthMotion
//
//  Created by James Alliban's MBP on 02/05/2014.
//
//


#pragma once


#include "ofMain.h"
#include "ofxCv.h"

class TrackedBlob
{
public:
    
	void setup(ofShader *_blobDifferncingShader, ofFbo *_blobDifferncingFbo, int sliceW, int sliceH);
	void update();
	void draw();
    void drawSmoothedPolylines();
    void drawSmoothedPolylinePoints();
    void drawSmoothedPolylineBoundingBox();
    void drawMotionDirection();
    void addNewPolyline(ofPolyline polyline);
    void resetAge();
    
    vector<ofPolyline> polylines;
    ofPolyline motionSmoothedPolyline;
    vector<ofVec2f> velocities;
    ofPolyline prevMotionPolyline;
    
    ofShader *blobDifferncingShader;
    ofFbo blobDifferncingFbo;
    
    int w, h;
    
    int label;
    int age;
    
    static float polylinesHistorySize;
    static float pointSmoothing;
    
    
    static float averageBoundingBoxHistorySize;
    static float motionProbPosInputMin;
    static float motionProbPosInputMax;
    static float motionProbPosOutputMin;
    static float motionProbPosOutputMax;
    static float motionProbSizeInputMin;
    static float motionProbSizeInputMax;
    static float motionProbSizeOutputMin;
    static float motionProbSizeOutputMax;
    
    int prevPolylineVertCount;
    
    float motionProbability;
    ofRectangle averageBoundingBox;
    ofVec2f averageVelocity;
};

//
//  DepthSlice.h
//  KinectDepthMotion
//
//  Created by James Alliban's MBP on 02/05/2014.
//
//


#pragma once


#include "ofMain.h"
#include "ofxCv.h"
#include "TrackedBlob.h"

class DepthSlice
{
public:
    
	void setup(int sliceW, int sliceH);
	void update(ofTexture *depthTex, int _minThreshold, int _maxThreshold);
	void drawImage();
    void drawContour();
    void drawContourInfo();
    void drawResampledPolylines();
    void drawResampledPolylinePoints();
    void drawResampledPolylineInfo();
    void drawMotionSmoothedBlobs();
    void drawMotionSmoothedBlobsPoints();
    void drawSmoothedPolylineBoundingBox();
    void drawMotionDirection();
    void setDrawPnt(float x, float y);
	void keyPressed (int key);
    
	ofxCv::ContourFinder contourFinder;
    ofxCv::RectTracker tracker;
    map<int, TrackedBlob> trackedBlobs;
    static float contourMinRadius;
    static float contourMaxRadius;
    static float contourThreshold;
    
    float minThreshold;
    float maxThreshold;
    
    ofPixels pix;
    
    ofShader *depthThresholdShader;
    ofShader *depthSmoothHShader;
    ofShader *depthSmoothVShader;
    ofShader *blobDifferncingShader;
    ofFbo *blobDifferncingFbo;
    ofFbo sliceFbo;
    ofFbo blurVFbo;
    static float blurAmount;
    static float blurIterations;
    ofPoint drawPnt;
    
    int w;
    int h;
    vector<ofPolyline> polylines;
    static float polySmoothing;
    static float resampledInputMin;
    static float resampledInputMax;
    static float resampledOutputMin;
    static float resampledOutputMax;
    static float resampleOutputStep;
    static float fixedResampleCount;
    static bool isResampleCountFixed;
};

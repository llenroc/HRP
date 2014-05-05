//
//  MotionTracker.h
//  KinectDepthMotion
//
//  Created by James Alliban's MBP on 01/05/2014.
//
//

#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "DepthSlice.h"

class MotionTracker
{
public:
    void setup(float depthW, float depthH);
    void update(ofTexture depthTex);
    void draw();
    
    
	ofxCv::ContourFinder contourFinder;
    vector<DepthSlice> depthSlices;
    float scale;
    float drawSizeW;
    float sliceW, sliceH;
    
    ofShader depthThresholdShader;
    ofShader depthSmoothHShader;
    ofShader depthSmoothVShader;
    ofShader blobDifferncingShader;
    ofFbo blobDifferncingFbo;
    ofFbo depthSmallFbo;
    float sliceAmount;
    
    
    float sliceMinThreshold;
    float sliceMaxThreshold;
    
    bool isDrawSlices;
    bool isDrawSliceImages;
    bool isDrawSliceContours;
    bool isDrawResampledPolylines;
    bool isDrawResampledPolylinePoints;
    bool isDrawResamplesPolylineInfo;
    bool isDrawMotionSmoothedPolylines;
    bool isDrawMotionSmoothedPolylinePoints;
    bool isDrawSmoothedPolylineBoundingBox;
    bool isDrawVelocities;
    
    bool isDrawCombinedVelocities;
    
    float combinedMotionScale;
};
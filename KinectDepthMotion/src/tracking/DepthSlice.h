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

class DepthSlice
{
public:
    
	void setup(int sliceW, int sliceH);
	void update(ofTexture *depthTex, int minThreshold, int maxThreshold);
	void draw();
    void debugDraw();
	void keyPressed (int key);
    
	ofxCv::ContourFinder contourFinder;
    static float contourMinRadius;
    static float contourMaxRadius;
    static float contourThreshold;
    ofPixels pix;
    
    ofShader *depthThresholdShader;
    ofShader *depthSmoothHShader;
    ofShader *depthSmoothVShader;
    ofFbo sliceFbo;
    ofFbo blurVFbo;
    static float blurAmount;
    static float blurIterations;
    
    int w;
    int h;
    vector<ofPolyline> polyLines;
};

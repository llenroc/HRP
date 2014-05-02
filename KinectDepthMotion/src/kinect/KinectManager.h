//
//  KinectManager.h
//  KinectDepthMotion
//
//  Created by James Alliban's MBP on 01/05/2014.
//
//

#pragma once


#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxCv.h"

class ofApp;

class KinectManager
{
public:
    
    KinectManager();
	
	void setup();
	void update(bool isPaused);
	void draw();
    void debugDraw();
    ofTexture getDepth();
	void keyPressed (int key);
	void exit();
	
    
    ofApp* app;
	ofxKinect kinect;
    
    ofTexture depthTex;
    bool isDrawDepth;
};

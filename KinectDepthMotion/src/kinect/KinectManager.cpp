//
//  KinectManager.cpp
//  KinectDepthMotion
//
//  Created by James Alliban's MBP on 01/05/2014.
//
//

#include "KinectManager.h"
#include "testApp.h"

KinectManager::KinectManager()
{
    
}


void KinectManager::setup()
{
    app = (ofApp*)ofGetAppPtr();
	kinect.init(false, false); // disable video image (faster fps)
	kinect.open();
}



void KinectManager::update()
{
	kinect.update();
	// there is a new frame and we are connected
	if(kinect.isFrameNew())
    {
        depthTex = kinect.getDepthTextureReference();
    }
}



void KinectManager::draw()
{
    if (!isDrawDepth) return;
    
    ofPushStyle();
    ofSetColor(255);
    kinect.drawDepth(0, 0);
    ofPopStyle();
}



void KinectManager::debugDraw()
{
}

ofTexture KinectManager::getDepth()
{
    return kinect.getDepthTextureReference();
}


void KinectManager::exit()
{
	kinect.close();
}


void KinectManager::keyPressed (int key)
{
	switch (key)
    {
		case 'o':
			kinect.open();
			break;

		case 'x':
			kinect.close();
			break;

//		case OF_KEY_UP:
//			angle++;
//			if(angle>30) angle=30;
//			kinect.setCameraTiltAngle(angle);
//			break;
//
//		case OF_KEY_DOWN:
//			angle--;
//			if(angle<-30) angle=-30;
//			kinect.setCameraTiltAngle(angle);
//			break;
	}
}

#include "testApp.h"


void testApp::setup()
{
    ofSetFullscreen(true);
	ofSetFrameRate(60);
	//ofSetLogLevel(OF_LOG_ERROR);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
	ofDisableSmoothing();
    ofSetMinMagFilters();
	ofEnableAlphaBlending();
    
    kinectDepthW = 640;
    kinectDepthH = 480;
    
    kinectManager.setup();
    motionTracker.setup(kinectDepthW, kinectDepthH);
    myGui.setup();
    
    isPaused = false;
}


void testApp::update()
{
    kinectManager.update(isPaused);
    motionTracker.update(kinectManager.getDepth());
}


void testApp::draw()
{
    ofBackground(40);
    
    ofPushMatrix();
    ofTranslate(0, 0);
    motionTracker.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight() - kinectDepthH);
    kinectManager.draw();
    ofPopMatrix();
}

void testApp::resetMotionTracker()
{
    motionTracker.setup(kinectDepthW, kinectDepthH);
}


void testApp::keyPressed(int key)
{
    if (key == 'p')
	{
        isPaused = !isPaused;
	}
    else if (key == 'f')
    {
        ofToggleFullscreen();
    }
}


void testApp::keyReleased(int key)
{
}


void testApp::mouseMoved(int x, int y)
{
}


void testApp::mouseDragged(int x, int y, int button)
{
}


void testApp::mousePressed(int x, int y, int button)
{
}


void testApp::mouseReleased(int x, int y, int button)
{
}


void testApp::windowResized(int w, int h)
{
}


void testApp::gotMessage(ofMessage msg)
{
}


void testApp::dragEvent(ofDragInfo dragInfo)
{
}

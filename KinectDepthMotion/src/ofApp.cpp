#include "ofApp.h"


void ofApp::setup()
{
    ofSetFullscreen(true);
	ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_ERROR);
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
#ifdef TARGET_OS_MAC_
    mainOutputSyphonServer.setName("UnityCam 3");
#endif
	
    
    isPaused = false;
}


void ofApp::update()
{
    if (!isPaused)
    {
        kinectManager.update();
        motionTracker.update(kinectManager.getDepth());
    }
}


void ofApp::draw()
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
    
    
#ifdef TARGET_OS_MAC_
	mainOutputSyphonServer.publishScreen();
#endif
}

void ofApp::resetMotionTracker()
{
	motionTracker.setup(kinectDepthW, kinectDepthH);
}


void ofApp::keyPressed(int key)
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


void ofApp::keyReleased(int key)
{
}


void ofApp::mouseMoved(int x, int y)
{
}


void ofApp::mouseDragged(int x, int y, int button)
{
}


void ofApp::mousePressed(int x, int y, int button)
{
}


void ofApp::mouseReleased(int x, int y, int button)
{
}


void ofApp::windowResized(int w, int h)
{
}


void ofApp::gotMessage(ofMessage msg)
{
}


void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

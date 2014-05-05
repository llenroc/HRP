#pragma once

#include "ofMain.h"
#include "KinectManager.h"
#include "MotionTracker.h"
#include "GUI.h"
#ifdef TARGET_OS_MAC_
#include "ofxSyphon.h"
#endif

//"Igloo Warper" - all 5 screens - 6400x720
// "Unitycam 3"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void resetMotionTracker();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int kinectDepthW, kinectDepthH;
    KinectManager kinectManager;
    MotionTracker motionTracker;
    GUI myGui;
    
#ifdef TARGET_OS_MAC_
    ofxSyphonServer mainOutputSyphonServer;
#endif
    bool isPaused;
};

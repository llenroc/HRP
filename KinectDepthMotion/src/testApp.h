#pragma once

#include "ofMain.h"
#include "KinectManager.h"
#include "MotionTracker.h"
#include "GUI.h"


class testApp : public ofBaseApp{

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
    
    bool isPaused;
};

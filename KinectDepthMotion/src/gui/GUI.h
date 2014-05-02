//
//  GUI.h
//  emptyExample
//
//  Created by James Alliban on 25/06/2013.
//
//

#pragma once

#include "ofMain.h"
#include "GUIManager.h"
#include "DepthSlice.h"

class testApp;

class GUI : public GUIManager
{
public:
    virtual void setup();
    virtual bool getVisible();
    
    void addVariousGUI();
    void addTrackingGUI();
    void addDebugGUI();
    void addBackgroundGUI();
    
    void trackingGUIEvent(ofxUIEventArgs &e);
    void debugGUIEvent(ofxUIEventArgs &e);
    void backgroundGUIEvent(ofxUIEventArgs &e);
    
    testApp* app;
};

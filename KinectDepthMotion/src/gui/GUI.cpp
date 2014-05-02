//
//  GUI.cpp
//  emptyExample
//
//  Created by James Alliban on 25/06/2013.
//
//

#include "GUI.h"
#include "testApp.h"


void GUI::setup()
{
    app = (testApp*)ofGetAppPtr();
    GUIManager::setup();
    
    addVariousGUI();
    addTrackingGUI();
    addDebugGUI();
    addBackgroundGUI();
    addGUIDesignGUI();
    
    setGUIColour();
}


bool GUI::getVisible()
{
    return GUIManager::getVisible();
}

void GUI::addVariousGUI()
{
    string title = "VARIOUS";
    ofxUICanvas* gui = getNewGUI(title);
    gui->addLabel("KEY", OFX_UI_FONT_MEDIUM);
    gui->addLabel("SPACE - SHOW/HIDE GUI", OFX_UI_FONT_SMALL);
    gui->addLabel("'[' - PREVIOUS GUI", OFX_UI_FONT_SMALL);
    gui->addLabel("']' - NEXT GUI", OFX_UI_FONT_SMALL);
    gui->addLabel("'p' - PAUSE SCENE", OFX_UI_FONT_SMALL);
    gui->addLabel("'f' - TOGGLE FULLSCREEN", OFX_UI_FONT_SMALL);
    
    finaliseCanvas(gui);
}

//
//void addTrackingGUI();
//void addDebugGUI();


void GUI::addTrackingGUI()
{
    string title = "TRACKING";
    ofxUICanvas* gui = getNewGUI(title);
    
    gui->addLabel("SLICE");
    gui->addSlider("SLICE TEXTURE SCALE", 0.1, 1, &app->motionTracker.scale, length, dim);
    gui->addSlider("BLUR AMOUNT", 0.1, 10, &DepthSlice::blurAmount, length, dim);
    gui->addSlider("BLUR ITERATIONS", 1, 10, &DepthSlice::blurIterations, length, dim);
    
    gui->addSlider("CONTOUR MIN RADIUS", 1, 100, &DepthSlice::contourMinRadius, length, dim);
    gui->addSlider("CONTOUR MAX RADIUS", 1, 1000, &DepthSlice::contourMaxRadius, length, dim);
    gui->addSlider("CONTOUR THRESHOLD", 1, 255, &DepthSlice::contourThreshold, length, dim);
    
	ofAddListener(gui->newGUIEvent, this, &GUI::trackingGUIEvent);
    finaliseCanvas(gui);
}



void GUI::addDebugGUI()
{
    string title = "DEBUG";
    ofxUICanvas* gui = getNewGUI(title);
    
    gui->addLabel("DRAWING");
    gui->addToggle("DRAW SLICES", &app->motionTracker.isDrawSlices, dim, dim);
    gui->addToggle("DRAW DEPTH", &app->kinectManager.isDrawDepth, dim, dim);
    
    gui->addLabel("SLICE");
    gui->addSlider("SLICE AMOUNT", 2, 30, &app->motionTracker.sliceAmount, length, dim);
    gui->addSlider("SLICE DRAW SIZE W", 50, 640, &app->motionTracker.drawSizeW, length, dim);
    
    
	ofAddListener(gui->newGUIEvent, this, &GUI::debugGUIEvent);
    finaliseCanvas(gui);
}



void GUI::addBackgroundGUI()
{
    string title = "BACKGROUND";
    ofxUICanvas* gui = getNewGUI(title);
    
    gui->addSpacer(length, 1);
    
//    gui->addSlider("BG FLAT RED", 0, 1, &app->scene.bgCol.r, length, dim);
//    gui->addSlider("BG FLAT GREEN", 0, 1, &app->scene.bgCol.g, length, dim);
//    gui->addSlider("BG FLAT BLUE", 0, 1, &app->scene.bgCol.b, length, dim);
//    gui->addSlider("BG FLAT ALPHA", 0, 1, &app->scene.bgCol.a, length, dim);
    
    
	ofAddListener(gui->newGUIEvent, this, &GUI::backgroundGUIEvent);
    finaliseCanvas(gui);
}







void GUI::trackingGUIEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    
    if (name == "SLICE TEXTURE SCALE")
        app->resetMotionTracker();
}



void GUI::debugGUIEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    
    if (name == "SLICE AMOUNT")
        app->resetMotionTracker();
}


void GUI::backgroundGUIEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    
}





//
//  GUI.cpp
//  emptyExample
//
//  Created by James Alliban on 25/06/2013.
//
//

#include "GUI.h"
#include "ofApp.h"


void GUI::setup()
{
    app = (ofApp*)ofGetAppPtr();
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
    
    gui->addLabel("SLICE STRUCTURE");
    gui->addSlider("SLICE AMOUNT", 2, 30, &app->motionTracker.sliceAmount, length, dim);
    gui->addSlider("SLICE TEXTURE SCALE", 0.1, 1, &app->motionTracker.scale, length, dim);
    gui->addRangeSlider("SLICE MIN/MAX THRESHOLD", 0, 255, &app->motionTracker.sliceMinThreshold, &app->motionTracker.sliceMaxThreshold, length, dim);
    
    gui->addSpacer(spacerW, spacerH);
    gui->addLabel("BLUR");
    gui->addSlider("BLUR AMOUNT", 0.1, 10, &DepthSlice::blurAmount, length, dim);
    gui->addSlider("BLUR ITERATIONS", 1, 10, &DepthSlice::blurIterations, length, dim);
    
    gui->addSpacer(spacerW, spacerH);
    gui->addLabel("CONTOUR");
    gui->addSlider("MIN RADIUS", 1, 100, &DepthSlice::contourMinRadius, length, dim);
    gui->addSlider("MAX RADIUS", 1, 1000, &DepthSlice::contourMaxRadius, length, dim);
    gui->addSlider("THRESHOLD", 1, 255, &DepthSlice::contourThreshold, length, dim);
    gui->addSlider("SMOOTHING", 1, 10, &DepthSlice::polySmoothing, length, dim);
    gui->addToggle("IS RESAMPLE COUNT FIXED", &DepthSlice::isResampleCountFixed, dim, dim);
    gui->addSlider("FIXED RESAMPLE COUNT", 10, 100, &DepthSlice::fixedResampleCount, length, dim);
    gui->addRangeSlider("ARC-RESAMPLE INPUT RANGE", 1, 1000, &DepthSlice::resampledInputMin, &DepthSlice::resampledInputMax, length, dim);
    gui->addRangeSlider("ARC-RESAMPLE OUTPUT RANGE", 1, 50, &DepthSlice::resampledOutputMin, &DepthSlice::resampledOutputMax, length, dim);
    gui->addSlider("ARC-RESAMPLE OUTPUT STEP", 1, 10, &DepthSlice::resampleOutputStep, length, dim);
    gui->addSlider("TRACKED BLOB HISTORY SIZE", 2, 10, &TrackedBlob::polylinesHistorySize, length, dim);
    gui->addSlider("TRACKED BLOB POINT SMOOTHING", 0.001, 1.0, &TrackedBlob::pointSmoothing, length, dim);
    
    gui->addSpacer(spacerW, spacerH);
    gui->addLabel("MOTION PROBABILITY CHECK");
    gui->addSlider("AVERAGE B BOX HISTORY SIZE", 2, 10, &TrackedBlob::averageBoundingBoxHistorySize, length, dim);
    gui->addRangeSlider("BBOX POS DIFF INPUT RANGE", 0, 10, &TrackedBlob::motionProbPosInputMin, &TrackedBlob::motionProbPosInputMax, length, dim);
    gui->addRangeSlider("BBOX POS DIFF OUTPUT RANGE", 0.1, 1, &TrackedBlob::motionProbPosOutputMin, &TrackedBlob::motionProbPosOutputMax, length, dim);
    gui->addRangeSlider("BBOX SIZE DIFF INPUT RANGE", 0, 100, &TrackedBlob::motionProbSizeInputMin, &TrackedBlob::motionProbSizeInputMax, length, dim);
    gui->addRangeSlider("BBOX SIZE DIFF OUTPUT RANGE", 0.1, 1, &TrackedBlob::motionProbSizeOutputMin, &TrackedBlob::motionProbSizeOutputMax, length, dim);


    
	ofAddListener(gui->newGUIEvent, this, &GUI::trackingGUIEvent);
    finaliseCanvas(gui);
}



void GUI::addDebugGUI()
{
    string title = "DEBUG";
    ofxUICanvas* gui = getNewGUI(title);
    
    gui->addLabel("SLICES");
    gui->addSlider("SLICE DRAW SIZE W", 50, 640, &app->motionTracker.drawSizeW, length, dim);
    gui->addToggle("DRAW SLICES", &app->motionTracker.isDrawSlices, dim, dim);
    gui->addToggle("DRAW SLICE IMAGES", &app->motionTracker.isDrawSliceImages, dim, dim);
    gui->addToggle("DRAW SLICE CONTOURS", &app->motionTracker.isDrawSliceContours, dim, dim);
    gui->addToggle("DRAW RESAMPLED POLYLINES", &app->motionTracker.isDrawResampledPolylines, dim, dim);
    gui->addToggle("DRAW RESAMPLED POLYLINE PNTS", &app->motionTracker.isDrawResampledPolylinePoints, dim, dim);
    gui->addToggle("DRAW RESAMPLED POLYLINE INFO", &app->motionTracker.isDrawResamplesPolylineInfo, dim, dim);
    gui->addToggle("DRAW MOTION SMOOTH POLYLINES", &app->motionTracker.isDrawMotionSmoothedPolylines, dim, dim);
    gui->addToggle("DRAW MOTION SMOOTH POLYLINE PNTS", &app->motionTracker.isDrawMotionSmoothedPolylinePoints, dim, dim);
    gui->addToggle("DRAW MOTION SMOOTH BOUNDING BOX", &app->motionTracker.isDrawSmoothedPolylineBoundingBox, dim, dim);
    gui->addToggle("DRAW VELOCITIES", &app->motionTracker.isDrawVelocities, dim, dim);
    
    
    gui->addSpacer(spacerW, spacerH);
    gui->addLabel("MOTION");
    gui->addToggle("DRAW COMBINED VELOCITIES", &app->motionTracker.isDrawCombinedVelocities, dim, dim);
    gui->addSlider("COMBINED VELOCITIES SCALE", 1, 20, &app->motionTracker.combinedMotionScale, length, dim);
    
    
    gui->addSpacer(spacerW, spacerH);
    gui->addLabel("DEPTH");
    gui->addToggle("DRAW DEPTH", &app->kinectManager.isDrawDepth, dim, dim);
    
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
    else if (name == "SLICE AMOUNT")
        app->resetMotionTracker();
}



void GUI::debugGUIEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    
}


void GUI::backgroundGUIEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    
}





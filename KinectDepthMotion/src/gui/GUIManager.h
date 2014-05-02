//
//  GUIManager.h
//  emptyExample
//
//  Created by James Alliban on 25/06/2013.
//
//

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxUI.h"

// gui titles
#define SIDE_TITLE "SIDE"

#define SIDE_AUTO_SAVE "AUTO SAVE"
#define SIDE_SAVE "SAVE"
#define SIDE_NEXT "NEXT"
#define SIDE_PREVIOUS "PREV"


class GUIManager
{
public:
    virtual void setup();
    virtual void setGUIPath(string _guiPath);
    virtual bool getVisible();
    virtual bool isMouseOver();
    virtual void addGUIDesignGUI();
    virtual void guiDesignGUIEvent(ofxUIEventArgs &e);
    
    ofColor colorBack;
    ofColor colorFill;
    ofColor colorHighlight;
    ofColor colorOutline;
    ofColor colorOutlineHighlight;
    ofColor colorPadded;
    ofColor colorPaddedHighlight;
    
protected:
    
    virtual void buildSideBarGUI();
    virtual ofxUICanvas* getNewGUI(string title);
    virtual void keyPressed(ofKeyEventArgs& eventArgs);
    virtual void mouseReleased(ofMouseEventArgs& eventArgs);
    virtual void exit(ofEventArgs& eventArgs);
    virtual void finaliseCanvas(ofxUICanvas* gui, bool isSidebar = false);
    virtual void sideBarGUIEvent(ofxUIEventArgs &e);
    void guiEvent(ofxUIEventArgs &e);
    
    void nextPage();
    void previousPage();
    void showGUI(string guiName);
    void toggleVisible();
    
    void saveSettings();
    
    void setGUIColour();
    
    ofxUICanvas* sideBarGui;
    
    vector<ofxUICanvas *> guiPages;
    int currentUIID;
    
    string guiPath;
    
    float dim;
    float xInit;
    float length;
    float halfLength;
    float sidebarLength;
    float canvasW;
    float canvasH;
    float toggleSize;
    float spacerW, spacerH;
    bool isVisible;
    bool isAutoSave;
};

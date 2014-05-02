//
//  MotionTracker.cpp
//  KinectDepthMotion
//
//  Created by James Alliban's MBP on 01/05/2014.
//
//


// TODO:
// Create DepthSlice class.
// Find contours in slices
// Apply to polylines then resample
// Smooth the depth slice by apply GPU blurring then adjust the contourFinder threshold
// Look at example-contours-advanced
// Contour tracking - see example app
// Calculate and display the velocity of centroid and individual contour points
// Maybe calculate and display predicted next position
//
//

// THOUGHTS:
// Look into blob tracking
// Blobs only become active after a lifespan of n
// What happens when a blob starts to shrink (body part moving out of threshold) - most velocities point inward
// Split blob?
// To avoid incorrect motion from blobs getting bigger/smaller (users body parts entering/exiting slice thresholds):
// - Scale down velocity for blobs rapidly changing size.
// - particles are more likely to appear when the blob maintain a consistant size.
// Possible issue with Convex Hulls - https://github.com/kylemcdonald/ofxCv/issues/83



#include "MotionTracker.h"


void MotionTracker::setup(float depthW, float depthH)
{
    printf("scale = %f \n", scale);
    sliceW = depthW * scale;
    sliceH = depthH * scale;
    
    ofFbo::Settings settings;
    settings.width = sliceW;
    settings.height = sliceH;
    settings.internalformat = GL_RGBA;
    
    depthSmallFbo.allocate(settings);
    
//	contourFinder.setMinAreaRadius(contourMinRadius);
//	contourFinder.setMaxAreaRadius(contourMaxRadius);
    
    depthThresholdShader.load("shaders/DepthThresholdShader");
    depthSmoothHShader.load("shaders/DepthSmoothShaderH");
    depthSmoothVShader.load("shaders/DepthSmoothShaderV");
    
    depthSlices.clear();
    for (int i = 0; i < (int)sliceAmount; i++)
    {
        DepthSlice depthSlice;
        depthSlice.setup(depthSmallFbo.getWidth(), depthSmallFbo.getHeight());
        depthSlice.depthThresholdShader = &depthThresholdShader;
        depthSlice.depthSmoothHShader = &depthSmoothHShader;
        depthSlice.depthSmoothVShader = &depthSmoothVShader;
        depthSlices.push_back(depthSlice);
    }
}


void MotionTracker::update(ofTexture depthTex)
{
    depthSmallFbo.begin();
    depthTex.draw(0, 0, depthSmallFbo.getWidth(), depthSmallFbo.getHeight());
    depthSmallFbo.end();
    
    
    if (depthSlices.size() == 0) return;
    
    for (int i = 0; i < depthSlices.size(); i++)
    {
        float thresholdSection = 255 / depthSlices.size();
        
        int minThreshold = 255 - (i * thresholdSection + thresholdSection);
        int maxThreshold = 255 - (i * thresholdSection);
        
        depthSlices[i].update(&depthSmallFbo.getTextureReference(), minThreshold, maxThreshold);
    }
}


void MotionTracker::draw()
{
    if (depthSlices.size() == 0 || !isDrawSlices) return;
    
    ofSetColor(255);
    int slicesPerRow = ofGetWidth() / drawSizeW; /// (sliceW * drawScale);
    float drawScale = drawSizeW / sliceW;
    
    for (int i = 0; i < depthSlices.size(); i++)
    {
        int x = (i % slicesPerRow * sliceW * drawScale);
        int y = (int)(i / slicesPerRow) * (sliceH * drawScale);
        ofPushMatrix();
        ofTranslate(x, y);
        ofScale(drawScale, drawScale);
        depthSlices[i].draw();
        ofPopMatrix();
        ofPushStyle();
        ofNoFill();
        ofSetColor(0, 255, 0, 255);
        ofRect(x, y, sliceW * drawScale, sliceH * drawScale);
        ofPopStyle();
    }
}

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
// Apply blur once to the depthmap instead of every slice. In blurring necessary when I can smooth polylines?
// Do I need to perform threshold slicing to create slice images? Maybe just adjust contour thresholds in each DepthSlice
// Look into blob tracking
// Blobs only become active after a lifespan of n
// What happens when a blob starts to shrink (body part moving out of threshold) - most velocities point inward
// Split blob?
// To avoid incorrect motion from blobs getting bigger/smaller (users body parts entering/exiting slice thresholds):
// - Scale down velocity for blobs rapidly changing size.
// - particles are more likely to appear when the blob maintain a consistant size.
// Possible issue with Convex Hulls - https://github.com/kylemcdonald/ofxCv/issues/83
// Only apply motion if is coniuous over n frames, i.e. motion vector needs to move in the same direction for several frames
// before it is recognised as real motion - cuts out false motion from flickering
// Really need to keep motion data in constant flow. Cancelling out motion when polyline counts change is not good. As counts increase/decrease
//   the app should be able to handle it.


// Changes made whilst in car
// --------------------------
// draw resampled info added
// threshold added for slice min/max distance - check MotionTracker::update()
// apply blurring to depthmap instead of all slices


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
    blobDifferncingShader.load("shaders/BlobDifferncing");
    
    blobDifferncingFbo.allocate(settings);
    
    depthSlices.clear();
    for (int i = 0; i < (int)sliceAmount; i++)
    {
        DepthSlice depthSlice;
        depthSlice.setup(depthSmallFbo.getWidth(), depthSmallFbo.getHeight());
        depthSlice.depthThresholdShader = &depthThresholdShader;
        depthSlice.depthSmoothHShader = &depthSmoothHShader;
        depthSlice.depthSmoothVShader = &depthSmoothVShader;
        depthSlice.blobDifferncingShader = &blobDifferncingShader;
        depthSlice.blobDifferncingFbo = &blobDifferncingFbo;
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
        
        int minThreshold = ofMap(255 - (i * thresholdSection + thresholdSection), 0, 255, sliceMinThreshold, sliceMaxThreshold);
        int maxThreshold = ofMap(255 - (i * thresholdSection), 0, 255, sliceMinThreshold, sliceMaxThreshold);
        
        depthSlices[i].update(&depthSmallFbo.getTextureReference(), minThreshold, maxThreshold);
        //depthSlices[i].update(&depthSmallFbo.getTextureReference(), 200, 255);
    }
    
    for (int i = 0; i < depthSlices.size(); i++)
    {
        
    }
}


void MotionTracker::draw()
{
    if (depthSlices.size() == 0 || !isDrawSlices) return;
    
    ofSetColor(255);
    int slicesPerRow = ofGetWidth() / drawSizeW;
    float drawScale = drawSizeW / sliceW;
    
    // set drawing x/y for each slice
    for (int i = 0; i < depthSlices.size(); i++)
        depthSlices[i].setDrawPnt((i % slicesPerRow * sliceW * drawScale), (int)(i / slicesPerRow) * (sliceH * drawScale));
    
    
    if (isDrawSliceImages)
    {
        for (int i = 0; i < depthSlices.size(); i++)
        {
            DepthSlice *depthSlice = &depthSlices[i];
            ofPushMatrix();
            ofTranslate(depthSlice->drawPnt.x, depthSlice->drawPnt.y);
            ofScale(drawScale, drawScale);
            depthSlice->drawImage();
            ofPopMatrix();
            
            ofPushStyle();
            ofNoFill();
            ofSetColor(0, 255, 0, 255);
            ofRect(depthSlice->drawPnt.x, depthSlice->drawPnt.y, sliceW * drawScale, sliceH * drawScale);
            ofPopStyle();
        }
    }
    
    if (isDrawSliceContours)
    {
        for (int i = 0; i < depthSlices.size(); i++)
        {
            DepthSlice *depthSlice = &depthSlices[i];
            ofPushMatrix();
            ofTranslate(depthSlice->drawPnt.x, depthSlice->drawPnt.y);
            ofScale(drawScale, drawScale);
            depthSlice->drawContour();
            ofPopMatrix();
            ofPushStyle();
        }
    }
    
    ofSetCircleResolution(6);
    for (int i = 0; i < depthSlices.size(); i++)
    {
        DepthSlice *depthSlice = &depthSlices[i];
        ofPushMatrix();
        ofTranslate(depthSlice->drawPnt.x, depthSlice->drawPnt.y);
        ofScale(drawScale, drawScale);
        
        if (isDrawResampledPolylines)
            depthSlice->drawResampledPolylines();
        if (isDrawResampledPolylinePoints)
            depthSlice->drawResampledPolylinePoints();
        if (isDrawResamplesPolylineInfo)
            depthSlice->drawResampledPolylineInfo();
        if (isDrawMotionSmoothedPolylines)
            depthSlice->drawMotionSmoothedBlobs();
        if (isDrawMotionSmoothedPolylinePoints)
            depthSlice->drawMotionSmoothedBlobsPoints();
        if (isDrawSmoothedPolylineBoundingBox)
            depthSlice->drawSmoothedPolylineBoundingBox();
        if (isDrawVelocities)
            depthSlice->drawMotionDirection();
        
        ofPopMatrix();
        ofPushStyle();
    }
    
    for (int i = 0; i < depthSlices.size(); i++)
    {
        ofPushMatrix();
        ofTranslate(100, ofGetHeight() - 640);
        //depthSlices[i].drawContour();
        //depthSlices[i].drawPolylinePoints();
        ofPopMatrix();
    }
    
    if (isDrawCombinedVelocities)
    {
        
        ofPushMatrix();
        ofTranslate(300, 0);
        ofScale(combinedMotionScale, combinedMotionScale);
        for (int i = 0; i < depthSlices.size(); i++)
        {
            DepthSlice *depthSlice = &depthSlices[i];
            depthSlice->drawMotionDirection();
        }
        ofPopMatrix();
    }
}

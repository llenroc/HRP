//
//  DepthSlice.cpp
//  KinectDepthMotion
//
//  Created by James Alliban's MBP on 02/05/2014.
//
//

#include "DepthSlice.h"

using namespace ofxCv;
using namespace cv;

float DepthSlice::blurAmount;
float DepthSlice::blurIterations;
float DepthSlice::contourMinRadius;
float DepthSlice::contourMaxRadius;
float DepthSlice::contourThreshold;
float DepthSlice::polySmoothing;

float DepthSlice::resampledInputMin;
float DepthSlice::resampledInputMax;
float DepthSlice::resampledOutputMin;
float DepthSlice::resampledOutputMax;
float DepthSlice::resampleOutputStep;
float DepthSlice::fixedResampleCount;
bool DepthSlice::isResampleCountFixed;

void DepthSlice::setup(int sliceW, int sliceH)
{
	contourFinder.setMinAreaRadius(contourMinRadius);
	contourFinder.setMaxAreaRadius(contourMaxRadius);
    
    w = sliceW;
    h = sliceH;
    
    ofFbo::Settings settings;
    settings.width = w;
    settings.height = h;
    settings.internalformat = GL_RGBA;
    
    sliceFbo.allocate(settings);
    blurVFbo.allocate(settings);
}


void DepthSlice::update(ofTexture *depthTex, int _minThreshold, int _maxThreshold)
{
    
    minThreshold = _minThreshold;
    maxThreshold = _maxThreshold;
    
    // create slice apply threshold to depth image on GPU
    ofSetColor(255);
    sliceFbo.begin();
    depthThresholdShader->begin();
    depthThresholdShader->setUniformTexture("tex0", *depthTex, depthTex->getTextureData().textureID);
    depthThresholdShader->setUniform1i("minThreshold", minThreshold);
    depthThresholdShader->setUniform1i("maxThreshold", maxThreshold);
    depthTex->draw(0, 0);
    depthThresholdShader->end();
    sliceFbo.end();
    
    // smooth the slice on GPU
    for (int i=0; i < blurIterations; i++)
    {
        //apply horizontal blur to sliceFbo and put the result in blurVFbo
        blurVFbo.begin();
        depthSmoothHShader->begin();
        depthSmoothHShader->setUniform1f("amt", blurAmount);
        sliceFbo.draw(0, 0);
        depthSmoothHShader->end();
        blurVFbo.end();
        
        //apply vertical blur to blurVFbo and put this back in sliceFbo
        sliceFbo.begin();
        depthSmoothVShader->begin();
        depthSmoothVShader->setUniform1f("amt", blurAmount);
        blurVFbo.draw(0,0);
        depthSmoothVShader->end();
        sliceFbo.end();
    }
    
    
    // find contours in slices
    sliceFbo.getTextureReference().readToPixels(pix);
    contourFinder.setMinAreaRadius(contourMinRadius);
    contourFinder.setMaxAreaRadius(contourMaxRadius);
    contourFinder.setThreshold(contourThreshold);
    contourFinder.findContours(pix);
    
    
	tracker = contourFinder.getTracker();
    
    
    // apply to polylines then resample and smooth
    polylines.clear();
    if (contourFinder.getContours().size() > 0)
    {
        vector<int> activeContourLabels;
        for (int i = 0; i < contourFinder.getPolylines().size(); i++)
        {
            int label = contourFinder.getLabel(i);
            float arcLength = contourFinder.getArcLength(i);
            ofPolyline polyline = contourFinder.getPolyline(i);
            
            float mappedResample = ofMap(arcLength, resampledInputMin, resampledInputMax, resampledOutputMin, resampledOutputMax);
            // No longer using stepped smooth
            //int steppedSmooth = mappedResample - fmod(mappedResample, resampleOutputStep);
            polyline = polyline.getResampledByCount(isResampleCountFixed ? fixedResampleCount : mappedResample);
            polyline = polyline.getSmoothed(polySmoothing);
            polylines.push_back(polyline);
            activeContourLabels.push_back(label);
            
            // add a new TrackedBlob if contour is new i.e. there are none using this label
            if (trackedBlobs.count(label) == 0)
            {
                TrackedBlob trackedBlob;
                trackedBlob.label = label;
                trackedBlob.setup(blobDifferncingShader, blobDifferncingFbo, w, h);
                trackedBlobs[label] = trackedBlob;
            }
            
            // loop through all tracked blobs to find if any need to be removed
            vector<int> keysToRemove;
            map<int, TrackedBlob>::iterator iter;
            for (iter = trackedBlobs.begin(); iter != trackedBlobs.end(); ++iter)
            {
                int key = iter->first;
                bool isBlobLabelStillActive = false;
                // for each TrackedBlob, check all the contours to see if there is a match. If not, add this key to a vector of keys to remove.
                for (int j = 0; j < contourFinder.size(); j++)
                {
                    int contourLabel = contourFinder.getLabel(j);
                    if (key == contourLabel) isBlobLabelStillActive = true;
                }
                if (!isBlobLabelStillActive) keysToRemove.push_back(key);
            }
            // remove dead maps
            for (int j = 0; j < keysToRemove.size(); j++)
                trackedBlobs.erase(keysToRemove[i]);
            
            
            trackedBlobs[label].addNewPolyline(polyline);
        }
    }
    
    
    // perform motion smoothing on blobs
    map<int, TrackedBlob>::iterator iter;
    for (iter = trackedBlobs.begin(); iter != trackedBlobs.end(); ++iter)
        iter->second.update();
}


void DepthSlice::drawImage()
{
    ofPushStyle();
    ofSetColor(255);
    sliceFbo.draw(0, 0);
    ofPopStyle();
    string msg = "Blobs:" + ofToString(trackedBlobs.size()) + ", MinTh:" + ofToString(minThreshold) + ", MaxTh:" + ofToString(maxThreshold);
    ofDrawBitmapString(msg, ofPoint(5, h - 2));
}


void DepthSlice::drawContour()
{
    ofPushStyle();
    ofSetColor(255, 0, 0, 255);
    contourFinder.draw();
    ofPopStyle();
}


void DepthSlice::drawContourInfo()
{
    for(int i = 0; i < contourFinder.size(); i++)
    {
        int label = contourFinder.getLabel(i);
        string msg = "L:" + ofToString(label) + "\n" + "A:" + ofToString(tracker.getAge(label));
        cv::Rect bb = contourFinder.getBoundingRect(i);
        ofDrawBitmapString(msg, ofPoint(bb.x + bb.width, bb.y + bb.height));
    }
}


void DepthSlice::drawResampledPolylines()
{
    ofPushStyle();
    ofSetColor(255, 0, 0, 255);
    for (int i = 0; i < polylines.size(); i++)
    {
        ofSetColor(0, 255, 0, 255);
        polylines[i].draw();
    }
    ofPopStyle();
}


void DepthSlice::drawResampledPolylinePoints()
{
    ofPushStyle();
    for (int i = 0; i < polylines.size(); i++)
    {
        ofPolyline *polyline = &polylines[i];
        ofSetColor(255, 0, 0, 255);
        for (int j = 0; j < polyline->size(); j++)
        {
            ofPoint pnt = polylines[i][j];
            ofSetColor(ofMap(j, 0, polyline->size(), 0, 255),
                       ofMap(j, 0, polyline->size(), 255, 0),
                       0);
            ofCircle(pnt, 0.7);
        }
    }
    ofPopStyle();
}


void DepthSlice::drawResampledPolylineInfo()
{
    for(int i = 0; i < polylines.size(); i++)
    {
        int label = contourFinder.getLabel(i);
        string msg = "PC:" + ofToString(polylines[i].size()) + "\n" + "P:" + ofToString(polylines[i].getPerimeter());
        ofRectangle bb = polylines[i].getBoundingBox();
        ofDrawBitmapString(msg, ofPoint(bb.x + bb.width, bb.y + bb.height));
    }
}


void DepthSlice::drawMotionSmoothedBlobs()
{
    map<int, TrackedBlob>::iterator iter;
    for (iter = trackedBlobs.begin(); iter != trackedBlobs.end(); ++iter)
    {
        iter->second.drawSmoothedPolylines();
    }
}


void DepthSlice::drawMotionSmoothedBlobsPoints()
{
    map<int, TrackedBlob>::iterator iter;
    for (iter = trackedBlobs.begin(); iter != trackedBlobs.end(); ++iter)
    {
        iter->second.drawSmoothedPolylinePoints();
    }
}


void DepthSlice::drawSmoothedPolylineBoundingBox()
{
    map<int, TrackedBlob>::iterator iter;
    for (iter = trackedBlobs.begin(); iter != trackedBlobs.end(); ++iter)
    {
        iter->second.drawSmoothedPolylineBoundingBox();
    }
}


void DepthSlice::drawMotionDirection()
{
    map<int, TrackedBlob>::iterator iter;
    for (iter = trackedBlobs.begin(); iter != trackedBlobs.end(); ++iter)
    {
        iter->second.drawMotionDirection();
    }
}


void DepthSlice::setDrawPnt(float x, float y)
{
    drawPnt.x = x;
    drawPnt.y = y;
}


void DepthSlice::keyPressed (int key)
{
}
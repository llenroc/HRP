//
//  DepthSlice.cpp
//  KinectDepthMotion
//
//  Created by James Alliban's MBP on 02/05/2014.
//
//

#include "DepthSlice.h"

float DepthSlice::blurAmount;
float DepthSlice::blurIterations;
float DepthSlice::contourMinRadius;
float DepthSlice::contourMaxRadius;
float DepthSlice::contourThreshold;

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

void DepthSlice::update(ofTexture *depthTex, int minThreshold, int maxThreshold)
{
    // create slice apply threshold to depth image on GPU
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
    
    // apply to polylines then resample and smooth
    polyLines.clear();
    if (contourFinder.getContours().size() > 0)
    {
//        contourFinder.getPol
    }
}


void DepthSlice::draw()
{
    ofPushStyle();
    ofSetColor(255);
    sliceFbo.draw(0, 0);
    
    ofSetColor(255, 0, 0, 255);
    contourFinder.draw();
    
    for (int i = 0; i < contourFinder.getPolylines().size(); i++)
    {
        float arcLength = contourFinder.getArcLength(i);
        //printf("arcLength = %f\n", arcLength);
        ofPolyline polyline = contourFinder.getPolyline(i);
        polyline = polyline.getResampledByCount(ofMap(arcLength, 0, 1000, 30, 31));
        ofSetColor(0, 255, 0, 255);
        polyline.draw();
    }
    ofPopStyle();
}



void DepthSlice::debugDraw()
{
}



void DepthSlice::keyPressed (int key)
{
}
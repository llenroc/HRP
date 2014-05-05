//
//  TrackedBlob.cpp
//  KinectDepthMotion
//
//  Created by James Alliban's MBP on 02/05/2014.
//
//

#include "TrackedBlob.h"


float TrackedBlob::polylinesHistorySize;
float TrackedBlob::pointSmoothing;
float TrackedBlob::averageBoundingBoxHistorySize;
float TrackedBlob::motionProbPosInputMin;
float TrackedBlob::motionProbPosInputMax;
float TrackedBlob::motionProbPosOutputMin;
float TrackedBlob::motionProbPosOutputMax;
float TrackedBlob::motionProbSizeInputMin;
float TrackedBlob::motionProbSizeInputMax;
float TrackedBlob::motionProbSizeOutputMin;
float TrackedBlob::motionProbSizeOutputMax;

void TrackedBlob::setup(ofShader *_blobDifferncingShader, ofFbo *_blobDifferncingFbo, int sliceW, int sliceH)
{
    blobDifferncingShader = _blobDifferncingShader;
    //blobDifferncingFbo = _blobDifferncingFbo;
    w = sliceW;
    h = sliceH;
    
    ofFbo::Settings settings;
    settings.width = sliceW;
    settings.height = sliceH;
    settings.internalformat = GL_RGBA;
    
    blobDifferncingFbo.allocate(settings);
    
    age = 0;
}


void TrackedBlob::update()
{
    // if the amount of polyline verts has changed (from expanding, contracting) restart the blob's history and age
    // this reduced the change of motion misreadings when all the points shift to adjust to new amount
    if (polylines[0].size() != prevPolylineVertCount)
    {
//        age = 0;
//        polylines.clear();
//        motionSmoothedPolyline.clear();
//        prevPolylineVertCount = polylines[0].size();
//        motionSmoothedPolyline.resize(polylines.size());
//        return;
        
        // if the polyline pnt count has increased increase the
        //if (polylines[0].size() < prevPolylineVertCount)
        
        
        motionSmoothedPolyline = polylines[0];
    }
    
    if (polylines.size() > 0 && age == 0)
        motionSmoothedPolyline = polylines[0];
    
    
    // peform smoothing
    if (polylines.size() > 0 && age > 0 && motionSmoothedPolyline.size() > 0)
    {
        prevMotionPolyline = motionSmoothedPolyline;
        ofPolyline *polyline = &polylines[0];
        ofSetColor(255, 0, 0, 255);
        for (int i = 0; i < polyline->size(); i++)
        {
            ofPoint pnt = polylines[0][i];
            ofPoint *motionPolylinePnt = &motionSmoothedPolyline[i];
            *motionPolylinePnt -= (*motionPolylinePnt - pnt) * pointSmoothing;
        }
    }
    
    
    
    
    
    
    
    
//     Figure out the odds of whether this blob has actually moved through a few tests
//     - Is the bounding box similar to average of last few bounding boxes?
//     - Check difference in arc
//     - Is motion uniform in size and direction?
//     - Does general vertex position changes over last few frames?
//     - Compare polylines on the GPU
    motionProbability = -1.0;
    if (polylines.size() > 1)
    {
        motionProbability = 1.0;
        
        // compare motion bounding box to the average of the last n bounding boxes
        averageBoundingBox = ofRectangle(0, 0, 0, 0);
        int boxAveragingHistory = (polylines.size() >= averageBoundingBoxHistorySize) ? averageBoundingBoxHistorySize : polylines.size();
        for (int i = 0; i < boxAveragingHistory; i++)
        {
            ofRectangle bb = polylines[0].getBoundingBox();
            averageBoundingBox.x += bb.x;
            averageBoundingBox.y += bb.y;
            averageBoundingBox.width += bb.width;
            averageBoundingBox.height += bb.height;
        }
        averageBoundingBox.x /= boxAveragingHistory;
        averageBoundingBox.y /= boxAveragingHistory;
        averageBoundingBox.width /= boxAveragingHistory;
        averageBoundingBox.height /= boxAveragingHistory;
        ofRectangle motionSmoothedPolylineBB = motionSmoothedPolyline.getBoundingBox();
        motionProbability *= ofMap(abs(averageBoundingBox.x - motionSmoothedPolylineBB.x), motionProbPosInputMin, motionProbPosInputMax, motionProbPosOutputMin, motionProbPosOutputMax, true);
        motionProbability *= ofMap(abs(averageBoundingBox.y - motionSmoothedPolylineBB.y), motionProbPosInputMin, motionProbPosInputMax, motionProbPosOutputMin, motionProbPosOutputMax, true);
        motionProbability *= ofMap(abs(averageBoundingBox.width - motionSmoothedPolylineBB.width), motionProbSizeInputMin, motionProbSizeInputMax, motionProbSizeOutputMin, motionProbSizeOutputMax, true);
        motionProbability *= ofMap(abs(averageBoundingBox.height - motionSmoothedPolylineBB.height), motionProbSizeInputMin, motionProbSizeInputMax, motionProbSizeOutputMin, motionProbSizeOutputMax, true);
    
    
        // get pixel difference between 2 polylines - perform this on the GPU
        blobDifferncingFbo.begin();
        ofPushStyle();
        ofClear(0, 0, 0, 255);
        ofSetColor(255, 0, 0);
        polylines[0].draw();
        ofSetColor(0, 0, 0);
        motionSmoothedPolyline.draw();
        ofPopStyle();
        blobDifferncingFbo.end();
        
    }
    
    
    // calculate velocities
//    velocities.clear();
    velocities.resize(motionSmoothedPolyline.size());
    averageVelocity = ofVec2f(0, 0);
    
    if (polylines.size() > 1 && age > 1)
    {
        for (int i = 0; i < motionSmoothedPolyline.size(); i++)
        {
            velocities[i] = (polylines[0][i] - motionSmoothedPolyline[i]) * motionProbability;
            averageVelocity += velocities[i].getNormalized();
        }
        averageVelocity /= motionSmoothedPolyline.size();
    }
    
    
    prevPolylineVertCount = polylines[0].size();
    ++age;
}


void TrackedBlob::draw()
{
    
}


void TrackedBlob::drawSmoothedPolylines()
{
    ofPushStyle();
    if (polylines.size() > 0)
    {
        ofPushStyle();
        ofSetColor(0, 0, 255);
        motionSmoothedPolyline.draw();
        ofPopStyle();
    }
    ofPopStyle();
}


void TrackedBlob::drawSmoothedPolylinePoints()
{
    ofPushStyle();
    for (int i = 0; i < motionSmoothedPolyline.size(); i++)
    {
        ofPoint pnt = motionSmoothedPolyline[i];
        ofSetColor(0,
                   ofMap(i, 0, motionSmoothedPolyline.size(), 255, 0),
                   ofMap(i, 0, motionSmoothedPolyline.size(), 0, 255));
        ofCircle(pnt, 1);
    }
    ofPopStyle();
}



void TrackedBlob::drawSmoothedPolylineBoundingBox()
{
    ofPushStyle();
    ofNoFill();
    ofSetColor(0, 255, 255);
    ofRect(motionSmoothedPolyline.getBoundingBox());
    ofSetColor(255, 0, 255);
    ofRect(averageBoundingBox);
    ofPopStyle();
}



void TrackedBlob::drawMotionDirection()
{
    ofPushStyle();
    if (polylines.size() > 1 && age > 1)
    {
        for (int i = 0; i < motionSmoothedPolyline.size(); i++)
        {
            ofSetColor(255, 255, 0);
            ofLine(prevMotionPolyline[i].x, prevMotionPolyline[i].y, polylines[0][i].x, polylines[0][i].y);
            
//            ofLine(prevMotionPolyline[i].x,
//                   prevMotionPolyline[i].x + velocities[i].x,
//                   prevMotionPolyline[i].y,
//                   prevMotionPolyline[i].y + velocities[i].y);
            
//            ofLine(motionSmoothedPolyline[i].x,
//                   motionSmoothedPolyline[i].x + velocities[i].x,
//                   motionSmoothedPolyline[i].y,
//                   motionSmoothedPolyline[i].y + velocities[i].y);
            
            
        }
        ofRectangle bb = motionSmoothedPolyline.getBoundingBox();
        string msg = "MP:" + ofToString(motionProbability) + "\nAV:" + ofToString(averageVelocity.length());
        ofDrawBitmapString(msg, ofPoint(bb.x + bb.width * 0.5, bb.y + bb.height * 0.5));
        
    }
    ofPopStyle();
    
    //blobDifferncingFbo.draw(0,0);
}


void TrackedBlob::addNewPolyline(ofPolyline polyline)
{
    polylines.insert(polylines.begin(), polyline);
    if (polylines.size() > polylinesHistorySize)
        polylines.pop_back();
}


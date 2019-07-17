#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	grbl.setup();
	grbl.connect("cu.wchusbserial1420", 115200);


	// move from stroke file
//	grbl.loadFromFile("./star.gcode");
	
	

	setRects();
	
	
}
//--------------------------------------------------------------
void ofApp::setRects(){
	screenRect.set(0,0,ofGetWidth(), ofGetHeight());
	
	grblRect = grbl.getArea();
	grblDrawRect = grblRect;
	grblDrawRect.scaleTo(screenRect);
}
//--------------------------------------------------------------
void ofApp::update(){
//	grbl.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
//	grbl.draw();
	ofBackground(0);
	ofPushStyle();
	ofFill();
	ofSetColor(60);
	ofDrawRectangle(grblDrawRect);
	
	ofNoFill();
	ofSetColor(120);
	ofSetLineWidth(2);
	ofDrawRectangle(grblDrawRect);
	ofPopStyle();
	
	
	ofSetColor(255);
	for(auto&p: drawPolys){
		p.draw();
	}
	ofPopMatrix();
	
	
}

//--------------------------------------------------------------
void ofApp::exit() {
	grbl.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case 'h':
		grbl.sendMessage("$$", true);
		break;
	case OF_KEY_RETURN:
			grbl.send(sendPolys);
			grbl.saveStrokesToGCodeFile(ofGetTimestampString()+".txt");
		break;
	case OF_KEY_RIGHT:
		grbl.moveRight(10);
		break;
	case OF_KEY_LEFT:
		grbl.moveLeft(10);
		break;
	case OF_KEY_UP:
		grbl.moveDown(10);
		break;
	case OF_KEY_DOWN:
		grbl.moveUp(10);
		break;
	default:
		break;
	}
}
//--------------------------------------------------------------
void ofApp::addPolyVertex(float x, float y, bool bMakeNewPoly){
	if(bMakeNewPoly){
		drawPolys.push_back(ofPolyline());
		sendPolys.push_back(ofPolyline());
	}
	drawPolys.back().addVertex({ ofClamp(x, grblDrawRect.getMinX(), grblDrawRect.getMaxX()),
								 ofClamp(y, grblDrawRect.getMinY(), grblDrawRect.getMaxY()),0});
	sendPolys.back().addVertex({ ofMap(x,grblDrawRect.getMinX(), grblDrawRect.getMaxX(), grblRect.getMinX(), grblRect.getMaxX(), true),
								 ofMap(y,grblDrawRect.getMinY(), grblDrawRect.getMaxY(), grblRect.getMinY(), grblRect.getMaxY(), true),0});

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
void ofApp::mouseMoved(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if(button != OF_MOUSE_BUTTON_RIGHT){
		addPolyVertex(x,y);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if(button == OF_MOUSE_BUTTON_RIGHT){
		grbl.setPosition({  ofMap(x, grblDrawRect.getMinX(), grblDrawRect.getMaxX(), grblRect.getMinX(), grblRect.getMaxX(), true),
							ofMap(y, grblDrawRect.getMinY(), grblDrawRect.getMaxY(), grblRect.getMinY(), grblRect.getMaxY(), true),0});
	}else{
		addPolyVertex(x,y, true);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if(button != OF_MOUSE_BUTTON_RIGHT){
		addPolyVertex(x,y);
	}
	
}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	setRects();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}

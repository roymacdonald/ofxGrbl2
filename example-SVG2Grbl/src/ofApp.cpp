#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	grbl.setup();
	grbl.connect("cu.wchusbserial1420", 115200);
	
	gui.setup();
	gui.add(grbl.getSettingsParams());
	
	
	// move from stroke file
	//	grbl.loadFromFile("./star.gcode");
	setRects();
	openFileDialog();
	
	
	
	
	
}
void ofApp::openFileDialog()
{
	auto r = ofSystemLoadDialog("Select svg file");
	if(r.bSuccess)
	{
		if(ofToLower(ofFilePath::getFileExt( r.getPath())) != "svg")
		{
			openFileDialog();
		}
			svg.load(r.getPath());
		
		cout << "load svg: " << r.getPath() << endl;
				svg.load(r.getPath());
			cout << " numPaths: " << svg.getNumPath() << " w: " << svg.getWidth()  << " h: " << svg.getHeight() << endl;
			ofRectangle bb;
			bool bFirst = true;
			for (ofPath p: svg.getPaths()){
				// svg defaults to non zero winding which doesn't look so good as contours
				p.setPolyWindingMode(OF_POLY_WINDING_ODD);
				const vector<ofPolyline>& lines = p.getOutline();
				
				for(const ofPolyline & line: lines){
					drawPolys.push_back(line);//.getResampledBySpacing(1));
					if(bFirst)
					{
						bFirst = false;
						bb = line.getBoundingBox();
					}
					else
					{
						bb.growToInclude(line.getBoundingBox());
					}
				}
			}
			cout << "Bounding box: " << bb << "\n";
			
			
		
		
//		for(auto& p: svg.getPaths()){
//			for(auto& o: p.getOutline())
//			{
//
//				drawPolys.push_back(ofPolyline());
//				sendPolys.push_back(ofPolyline());
//
//				for(auto& v: o.getVertices())
//				{
//					addPolyVertex(v.x, v.y, false);
//				}
//			}
//		}
	}
}
//--------------------------------------------------------------
void ofApp::setRects(){
	auto guiW = gui.getShape().width;
	screenRect.set(guiW,0,ofGetWidth() - guiW, ofGetHeight());
	
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
	gui.draw();
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
	if(ofGetKeyPressed(OF_KEY_COMMAND))
	{
		if(key == 'o')
		{
			openFileDialog();
		}
	}
	else
	{
	
	switch (key)
	{
		case 'h':
			grbl.goHome();
			//		grbl.sendMessage("$$", true);
			break;
		case OF_KEY_RETURN:
			grbl.send(sendPolys);
			//			grbl.saveStrokesToGCodeFile(ofGetTimestampString()+".txt");
			break;
			
		case 'D':
			grbl.penDown();
			break;
		case 'U':
			grbl.penUp();
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
	if(button != OF_MOUSE_BUTTON_RIGHT && bAddingVertices){
//		addPolyVertex(x,y);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
//	if(grblDrawRect.inside(x, y)){
//		if(button == OF_MOUSE_BUTTON_RIGHT){
//			grbl.setPosition({  ofMap(x, grblDrawRect.getMinX(), grblDrawRect.getMaxX(), grblRect.getMinX(), grblRect.getMaxX(), true),
//				ofMap(y, grblDrawRect.getMinY(), grblDrawRect.getMaxY(), grblRect.getMinY(), grblRect.getMaxY(), true),0});
//		}else{
//			bAddingVertices = true;
//			addPolyVertex(x,y, true);
//		}
//	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
//	if(grblDrawRect.inside(x, y) && bAddingVertices){
//		bAddingVertices = false;
//		if(button != OF_MOUSE_BUTTON_RIGHT){
//			addPolyVertex(x,y);
//		}
//	}
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

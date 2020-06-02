#pragma once

#include "ofMain.h"
#include "ofxGrblDevice.h"
#include "ofxGui.h"
#include "ofxSvg.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
	ofxGrbl::device grbl;
	
	vector<ofPolyline> sendPolys, drawPolys;
	

	void setRects();
	ofRectangle screenRect;
	ofRectangle grblRect, grblDrawRect;

	
	
	void addPolyVertex(float x, float y, bool bMakeNewPoly = false);

	ofxPanel gui;
	bool bAddingVertices = false;

	void openFileDialog();
	
	ofxSVG svg;
	
	
};

//#pragma once
//
//#include "ofMain.h"
//#include "ofxGui.h"
//#include "ofxGrblSettings.h"
//class ofxGrbl{
//
//public:
//	
//	void setup();
//	void update();
//	void draw();
//	void draw(int x, int y, int w, int h);
//	void close();
//
//	void keyPressed(ofKeyEventArgs& k);
//	void keyReleased(ofKeyEventArgs& k);
//	void mouseMoved(ofMouseEventArgs& m);
//	void mouseDragged(ofMouseEventArgs& m);
//	void mousePressed(ofMouseEventArgs& m);
//	void mouseReleased(ofMouseEventArgs& m);
//	void dragEvent(ofDragInfo dragInfo);
//	
//	
//	
//	void setDrawMode();
//	void setMoveMode();
//	void setColor(ofColor _color);
//	ofColor color;
//	void setBGColor(ofColor _color);
//	ofColor bgColor;
//
//	
//	void drawStrokes();
//	
//	// fbo
//	ofFbo _fbo;
//
//	// Settings
//	
//	glm::vec3 prevPos;
//	glm::vec3 targetPos;
//	glm::vec3 currentPos;
//	
//
//	bool isDrawMode = false;
//	bool isDown = false;
//	
//	vector<vector<glm::vec3>> strokeList;
//	vector<glm::vec3> tmpStroke;
//
//	// UI
//	void initGui();
//	void toggleGuiVisible();
//	
//	ofxPanel gui;
//	
//	
//};

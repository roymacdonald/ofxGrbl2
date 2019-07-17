//#include "ofxGrbl.h"
//
////--------------------------------------------------------------
//void ofxGrbl::setup() {
//	ofLogVerbose("ofxGrbl::setup");
//
//	readBuffer = "";
//	status = "";
//
//	// param
//	isReadyToSend = true;
//	isPause = false;
//	isDrawMode = true;
//	firstTimeLoad = true;
//
//	// fbo
//	cout << ofGetWidth() << " , " << ofGetHeight() ;
//	WINDOW_WIDTH = ofGetWidth();
//	WINDOW_HEIGHT = ofGetHeight();
//	_fbo.allocate(WINDOW_WIDTH, WINDOW_HEIGHT);
//
//	// serial
//	serial.getDeviceList();
//	isConnect = false;
//	isDeviceReady = false;
////	port = "COM3";
//	baudrate = 115200;
//
//	// settings
//	//baudrateList.push_back("9600");
//	//baudrateList.push_back("19200");
//	//baudrateList.push_back("38400");
//	//baudrateList.push_back("57600");
//	baudrateList.push_back("115200");
//
//	// mode
//	modeList.push_back("Spindle");
//	modeList.push_back("Laser");
//	modeList.push_back("Plotter");
//
//	
//	if(bgColor == ofColor())bgColor = ofColor(40, 255);
//
//	if(settingsFileName == "") settingsFileName = "grblSettings.xml";
//
//	initUI();
//	Connect();
//
//	color = ofColor::white;
//}
//
////--------------------------------------------------------------
//void ofxGrbl::update() {
//
//
//}
//
////--------------------------------------------------------------
//void ofxGrbl::draw() {
//	draw(0, 0, ofGetWidth(), ofGetHeight());
//}
////--------------------------------------------------------------
//void ofxGrbl::draw(int x, int y, int w, int h) {
//	_fbo.begin();
//	ofBackground(bgColor);
//	ofNoFill();
//	ofSetColor(255);
//	ofSetLineWidth(3);
//	ofDrawRectangle(2, 2, WINDOW_WIDTH - 3, WINDOW_HEIGHT - 3);
//	ofFill();
//	ofSetLineWidth(3);
//
//	if (ofGetMousePressed(0)) {
//		//ofSetColor(color);
//		//ofDrawCircle((float)ofGetMouseX() / ofGetWidth() * WINDOW_WIDTH, (float)ofGetMouseY() / ofGetHeight() * WINDOW_HEIGHT, 15);
//		//ofSetColor(0, 255, 0);
//		//ofDrawBitmapString("X:" + ofToString(ofGetMouseX() / (float)ofGetWidth() * GRBL_WIDTH) + "\nY:" + ofToString(ofGetMouseY() / (float)ofGetHeight() * GRBL_HEIGHT), (float)ofGetMouseX() / ofGetWidth() * WINDOW_WIDTH + 20, (float)ofGetMouseY() / ofGetHeight() * WINDOW_HEIGHT + 20);
//	}
//
//	for (int i = 0; i < strokeList.size(); i++) {
//		ofPath line = ofPath();
//		line.setFilled(false);
//		line.setStrokeWidth(3);
//		line.setStrokeColor(ofColor(255, 255, 255));
//		line.moveTo(strokeList[i][0].x * WINDOW_WIDTH, strokeList[i][0].y * WINDOW_HEIGHT);
//		for (int j = 1; j < strokeList[i].size(); j++) {
//			line.lineTo(strokeList[i][j].x * WINDOW_WIDTH, strokeList[i][j].y * WINDOW_HEIGHT);
//		}
//		line.draw();
//		line.close();
//	}
//
//	if (tmpStroke.size() > 0) {
//		ofPath line = ofPath();
//		line.setStrokeColor(color);
//		line.setFilled(false);
//		line.setStrokeWidth(3);
//		line.moveTo(tmpStroke[0].x * WINDOW_WIDTH, tmpStroke[0].y * WINDOW_HEIGHT);
//		for (int i = 1; i < tmpStroke.size(); i++) {
//			line.lineTo(tmpStroke[i].x * WINDOW_WIDTH, tmpStroke[i].y * WINDOW_HEIGHT);
//		}
//		line.draw();
//		line.close();
//	}
//
//	ofSetColor(255, 0, 0);
//
//	ofDrawCircle(currentPos.x * WINDOW_WIDTH, currentPos.y * WINDOW_HEIGHT, 20);
//	ofDrawBitmapString("X:" + ofToString(currentPos.x, 3) + "\nY:" + ofToString(currentPos.y, 3), currentPos.x * WINDOW_WIDTH + 20, currentPos.y * WINDOW_HEIGHT + 20);
//	
//	// mousePos
//	if (isDrawMode) {
//		ofSetColor(0, 255, 0);
//		ofDrawBitmapString("[ Draw Mode ]", targetPos.x * WINDOW_WIDTH + 20, targetPos.y * WINDOW_HEIGHT);
//	}
//	else {
//		ofSetColor(255, 100, 0);
//		ofDrawBitmapString("[ Move Mode ]", targetPos.x * WINDOW_WIDTH + 20, targetPos.y * WINDOW_HEIGHT);
//	}
//	ofDrawLine(targetPos.x * WINDOW_WIDTH - 10, targetPos.y * WINDOW_HEIGHT, targetPos.x * WINDOW_WIDTH + 10, targetPos.y * WINDOW_HEIGHT);
//	ofDrawLine(targetPos.x * WINDOW_WIDTH, targetPos.y * WINDOW_HEIGHT - 10, targetPos.x * WINDOW_WIDTH, targetPos.y * WINDOW_HEIGHT + 10);
//	ofDrawBitmapString("X:" + ofToString(targetPos.x, 3) + "\nY:" + ofToString(targetPos.y, 3), targetPos.x * WINDOW_WIDTH + 20, targetPos.y * WINDOW_HEIGHT + 20);
//
//	_fbo.end();
//
//	ofSetColor(255, 255, 255);
//	_fbo.draw(x, y, w, h);
//}
//
////--------------------------------------------------------------
//void ofxGrbl::close() {
//	if (isConnect) {
//		if (bSpindle) setSpindle(false, true);
//		sendMessage("G90 G0 X0 Y0 Z0", true);
//	}
//}
//
////--------------------------------------------------------------
//void ofxGrbl::keyPressed(int key) {
//
//}
//
////--------------------------------------------------------------
//void ofxGrbl::keyReleased(int key) {
//
//}
//
////--------------------------------------------------------------
//void ofxGrbl::mouseMoved(int x, int y) {
//	
//	if (x < 0)x = 0;
//	if (x > GRBL_WIDTH) x = GRBL_WIDTH;
//	if (y < 0)y = 0;
//	if (y > GRBL_HEIGHT) y = GRBL_HEIGHT;
//	targetPos = glm::vec3((float)x / GRBL_WIDTH, (float)y / GRBL_HEIGHT, 0);
//
//	if (gui->isVisible()) return;
//	if (isDrawMode == false) {
//		sendMessage(vec3fToGcode(targetPos));
//	}
//}
//
////--------------------------------------------------------------
//void ofxGrbl::mouseDragged(int x, int y, int button) {
//	if (gui->isVisible()) return;
//	if (x < 0)x = 0;
//	if (x > GRBL_WIDTH) x = GRBL_WIDTH;
//	if (y < 0)y = 0;
//	if (y > GRBL_HEIGHT) y = GRBL_HEIGHT;
//
//	targetPos = glm::vec3((float)x / GRBL_WIDTH, (float)y / GRBL_HEIGHT,0);
//
//	// minimum move
//	if (ofDist(prevPos.x, prevPos.y, targetPos.x, targetPos.y) < 0.001f) {
//		cout << "Minimum move : " << ofDist(prevPos.x, prevPos.y, targetPos.x, targetPos.y) ;
//		return;
//	}
//
//	tmpStroke.push_back(targetPos);
//	sendMessage(vec3fToGcode(targetPos));
//
//	prevPos = targetPos;
//}
//
////--------------------------------------------------------------
//void ofxGrbl::mousePressed(int x, int y, int button) {
//	if (gui->isVisible()) return;
//	tmpStroke.clear();
//	glm::vec3 _tmpVec2 = glm::vec3((float)x / GRBL_WIDTH, (float)y / GRBL_HEIGHT,0);
//
//	tmpStroke.push_back(_tmpVec2);
//	sendMessage(vec3fToGcode(_tmpVec2));
//	/*if (_settings.Mode == "Spindle" || _settings.Mode == "Laser")*/setSpindle(true, false);
//	if (_settings.Mode == "Plotter")  sendMessage("G1 Z" + ofToString(_settings.HomePosition.z - _settings.PushDistance, 2));
//}
//
////--------------------------------------------------------------
//void ofxGrbl::mouseReleased(int x, int y, int button) {
//	if (gui->isVisible()) return;
//	glm::vec3 _tmpVec2 = glm::vec3((float)x / GRBL_WIDTH, (float)y / GRBL_HEIGHT,0);
//	tmpStroke.push_back(_tmpVec2);
//	if (tmpStroke.size() > 2) {
//		strokeList.push_back(tmpStroke);
//	}
//
//	tmpStroke.clear();
//
//	sendMessage(vec3fToGcode(_tmpVec2));
//	/*if (_settings.Mode == "Spindle" || _settings.Mode == "Laser")*/setSpindle(false, false);
//	if(_settings.Mode == "Plotter")  sendMessage("G1 Z" + ofToString(_settings.HomePosition.z, 2));
//}
//
////--------------------------------------------------------------
//void ofxGrbl::dragEvent(ofDragInfo dragInfo) {
//	for (auto& f: dragInfo.files) {
//		loadFromFile(ofToDataPath(f));
//	}
//}
//
////--------------------------------------------------------------
//void ofxGrbl::initUI() {
//	cout << "initUI()" ;
//
//	gui.setup("[ Grbl Manager ]");
//
//	
//	gui->addLabel("MODE_LABEL", "Mode");
//	gui->addRadio("MODE", modeList);
//
//	gui->addLabel("SPINDLE_LABEL", "Spindle");
//	gui->addLabelToggle("bSpindle", &bSpindle);
//	gui->addSlider("Spindle Speed(Laser Power)", 0.0, 1000.0, &_settings.SpindleSpeed);
//
//	gui->addLabel("Plotter_LABEL", "Plotter");
//	gui->addSlider("Push Distance", 0.0, 100.0, &_settings.PushDistance);
//	
//
//	gui->addWidgetDown(new ofxUILabel("Grbl Settings", OFX_UI_FONT_MEDIUM));
//	gui->addSlider("MAX SPEED X", 1, 20000, &_settings.MaxSpeed.x);
//	gui->addSlider("MAX SPEED Y", 1, 20000, &_settings.MaxSpeed.y);
//	gui->addSlider("MAX SPEED Z", 1, 20000, &_settings.MaxSpeed.z);
//	gui->addSlider("ACCEL X", 10.0, 500.0, &_settings.Accel.x);
//	gui->addSlider("ACCEL Y", 10.0, 500.0, &_settings.Accel.y);
//	gui->addSlider("ACCEL Z", 10.0, 500.0, &_settings.Accel.z);
//	gui->addSlider("MAX TRAVEL X", 100.0, 2000.0, &_settings.MaxTravel.x);
//	gui->addSlider("MAX TRAVEL Y", 100.0, 2000.0, &_settings.MaxTravel.y);
//	gui->addSlider("MAX TRAVEL Z", 100.0, 2000.0, &_settings.MaxTravel.z);
//	
//	
//	
//
//	gui->addLabel("PORT_LABEL", "Serial Port");
//	gui->addTextInput("PORT", "COM3");
//	gui->addLabelButton("CONNECT", false);
//	
//
//	gui->addSpacer(length - xInit, 2);
//	gui->addWidgetDown(new ofxUILabel("CONTROL", OFX_UI_FONT_MEDIUM));
//	gui->addImageButton("HOME", "./GUI/icon/fa-home.png", false, 30, 30);
//	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
//	gui->addImageButton("PLAY", "./GUI/icon/fa-play-circle-o.png", false, 30, 30);
//	gui->addImageToggle("PAUSE", "./GUI/icon/fa-pause-circle-o.png", &isPause, 30, 30);
//	gui->addImageButton("STOP", "./GUI/icon/fa-stop-circle-o.png", false, 30, 30);
//	gui->addImageButton("PATH_SAVE", "./GUI/icon/fa-save.png", false, 30, 30);
//	gui->addImageButton("PATH_LOAD", "./GUI/icon/fa-folder-open.png", false, 30, 30);
//	gui->addImageButton("TRASH", "./GUI/icon/fa-trash.png", false, 30, 30);
//	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
//
//
//	loadSettings();
//}
//
////--------------------------------------------------------------
//void ofxGrbl::guiEvent(ofxUIEventArgs &e)
//{
//	string name = e.widget->getName();
//	int kind = e.widget->getKind();
//	//cout << "got event from: " << name << " kind:" << ofToString(kind) ;
//	
//	if (name == "PORT") {
//		ofxUITextInput *input = (ofxUITextInput *)e.widget;
//		port = input->getTextString();
//	}
//	else if (name == "BAUDRATE") {
//		ofxUIRadio *radio = (ofxUIRadio *)e.widget;
//		baudrate = ofToInt(radio->getActiveName());
//	}
//	else if (name == "CONNECT") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			Connect();
//		}
//	}
//	else if (name == "SERIAL SEND") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			ofxUITextInput *input = (ofxUITextInput *)gui->getWidget("INPUT");
//			sendMessage(input->getTextString(), true);
//		}
//	}
//	else if (name == "GET INFO") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			sendMessage("$$", true);
//		}
//	}
//	else if (name == "MODE") {
//		ofxUIRadio *radio = (ofxUIRadio *)e.widget;
//		string _selected = radio->getActiveName();
//		_settings.Mode = _selected;
//	}
//	else if (name == "Spindle" || name == "Laser" || name == "Plotter") {
//		ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
//		if (toggle->getValue()) {
//			_settings.Mode = toggle->getName();
//			if (_settings.Mode == "Laser") {
//				sendMessage("$32=1", true);
//			}
//			else {
//				sendMessage("$32=0", true);
//			}
//		}
//	}
//	else if (name == "Spindle Speed(Laser Power)") {
//		setSpindleSpeed(_settings.SpindleSpeed, true);
//	}
//	else if (name == "bSpindle") {
//		ofxUILabelToggle *toggle = (ofxUILabelToggle *)e.widget;
//		setSpindle(toggle->getValue(), true);
//	}
//	else if (name == "SET") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			setSettings();
//		}
//	}
//
//	else if (name == "HOME") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			home();
//		}
//	}
//	else if (name == "HOMING") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			homing();
//		}
//	}
//	else if (name == "KILL") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			killAlarmLock();
//		}
//	}
//	else if (name == "PLAY") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			if (sendQueList.size() == 0) {
//				drawStrokes();
//			}
//			isPause = false;
//		}
//	}
//	else if (name == "STOP") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			resetStrokes();
//		}
//	}
//	else if (name == "PATH_SAVE") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			if (tmpStroke.size() > 2) {
//				strokeList.push_back(tmpStroke);
//			}
//			tmpStroke.clear();
//
//			// Did'nt work on WindowsOS.
//			/*
//			ofFileDialogResult saveFileResult = ofSystemSaveDialog("strokeList.gcode", "Save stroke paths to GCODE(.gcode)");
//			saveStrokes(saveFileResult.getPath());
//			*/
//			string _fileName = ofSystemTextBoxDialog("Please enter the file name.", "stroke");
//			saveStrokes("./" + _fileName + ".gcode");
//		}
//	}
//	else if (name == "PATH_LOAD") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a GCODE(.gcode .nc .ngc) file");
//			loadFromFile(openFileResult.getPath());
//		}
//	}
//	else if (name == "TRASH") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			resetStrokes();
//		}
//	}
//	else if (name == "SAVE") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			saveSettings();
//		}
//	}
//	else if (name == "LOAD") {
//		ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//		if (button->getValue()) {
//			loadSettings();
//		}
//	}
//
//}
//
////--------------------------------------------------------------
//void ofxGrbl::toggleVisible() {
//
//}
//
////--------------------------------------------------------------
//void ofxGrbl::saveSettings() {
//
//}
////--------------------------------------------------------------
//void ofxGrbl::loadSettings() {
//	ofLogVerbose(" ofxGrbl ") << "loadSettings" ;
//	gui->loadSettings("./GUI/" + settingsFileName);
//
//	setSettings();
//
//	if (firstTimeLoad) {
//		ofLogVerbose(" ofxGrbl ") << "FirstTimeLoad!" ;
//		firstTimeLoad = false;
//		currentPos = targetPos = prevPos = glm::vec3(_settings.HomePosition.x  * _settings.MaxTravel.x, _settings.HomePosition.y  * _settings.MaxTravel.y, _settings.HomePosition.z  * _settings.MaxTravel.z);
//	}
//}
////--------------------------------------------------------------
//void ofxGrbl::drawStrokes() {
//	for (int i = 0; i < strokeList.size(); i++) {
//
//		for (int j = 0; j < strokeList[i].size(); j++) {
//			sendMessage(vec3fToGcode(strokeList[i][j]));
//			if (j == 0) {
//				if (_settings.Mode == "Plotter") sendMessage("G1 Z" + ofToString(_settings.HomePosition.z - _settings.PushDistance, 4));
//				/*if (_settings.Mode == "Spindle" || _settings.Mode == "Laser")*/setSpindle(true, false);
//			}
//			if (j == strokeList[i].size() - 1) {
//				if (_settings.Mode == "Plotter") sendMessage("G1 Z" + ofToString(_settings.HomePosition.z, 4));
//				/*if (_settings.Mode == "Spindle" || _settings.Mode == "Laser")*/setSpindle(false, false);
//			}
//		}
//	}
//}
//
////--------------------------------------------------------------
//void ofxGrbl::resetStrokes() {
//	strokeList.clear();
//	sendQueList.clear();
//}
////--------------------------------------------------------------
//void ofxGrbl::setDrawMode() {
//	isDrawMode = true;
//}
////--------------------------------------------------------------
//void ofxGrbl::setMoveMode() {
//	isDrawMode = false;
//}
////--------------------------------------------------------------
//void ofxGrbl::setColor(ofColor _color) {
//	ofLogVerbose(" ofxGrbl ") << "SetColor() : R" << (int)_color.r << " G" << (int)_color.g << " B" << (int)_color.b << " A" << (int)_color.a ;
//	color = _color;
//}
////--------------------------------------------------------------
//void ofxGrbl::setBGColor(ofColor _color) {
//	ofLogVerbose(" ofxGrbl ") << "setBGColor() : R" << (int)_color.r << " G" << (int)_color.g << " B" << (int)_color.b << " A" << (int)_color.a ;
//	bgColor = _color;
//}
////--------------------------------------------------------------

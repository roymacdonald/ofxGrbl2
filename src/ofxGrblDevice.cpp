//
//  ofxGrblDevice.cpp
//  ofxGrbl
//
//  Created by Roy Macdonald on 7/9/19.
//
//

#include "ofxGrblDevice.h"
#include "ofxGrblUtils.h"
using namespace ofxGrbl;
enum CommandType{
	OFX_GRBL_COMM_NONE,
	OFX_GRBL_COMM_G0,
	OFX_GRBL_COMM_M,
};
//--------------------------------------------------------------
void device::setup(string _port, int _baudrate, const std::string& settingsFileName) {
	ofLogVerbose("ofxGrbl::device::setup");
	
	readBuffer = "";
	status = "";
	
	// param
	isReadyToSend = true;
	isPause = false;
	
	firstTimeLoad = true;
	
	// serial
	serial.listDevices();
	bConnected = false;
	isDeviceReady = false;
	
	connect(_port, _baudrate);
	
	loadSettings(settingsFileName);
	
	updateListener = ofEvents().update.newListener(this, &ofxGrbl::device::update);
	
}

//--------------------------------------------------------------
void device::update(ofEventArgs&) {
	if (bConnected) {
		
		while (serial.available() > 0) {
			
			char _byte = (char)serial.readByte();
			cout <<_byte;
			if (_byte == '\n' || _byte == '\r') {
				if (readBuffer != "") {
					ofLogVerbose(" ofxGrbl ") << "[ RECEIVE ] " << readBuffer ;
					if (readBuffer.find("Grbl") != string::npos) {
						if (!isDeviceReady) {
							isDeviceReady = true;
							sendMessage("$$", true);
							//			sendSettings();
						}
					}else
					if (readBuffer == "ok") {
						isReadyToSend = true;
						//sentCount--;
						//ofLogVerbose(" ofxGrbl ") << "Sent: " << sentCount ;
					}else
					if (readBuffer == "error: Unsupported command") {
						ofLogVerbose(" ofxGrbl ") << "[ PAUSED ]" ;
						isPause = true;
						isReadyToSend = true;
					}
					if (readBuffer[0] == '<') {
						// parse grbl state message
						/*
						 vector<string> _status = ofSplitString(readBuffer, ",");
						 vector<string> _posx = ofSplitString(_status[1], ":");
						 vector<string> _posz = ofSplitString(_status[3], ">");
						 ofLogVerbose(" ofxGrbl ") << "[ POSITION ] " << _posx[1] << ", " << _status[2] << ", " << _posz[0] ;
						 currentPos = ofVec2f(ofToFloat(_posx[1]) / (float)GRBL_WIDTH, ofToFloat(_status[2]) / (float)GRBL_HEIGHT);
						 */
						
						readBuffer = readBuffer.substr(1, readBuffer.length() - 2);
						vector<string> _status = ofSplitString(readBuffer, "|");
						status = _status[0];
						vector<string> _pos_str = ofSplitString(_status[1], ":");
						if(_pos_str.size() > 1){
							vector<string> _pos = ofSplitString(_pos_str[1], ",");
							for(int pind = 0; pind < _pos.size() && pind < 3; pind++){
								currentPos[pind] = ofToFloat(_pos[pind]);
							}
							
							ofLogVerbose(" ofxGrbl ") << "[ POSITION ] " << currentPos;
							
							
							// Events
							ofNotifyEvent(PositionEvent, currentPos);
							if (currentPos.z < 1.0f) {
								if (!isDown) {
									isDown = true;
									ofLogVerbose(" ofxGrbl ") << "DOWN" ;
									ofNotifyEvent(UpDownEvent, isDown);
//									if (strokeList.size() > 1) strokeList.erase(strokeList.begin());
								}
							}
							else {
								if (isDown) {
									isDown = false;
									ofLogVerbose(" ofxGrbl ") << "UP" ;
									ofNotifyEvent(UpDownEvent, isDown);
								}
							}
						}
					}
					readBuffer = "";
				}
			}
			else {
				readBuffer = readBuffer + _byte;
			}
		}
	}
//	if(bSendToFile){
		
//		
//		if (sendQueList.size() > 0) {
//			std::cout << "sendQueList size : " << sendQueList.size()  << std::endl;
//			//			sendMessage(sendQueList[0], true);
//			for(auto& l: sendQueList){
//				std::cout << l << std::endl;
//				outputBuffer.append(l + "\n");
//				std::cout << l << std::endl;
//			}
//			sendQueList.clear();
////			sendQueList.erase(sendQueList.begin());
//			//			ofLogVerbose("device::update") << "Queue  size: " << sendQueList.size() ;
//			
//			//			isReadyToSend = false;
//		}
		
		
//	}else{
		if (bConnected && isDeviceReady) {
			
			// send
			if (isReadyToSend && !isPause) {
				if (sendQueList.size() > 0) {
					sendMessage(sendQueList[0], true);
					
					sendQueList.erase(sendQueList.begin());
					ofLogVerbose("device::update") << "Queue  size: " << sendQueList.size() ;
					
					isReadyToSend = false;
				}
			}
		}
//	}
}

//--------------------------------------------------------------
//void device::saveOutBuffer( const std::string& path){
//	
//	if (sendQueList.size() > 0) {
//		std::cout << "sendQueList size : " << sendQueList.size()  << std::endl;
//		for(auto& l: sendQueList){
//			outputBuffer.append(l + "\n");
//		}
//		sendQueList.clear();
//	
//		std::cout << "ofxGrbl::device::saveOutBuffer << " << path << std::endl;
//		ofBufferToFile(path, outputBuffer, false);
//	}
//}
//--------------------------------------------------------------
void device::close() {
	if (bConnected) {
		if (bSpindle) enableSpindle(false, true);
		sendMessage("G90 G0 X0 Y0 Z0", true);
	}
}

//--------------------------------------------------------------
void device::loadGCodeFromFile(const std::string& _path) {
	string _ext = ofFilePath::getFileExt(_path);
	if (_ext == "gcode" || _ext == "nc" || _ext == "ngc") {
		
		ofLogVerbose(" ofxGrbl ") << "loadFromFile( " << _path << " )" ;
		string _text = string(ofBufferFromFile(_path));
		vector<string> _linelist = ofSplitString(_text, "\n", true);
		ofLogVerbose(" ofxGrbl ") << "loadFromFile() : " << _linelist.size() << " lines." ;
		
		clear();
		
		bool bAbsolutePosition = true;
		
		for (int i = 0; i < _linelist.size(); i++) {
			_linelist[i] = ofTrim(_linelist[i]);
			
			
			auto commands = ofSplitString(_linelist[i], " ", true, true);
			glm::vec3 tmpVec;
			
			
			
			for(auto& c: commands){
				if(c.size() == 0)continue;
				float val;
				if(c.size() > 1){
					val = ofToFloat(c.substr(1));
				}
				
				if(c[0] == 'G'){
					if(c == "G90"){
						bAbsolutePosition = true;
					}else if(c == "G91"){
						bAbsolutePosition = false;
					}
				}else
				if(c[0] == 'M'){continue;}
				if(c[0] == 'X'){tmpVec.x = val;}
				if(c[0] == 'Y'){tmpVec.y = val;}
				if(c[0] == 'Z'){
					if(!ofIsFloatEqual(tmpVec.z, val) && val < 0.01f){
						polylines.push_back(ofPolyline());
					}
					tmpVec.z = val;
				}
				if(c[0] == 'F'){}
				
				if(!bAbsolutePosition){
					tmpVec += polylines.back().getVertices().back();
				}
				polylines.back().addVertex(tmpVec);
			}
			
			if (_settings.mode == OFXGRBL_PLOTTER) {
				
			}
			sendMessage(_linelist[i]);
		}
	} else {
		ofLogVerbose(" ofxGrbl ") << "Invalid extension. Please use ( .gcode / .ngc / .nc ). " ;
	}
}
//--------------------------------------------------------------
void device::saveStrokesToGCodeFile(const std::string& _path) {
	ofBuffer outputBuffer;
	
	if (sendQueList.size() > 0) {
		std::cout << "sendQueList size : " << sendQueList.size()  << std::endl;
		for(auto& l: sendQueList){
			outputBuffer.append(l + "\n");
		}
		sendQueList.clear();
		
		std::cout << "ofxGrbl::device::saveStrokesToGCodeFile << " << _path << std::endl;
		ofBufferToFile(_path, outputBuffer, false);
	}
}
//--------------------------------------------------------------
void device::send(const ofPath& path){
	send(path.getOutline());
}
//--------------------------------------------------------------
void device::send(const ofPolyline& poly){
	send(poly.getVertices());
}
//--------------------------------------------------------------
void device::send(const std::vector<glm::vec3>& p){
	sendUnits();
	
	if(p.size()> 0){
		sendMessage(getPositionModeString(OFXGRBL_ABSOLUTE));
		if (_settings.mode == OFXGRBL_PLOTTER) {
			sendMessage("G0 Z1.0");//bring pen up
			setPosition({p[0].x, p[0].y, 1}, true);//set the position t the first polyline vertex
			sendMessage("G1 Z0.0 F"+ ofToString(_settings.speed->z));//bring pen down
		}else{
			setPosition({p[0].x, p[0].y, 1}, true);//set the position t the first polyline vertex
		}
		sendMessage("G1 F"+ ofToString(_settings.speed->x));//set the feedrate(speed) of all subsequent moves
		for(size_t i = 1; i < p.size(); i++){
			sendMessage("G1" + vec3ToGcode(p[i], areaRect));
		}
		if (_settings.mode == OFXGRBL_PLOTTER) {
			sendMessage("G0 Z1.0");//bring pen up
		}
	}
}
//--------------------------------------------------------------
void device::send(const std::vector<ofPolyline>& polys){
	for(auto& p: polys){
		send(p);
	}
}
//--------------------------------------------------------------
void device::send(const ofPath& path, float z){
	send(path.getOutline(), z);
}
//--------------------------------------------------------------
void device::send(const ofPolyline& poly, float z){
	send(poly.getVertices(), z);
}
//--------------------------------------------------------------
void device::send(const std::vector<ofPolyline>& polys, float z){
	for(auto& p: polys){
		send(p,z);
	}
}

//--------------------------------------------------------------
void device::send(const std::vector<glm::vec3>& points, float z){
	auto p = points;
	for(auto& v : p){
		v.z = z;
	}
	send(p);
}

//--------------------------------------------------------------
void device::sendMessage(const std::string& _msg, bool direct) {
	cout << _msg << endl;
	if (direct) {
		if (bConnected) {
			if (_msg != "") {
				string _message = _msg + "\n";
				unsigned char* writeByte = (unsigned char*)_message.c_str();
				serial.writeBytes(writeByte, _message.length());
			}else {
				ofLogVerbose(" ofxGrbl ") << "sendMessage() : Message is empty." ;
			}
		}else {
			ofLogVerbose(" ofxGrbl ") << "sendMessage() : Serial is not connected." ;
		}
	}else {
		if (_msg != "") {
			string _message = _msg;
			sendQueList.push_back(_message);
		}
	}
}
//--------------------------------------------------------------
bool device::checkZisDown(const std::string& _line) {
	float _zPos;
	vector<string> _commands = ofSplitString(_line, " ", true);
	for (int i = 0; i < _commands.size(); i++) {
		if (_commands[i][0] == 'Z') {
			if (_commands[i].size() == 1) {
				// space parse
				_zPos = ofToFloat(_commands[i + 1]); // use next character
			}
			else {
				// no space parce
				_zPos = ofToFloat(_commands[i].substr(1));
			}
		}
	}
	
	return (_zPos == _settings.homePosition->z);
}

//--------------------------------------------------------------
void device::saveSettings(const std::string& settingsFileName) {
	if(settingsFileName != ""){
		this->settingsFileName = settingsFileName;
	}
	_settings.save(this->settingsFileName);
}
//--------------------------------------------------------------
void device::loadSettings(const std::string& settingsFileName) {
	if(settingsFileName != ""){
		this->settingsFileName = settingsFileName;
	}
	areaRect.set(0,0, _settings.maxTravel->x, _settings.maxTravel->y);
	if(_settings.load(this->settingsFileName)){
		sendSettings();
		if (firstTimeLoad) {
			ofLogVerbose(" ofxGrbl ") << "FirstTimeLoad!" ;
			firstTimeLoad = false;
//			currentPos = glm::vec3(_settings.homePosition->x  * _settings.maxTravel->x, _settings.homePosition->y  * _settings.maxTravel->y, _settings.homePosition->z  * _settings.maxTravel->z);
		}
	}
}
//--------------------------------------------------------------
void device::sendSettings() {
	
	
	sendMessage("$0=" + ofToString(_settings.stepPulse.get() ));
	sendMessage("$1=" + ofToString(_settings.stepIdleDelay.get() ));
	sendMessage("$2=" + ofToString(_settings.stepPortInvertMask.get() ));
	sendMessage("$3=" + ofToString(_settings.dirPortInvertMask.get() ));
	sendMessage("$4=" + ofToString(_settings.stepEnableInvert.get() ));
	sendMessage("$5=" + ofToString(_settings.limitPinsInvert.get() ));
	sendMessage("$6=" + ofToString(_settings.probePinInvert.get() ));
	sendMessage("$10=" + ofToString(_settings.statusReportMask.get() ));
	sendMessage("$11=" + ofToString(_settings.junctionDeviat .get(), OFX_GRBL_FLOAT_RES));
	sendMessage("$12=" + ofToString(_settings.arcTolerance .get(), OFX_GRBL_FLOAT_RES));
	sendMessage("$13=" + ofToString(_settings.reportInches.get() ));
	sendMessage("$20=" + ofToString(_settings.softLimits.get() ));
	sendMessage("$21=" + ofToString(_settings.hardLimits.get() ));
	sendMessage("$22=" + ofToString(_settings.homingCycles.get() ));
	sendMessage("$23=" + ofToString(_settings.homingDirInver.get() ));
	sendMessage("$24=" + ofToString(_settings.homingFeed .get(), OFX_GRBL_FLOAT_RES));
	sendMessage("$25=" + ofToString(_settings.homingSeek .get(), OFX_GRBL_FLOAT_RES));
	sendMessage("$26=" + ofToString(_settings.homingDebounce.get() ));
	sendMessage("$27=" + ofToString(_settings.homingPullOff .get(), OFX_GRBL_FLOAT_RES));
	sendMessage("$30=" + ofToString(_settings.maxSpindleSpeed .get(), OFX_GRBL_FLOAT_RES));
	sendMessage("$31=" + ofToString(_settings.minSpindleSpeed .get(), OFX_GRBL_FLOAT_RES));
	
	
	//	OFXGRBL_SPINDLE,
	//	OFXGRBL_LASER,
	//	OFXGRBL_PLOTTER
	if (_settings.mode == OFXGRBL_LASER) {
		sendMessage("$32=1");
	}else {
		sendMessage("$32=0");
	}
	// set spindle speed
	sendMessage("S" + ofToString((int)_settings.spindleSpeed));
	
	// set max speed
	//	sendMessage("F" +     ofToString(_settings.maxSpeed->x, 4));
	sendMessage("$110=" + ofToString(_settings.maxSpeed->x, OFX_GRBL_FLOAT_RES));
	sendMessage("$111=" + ofToString(_settings.maxSpeed->y, OFX_GRBL_FLOAT_RES));
	sendMessage("$112=" + ofToString(_settings.maxSpeed->z, OFX_GRBL_FLOAT_RES));
	// set accesl
	sendMessage("$120=" + ofToString(_settings.accel->x, OFX_GRBL_FLOAT_RES));
	sendMessage("$121=" + ofToString(_settings.accel->y, OFX_GRBL_FLOAT_RES));
	sendMessage("$122=" + ofToString(_settings.accel->z, OFX_GRBL_FLOAT_RES));
	// set max travel
	sendMessage("$130=" + ofToString(_settings.maxTravel->x, OFX_GRBL_FLOAT_RES));
	sendMessage("$131=" + ofToString(_settings.maxTravel->y, OFX_GRBL_FLOAT_RES));
	sendMessage("$132=" + ofToString(_settings.maxTravel->z, OFX_GRBL_FLOAT_RES));
	
	//	setArea(_settings.maxTravel->x, _settings.maxTravel->y);
}
//--------------------------------------------------------------
void device::sendUnits(){
	if(_settings.units == OFXGRBL_MILLIMETERS){
		sendMessage("G21");
	}else{
		sendMessage("G20");
	}
}

//--------------------------------------------------------------
void device::clear() {
//	strokeList.clear();
	sendQueList.clear();
}
//--------------------------------------------------------------
void device::setHome(const glm::vec3& _homePos) {
	ofLogVerbose(" ofxGrbl ") << "setHome(" << _homePos << ")" ;
	_settings.homePosition = _homePos;
	sendMessage("G90 G0 X" + ofToString(_settings.homePosition->x) + " Y" + ofToString(_settings.homePosition->y) + " Z" + ofToString(_settings.homePosition->z), true);
}
//--------------------------------------------------------------
void device::goHome() {
	sendMessage("$H", true);
}
//--------------------------------------------------------------
void device::killAlarmLock() {
	sendMessage("$X", true);
}
//--------------------------------------------------------------
void device::connect(string _port, int _baudrate) {
	if (_port == "") _port = port;
	if (_baudrate <= 0) _baudrate = baudrate;
	
	ofLogVerbose(" ofxGrbl ") << "Connect( " << _port << ", " << _baudrate << " )" ;
	
	// reset serial
	if (bConnected || isDeviceReady) {
		serial.close();
		bConnected = false;
		isDeviceReady = false;
	}
	
	bConnected = serial.setup(_port, _baudrate);
	if (bConnected) {
		
		ofLogVerbose(" ofxGrbl ") << "Connected to " << _port << "@" << _baudrate << " !" ;
	} else {
		ofLogWarning(" ofxGrbl ", "Connect") << "Can not connect. Port " << _port << " does not exists." ;
	}
}
//--------------------------------------------------------------
bool device::isConnected(){
	return bConnected;
}
//--------------------------------------------------------------
bool device::isSpindleEnabled(){
	return bSpindle;
}
//--------------------------------------------------------------
void device::enableSpindle(bool _enable, bool _direct) {
	if(bSpindle != _enable){
		ofLogVerbose(" ofxGrbl ") << "enableSpindle(" << _enable << ", " << _direct << ")" ;
		bSpindle = _enable;
		if (bSpindle) {
			sendMessage("M3", _direct);
		}
		else {
			sendMessage("M5", _direct);
		}
	}
}
//--------------------------------------------------------------
void device::setSpindleSpeed(int _speed, bool _direct) {
	if(_settings.spindleSpeed != _speed){
		ofLogVerbose(" ofxGrbl ") << "setSpindleSpeed(" << _speed << ", " << _direct << ")" ;
		_settings.spindleSpeed = _speed;
		sendMessage("S" + ofToString((int)_settings.spindleSpeed), _direct);
	}
}
//--------------------------------------------------------------
string device::getPositionModeString(PositionMode newMode){
	if(positionMode != newMode){
		positionMode = newMode;
		switch(positionMode){
			case OFXGRBL_ABSOLUTE: return "G90";
			case OFXGRBL_RELATIVE: return "G91";
		}
	}
	return "";
}
//--------------------------------------------------------------
void device::setPosition(const glm::vec3& _pos, bool bRapidMovement) {
	string msg = getPositionModeString(OFXGRBL_ABSOLUTE);
	
	sendMessage(msg + (string)(bRapidMovement?"G0":"G1") + vec3ToGcode(_pos, areaRect) +  " F"+ ofToString(_settings.speed->x), false);
				//+ " X" + ofToString(_pos.x) + " Y" + ofToString(_pos.y) + " Z" + ofToString(_pos.z) x
}
//--------------------------------------------------------------
void device::moveRight(float _mm) {
	sendMessage("G91 G1 X" + ofToString(_mm) + " Y0 Z0 F"+ ofToString(_settings.speed->x), true);
}
//--------------------------------------------------------------
void device::moveLeft(float _mm) {
	sendMessage("G91 G1 X" + ofToString(-_mm) + " Y0 Z0 F"+ ofToString(_settings.speed->x), true);
}
//--------------------------------------------------------------
void device::moveUp(float _mm) {
	sendMessage("G91 G1 X0 Y" + ofToString(_mm) + " Z0  F"+ ofToString(_settings.speed->y), true);
}
//--------------------------------------------------------------
void device::moveDown(float _mm) {
	sendMessage("G91 G1 X0 Y" + ofToString(-_mm) + " Z0  F"+ ofToString(_settings.speed->y), true);
}
////--------------------------------------------------------------
//const vector<vector<glm::vec3>> & device::getStrokeList() const{
//	return strokeList;
//}
////--------------------------------------------------------------
//vector<vector<glm::vec3>> & device::getStrokeList(){
//	return strokeList;
//}

//
//  ofxGrblDevice.cpp
//  ofxGrbl
//
//  Created by Roy Macdonald on 7/9/19.
//
//

#include "ofxGrblDevice.h"
#include "ofxGrblUtils.h"
namespace ofxGrbl{

//--------------------------------------------------------------
void device::setup(string _port, int _baudrate, const std::string& settingsFileName) {
	ofLogVerbose("ofxGrbl::device::setup");
	
	readBuffer = "";
	status = "";
	
	// param
	isReadyToSend = true;
	//	isPause = false;
	
	firstTimeLoad = true;
	
	// serial
	serial.listDevices();
	bConnected = false;
	isDeviceReady = false;
	
	connect(_port, _baudrate);
	
	loadSettings(settingsFileName);
	
	updateListener = ofEvents().update.newListener(this, &ofxGrbl::device::update);

	
	
	vec3ParamMap["$110"].makeReferenceTo(_settings.maxSpeed);
	vec3ParamMap["$120"].makeReferenceTo(_settings.accel);
	vec3ParamMap["$130"].makeReferenceTo(_settings.maxTravel);

	intParamMap["$0"].makeReferenceTo(_settings.stepPulse);
	intParamMap["$1"].makeReferenceTo(_settings.stepIdleDelay);
	intParamMap["$2"].makeReferenceTo(_settings.stepPortInvertMask);
	intParamMap["$3"].makeReferenceTo(_settings.dirPortInvertMask);
	intParamMap["$10"].makeReferenceTo(_settings.statusReportMask);
	intParamMap["$23"].makeReferenceTo(_settings.homingDirInver);
	intParamMap["$26"].makeReferenceTo(_settings.homingDebounce);


	boolParamMap["$4"].makeReferenceTo(_settings.stepEnableInvert);
	boolParamMap["$5"].makeReferenceTo(_settings.limitPinsInvert);
	boolParamMap["$6"].makeReferenceTo(_settings.probePinInvert);
	boolParamMap["$13"].makeReferenceTo(_settings.reportInches);
	boolParamMap["$20"].makeReferenceTo(_settings.softLimits);
	boolParamMap["$21"].makeReferenceTo(_settings.hardLimits);
	boolParamMap["$22"].makeReferenceTo(_settings.homingCycles);
	
	floatParamMap["$11"].makeReferenceTo(_settings.junctionDeviat);
	floatParamMap["$12"].makeReferenceTo(_settings.arcTolerance);
	floatParamMap["$24"].makeReferenceTo(_settings.homingFeed);
	floatParamMap["$25"].makeReferenceTo(_settings.homingSeek);
	floatParamMap["$27"].makeReferenceTo(_settings.homingPullOff);
	floatParamMap["$30"].makeReferenceTo(_settings.maxSpindleSpeed);
	floatParamMap["$31"].makeReferenceTo(_settings.minSpindleSpeed);
	


	
	
}



//--------------------------------------------------------------
void device::readSettingString(const string& msg)
{
	auto p = readBuffer.find_first_of('=');
	if(p != string::npos)
	{
		auto key = msg.substr(0, p);
		auto value = msg.substr(p+1);
	


		if(!setParamFromString(floatParamMap, key, value)){
			if(!setParamFromString(boolParamMap, key, value)){
				if(!setParamFromString(intParamMap, key, value)){
					if(key.size() == 4)
					{
						auto k = key.substr(0, key.size()-1)+"0";
						cout << "k: " << k << "\n";
						if(vec3ParamMap.count(k))
						{
							auto v = vec3ParamMap[k].get();
							string s;
							s +=key.back();
							cout << "s " << s << "\n";
							v[ofToInt(s)] = ofToFloat(value);
							vec3ParamMap[k] = v;
						}
					}
					
					
				}
			}
		}

//		if(paramMap.count(key))
//		{
//			if(paramMap[key])
//			{
//				auto t = paramMap[key]->valueType();
//				if(t == typeid(float).name())
//				{
//					auto k = dynamic_cast<ofParameter<float>*>(paramMap[key]);
//					if(k)
//					{
//					 k-> ofFromString<float>(value);
//					}
//				}
//			}
//		}
//
	}
	
}


//--------------------------------------------------------------
void device::update(ofEventArgs&) {
	if (bConnected) {
		
		while (serial.available() > 0) {
			
			char _byte = (char)serial.readByte();
//			cout <<_byte;
			if (_byte == '\n' || _byte == '\r') {
				if (readBuffer != "") {
					ofLogVerbose(" ofxGrbl ") << "[ RECEIVE ] " << readBuffer ;
					if (readBuffer.find("Grbl") != string::npos) {
						if (!isDeviceReady) {
							isDeviceReady = true;
							sendMessage("$$", true);
							//			sendSettings();
						}
					}else if(readBuffer[0] == '$'){

						readSettingString(readBuffer);
						
					}else
					if (readBuffer == "ok") {
							isReadyToSend = true;
						cout << "ready to send\n";
							//sentCount--;
							//ofLogVerbose(" ofxGrbl ") << "Sent: " << sentCount ;
					}else
					if (readBuffer.substr(0,6) == "error:") {
								ofLogVerbose(" ofxGrbl ") << "[ " << readBuffer << " ]" ;
								//						isPause = true;
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
	if (bConnected && isDeviceReady) {
//		cout << ".";
		// send
		if (isReadyToSend){// && !isPause) {
			if (sendQueList.size() > 0) {
				sendMessage(sendQueList[0], true);
				
				sendQueList.erase(sendQueList.begin());
				ofLogVerbose("device::update") << "Queue  size: " << sendQueList.size() ;
				
				isReadyToSend = false;
			}
		}
	}
}
//--------------------------------------------------------------
void device::close() {
	if (bConnected) {
		
		if (bSpindle) enableSpindle(false, true);
		//		sendMessage("G90 G0 X0 Y0 Z0", true);
		_closeSerial();
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
					if( _settings.mode == OFXGRBL_PLOTTER && !ofIsFloatEqual(tmpVec.z, val) && val < 0.01f){
						polylines.push_back(ofPolyline());
					}
					tmpVec.z = val;
				}
				if(c[0] == 'F'){}
				
				if(!bAbsolutePosition){
					if(polylines.size() > 0){
						if(polylines.back().getVertices().size() > 0){
							tmpVec += polylines.back().getVertices().back();
						}
					}
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
	
	if(p.size()> 0){
		sendMessage(getPositionModeString(OFXGRBL_ABSOLUTE));
		if (_settings.mode == OFXGRBL_PLOTTER) {
			sendMessage("G0 Z1.0");//bring pen up
			setPosition({p[0].x, p[0].y, 1}, true);//set the position t the first polyline vertex
			sendMessage("G1 Z0.0" + getFeedRateString(_settings.speed->z));//bring pen down
		}else{
			setPosition(p[0], true);//set the position t the first polyline vertex
		}
		auto fr =  getFeedRateString(_settings.speed->x);
		if(!fr.empty()){
			sendMessage("G1" + fr);//set the feedrate(speed) of all subsequent moves
		}
		for(size_t i = 1; i < p.size(); i++){
			sendMessage("G1" + vec3ToGcode(p[i]));
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
				ofLogVerbose(" ofxGrbl::device::sendMessage()") << _message;
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
void device::penUp(){
	sendMessage("G1 Z1.0" + getFeedRateString(_settings.speed->z));//bring pen down
}
//--------------------------------------------------------------
void device::penDown(){
	sendMessage("G1 Z0.0" + getFeedRateString(_settings.speed->z));//bring pen down
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
	settingsListeners.unsubscribeAll();
	
	settingsListeners.push(_settings.units.newListener([&](ofxGrblUnits&){
		sendUnits();
	}));
	
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
	
	sendMessage("$32=" + (string)((_settings.mode == OFXGRBL_SPINDLE)?"0":"1"));
	
	// set max speed
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
void device::setOrigin(const glm::vec3& _origin) {
	ofLogVerbose(" ofxGrbl ") << "setOrigin(" << _origin << ")" ;
	_settings.origin = _origin;
	
	setPosition(_settings.origin.get(), true, true);
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
//	if (bConnected || isDeviceReady) {
//		serial.close();
//		bConnected = false;
//		isDeviceReady = false;
//	}
	_closeSerial();
	
	bConnected = serial.setup(_port, _baudrate);
	if (bConnected) {
		
		ofLogVerbose(" ofxGrbl ") << "Connected to " << _port << "@" << _baudrate << " !" ;
	} else {
		ofLogWarning(" ofxGrbl ", "Connect") << "Can not connect. Port " << _port << " does not exists." ;
	}
}
//--------------------------------------------------------------
void device::_closeSerial()
{
	if (bConnected || isDeviceReady) {
		serial.close();
		bConnected = false;
		isDeviceReady = false;
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
		sendMessage((bSpindle?"M3":"M5"), _direct);
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
void device::setUnits(ofxGrblUnits newUnits){
	if(_settings.units.get() != newUnits){
		_settings.units = newUnits;
		//		sendUnits(); // send is going to happen implicitly as there is a listener for _settings.units that will send it
	}
}
//--------------------------------------------------------------
string device::getPositionModeString(ofxGrblPositionMode newMode){
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
string device::getFeedRateString(const float& newFeedrate){
//	cout << "device::getFeedRateString  " << newFeedrate << "  " << lastFeedRateSent << "\n";
	if(!ofIsFloatEqual(newFeedrate, lastFeedRateSent)){
		lastFeedRateSent = newFeedrate;
		return " F"+ ofToString(lastFeedRateSent);
	}
	return "";
}
//--------------------------------------------------------------
void device::setPosition(const glm::vec3& _pos, bool bRapidMovement, bool _sendDirect, ofxGrblPositionMode _positionMode) {
	string msg = getPositionModeString(_positionMode);
	msg += (string)(bRapidMovement?"G0":"G1"); // send rapid or nonrapid movement command
	msg += vec3ToGcode(_pos);// send position
	msg += getFeedRateString(_settings.speed->x);// send feedrate
	
	sendMessage(msg, _sendDirect);
}
//--------------------------------------------------------------
void device::moveRight(float _mm) {
	setPosition({_mm, 0, 0}, false, true,  OFXGRBL_RELATIVE);
}
//--------------------------------------------------------------
void device::moveLeft(float _mm) {
	moveRight(_mm * -1.f);
}
//--------------------------------------------------------------
void device::moveUp(float _mm) {
	setPosition({ 0, _mm, 0}, false, true,  OFXGRBL_RELATIVE);
}
//--------------------------------------------------------------
void device::moveDown(float _mm) {
	moveUp(_mm * -1.f);
}
//--------------------------------------------------------------
string device::vec3ToGcode( const glm::vec3& _vec) {
	return " X" + ofToString(_vec.x , OFX_GRBL_FLOAT_RES) + " Y" + ofToString(_vec.y, OFX_GRBL_FLOAT_RES) + " Z" + ofToString(_vec.z, OFX_GRBL_FLOAT_RES);
}


}


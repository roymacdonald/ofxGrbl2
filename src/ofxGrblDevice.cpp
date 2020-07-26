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

template <typename T>
void parameter<T>::send()
{
	if(_parentDevice){
		if(_isFloat){
			_parentDevice->sendMessage(address+"=" + ofToString(param.get(), OFX_GRBL_FLOAT_RES));
		}
		else
		{
			_parentDevice->sendMessage(address+"=" + ofToString(param.get()));
		}
		
		cout << "param sent: " << address << "=" << ofToString(param.get(), OFX_GRBL_FLOAT_RES) << "\n";
	}
}

template <>
void parameter<glm::vec3>::send()
{
	if(_parentDevice){

		string addr = address.substr(0, address.size()-1);
		
		 _parentDevice->sendMessage( addr + "0=" + ofToString(param->x, OFX_GRBL_FLOAT_RES));
		 _parentDevice->sendMessage( addr + "1=" + ofToString(param->y, OFX_GRBL_FLOAT_RES));
		 _parentDevice->sendMessage( addr + "2=" + ofToString(param->z, OFX_GRBL_FLOAT_RES));
		
	}
}

//--------------------------------------------------------------
device::device()
{
	
	addToMap(vec3ParamMap, "$110", _settings.maxSpeed);
	addToMap(vec3ParamMap, "$120", _settings.accel);
	addToMap(vec3ParamMap, "$130", _settings.maxTravel);

	addToMap(intParamMap, "$0", _settings.stepPulse);
	addToMap(intParamMap, "$1", _settings.stepIdleDelay);
	addToMap(intParamMap, "$2", _settings.stepPortInvertMask);
	addToMap(intParamMap, "$3", _settings.dirPortInvertMask);
	addToMap(intParamMap, "$10", _settings.statusReportMask);
	addToMap(intParamMap, "$23", _settings.homingDirInver);
	addToMap(intParamMap, "$26", _settings.homingDebounce);


	addToMap(boolParamMap, "$4", _settings.stepEnableInvert);
	addToMap(boolParamMap, "$5", _settings.limitPinsInvert);
	addToMap(boolParamMap, "$6", _settings.probePinInvert);
	addToMap(boolParamMap, "$13", _settings.reportInches);
	addToMap(boolParamMap, "$20", _settings.softLimits);
	addToMap(boolParamMap, "$21", _settings.hardLimits);
	addToMap(boolParamMap, "$22", _settings.homingCycles);
	
	addToMap(floatParamMap, "$11", _settings.junctionDeviat);
	addToMap(floatParamMap, "$12", _settings.arcTolerance);
	addToMap(floatParamMap, "$24", _settings.homingFeed);
	addToMap(floatParamMap, "$25", _settings.homingSeek);
	addToMap(floatParamMap, "$27", _settings.homingPullOff);
	addToMap(floatParamMap, "$30", _settings.maxSpindleSpeed);
	addToMap(floatParamMap, "$31", _settings.minSpindleSpeed);

	readBuffer = "";
	status = "";
	
	areaRect.set(0,0, _settings.maxTravel->x, _settings.maxTravel->y);
	
	updateListener = ofEvents().update.newListener(this, &ofxGrbl::device::update);
	exitListener = ofEvents().exit.newListener([&](ofEventArgs&){
		this->close();
	});
	
	settingsListeners.push(_settings.units.newListener([&](ofxGrblUnits&){
		sendUnits();
	}));
	
	settingsListeners.push(_settings.maxTravel.newListener([&](glm::vec3&){
		areaRect.set(0,0, _settings.maxTravel->x, _settings.maxTravel->y);
	}));
	
	
	isReadyToSend = true;
	bConnected = false;
	isDeviceReady = false;
}

//--------------------------------------------------------------
void device::setup() {
	ofLogVerbose("ofxGrbl::device::setup");
	
	// serial
	serial.listDevices();
	
	gui.setup("GRBL", "ofxGrblSettings.xml");
	ports = std::make_unique<ofxDropdown>(port);
	
	ports->enableCollapseOnSelection();
	ports->disableMultipleSelection();

	
	ports->addListener(this, &device::portChanged);
	
//	std::vector<int> bauds = {300, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200, 230400, 12000000};
	
//	baudrates = std::make_unique<ofxIntDropdown>(baudrate, bauds);
//
//	baudrates->addListener(this, &device::baudrateChanged);
	
	
	gui.add(ports.get());
//	gui.add(baudrates.get());
	
	gui.add(getSettingsParams());
	
	
	dropdownListeners.push(ports->dropdownWillShow_E.newListener(this, &device::populatePortsDropdown));

	
	
}

//--------------------------------------------------------------
void device::populatePortsDropdown()
{
	if(ports)
	{
		ports->clear();
		ports->add(getAvailablePorts());
	}
}
void device::portChanged(std::string& p)
{
	if(bIgnorePortChange ) return;
	connect();
}
//void device::baudrateChanged(int& bd)
//{
//	if(bIgnorePortChange ) return;
//	connect();
//}


//--------------------------------------------------------------
void device::setup(string _port) {

	setup();
	
	connect(_port);
	


	
}

vector<std::string> device::getAvailablePorts()
{

	vector<std::string> p;
	
	auto devs = serial.getDeviceList();
	for(auto& d: devs)
	{
		p.push_back(d.getDevicePath());
	}
	return p;
}

//--------------------------------------------------------------
void device::readSettingString(const string& msg)
{
	auto p = readBuffer.find_first_of('=');
	if(p != string::npos)
	{
		auto key = msg.substr(0, p);
		auto value = msg.substr(p+1);
	
		if(setParamFromString(floatParamMap , key , value)){ return;}
		if(setParamFromString(boolParamMap  , key , value)){ return;}
		if(setParamFromString(intParamMap   , key , value)){ return;}
		if(setParamFromString(vec3ParamMap  , key , value)){ return;}
	
		ofLogVerbose("ofxGrbl::device::readSettingString") << "could not read setting string: " + msg ;
	
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
						//cout << "ready to send\n";
							//sentCount--;
							//ofLogVerbose(" ofxGrbl ") << "Sent: " << sentCount ;
					}else
					if (readBuffer.substr(0,6) == "error:") {
								ofLogVerbose(" ofxGrbl ") << "[ ERROR : " << getGrblStatusString(ofToInt(readBuffer.substr(6))) << " ]" ;
							
//						ofLogVerbose(" ofxGrbl ") << "[ ERROR ]";
								//						isPause = true;
								isReadyToSend = true;
					}else if (readBuffer.substr(0,6) == "ALARM:") {
							ofLogVerbose(" ofxGrbl ") << "[ ALARM : " << getGrblAlarmString(ofToInt(readBuffer.substr(6))) << " ]" ;
							
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
//			sendMessage("G0 Z1.0");//bring pen up
			penUp();
			setPosition({p[0].x, p[0].y, 1}, true);//set the position t the first polyline vertex
//			sendMessage("G1 Z0.0" + getFeedRateString(_settings.speed->z, false));//bring pen down
			penDown();
		}else{
			setPosition(p[0], true);//set the position t the first polyline vertex
		}
		auto fr =  getFeedRateString(_settings.speed->x, false);
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
//	cout << _msg << endl;
	if (direct) {
		if (bConnected) {
			if (_msg != "") {

				string _message = _msg + "\n";
				unsigned char* writeByte = (unsigned char*)_message.c_str();
				serial.writeBytes(writeByte, _message.length());
				ofLogVerbose(" ofxGrbl::device::sendMessage()") << _msg;
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
	sendMessage("G1 Z1.0" + getFeedRateString(_settings.speed->z, false));//bring pen down
}
//--------------------------------------------------------------
void device::penDown(){
	sendMessage("G1 Z0.0" + getFeedRateString(_settings.speed->z, false));//bring pen down
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
	
//	settingsListeners.unsubscribeAll();
	
//	settingsListeners.push(_settings.units.newListener([&](ofxGrblUnits&){
//		sendUnits();
//	}));
//
//	settingsListeners.push(_settings.maxTravel.newListener([&](glm::vec3&){
//		areaRect.set(0,0, _settings.maxTravel->x, _settings.maxTravel->y);
//	}));
	
	
	if(_settings.load(this->settingsFileName)){
//		sendSettings();
//		if (firstTimeLoad) {
//			ofLogVerbose(" ofxGrbl ") << "FirstTimeLoad!" ;
//			firstTimeLoad = false;
//			//			currentPos = glm::vec3(_settings.homePosition->x  * _settings.maxTravel->x, _settings.homePosition->y  * _settings.maxTravel->y, _settings.homePosition->z  * _settings.maxTravel->z);
//		}
	}
}
//--------------------------------------------------------------
void device::sendSettings() {
	
	for(auto& p :floatParamMap)
	{
		p.second.send();
	}
	for(auto& p :boolParamMap)
	{
		p.second.send();
	}
	for(auto& p :intParamMap)
	{
		p.second.send();
	}
	for(auto& p :vec3ParamMap)
	{
		p.second.send();
	}

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
void device::connect(string _port){//}, int _baudrate) {
	bIgnorePortChange = true;
	port = _port;
//	baudrate = _baudrate;
	bIgnorePortChange = false;
	connect();
	
}
//--------------------------------------------------------------
void device::connect() {
	
	ofLogVerbose(" ofxGrbl ") << "Connect( " << port.get() << ", " << baudrate << " )" ;
	
	// reset serial
//	if (bConnected || isDeviceReady) {
//		serial.close();
//		bConnected = false;
//		isDeviceReady = false;
//	}
	_closeSerial();
	
	bConnected = serial.setup(port.get(), baudrate);
	if (bConnected) {
		
		ofLogVerbose(" ofxGrbl ") << "Connected to " << port << "@" << baudrate << " !" ;
	} else {
		ofLogWarning(" ofxGrbl ", "Connect") << "Can not connect. Port " << port << " does not exists." ;
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
string device::getFeedRateString(const float& newFeedrate, bool bRapidMovement){
//	cout << "device::getFeedRateString  " << newFeedrate << "  " << lastFeedRateSent << "\n";
	
//	int index = bRapidMovement?0:1;
//	if(!ofIsFloatEqual(newFeedrate, lastFeedRateSent[index])){
//		lastFeedRateSent[index] = newFeedrate;
//		cout << "device::getFeedRateString  " << lastFeedRateSent[index] << " Rapid Movement: " << boolalpha << bRapidMovement << "\n";
//		return " F"+ ofToString(lastFeedRateSent[index]);
//	}
//	return "";
	return " F"+ ofToString(newFeedrate, OFX_GRBL_FLOAT_RES);
}
//--------------------------------------------------------------
void device::setPosition(const glm::vec3& _pos, bool bRapidMovement, bool _sendDirect, ofxGrblPositionMode _positionMode) {
	string msg = getPositionModeString(_positionMode);
	msg += (string)(bRapidMovement?"G0":"G1"); // send rapid or nonrapid movement command
	msg += vec3ToGcode(_pos);// send position
	msg += getFeedRateString(_settings.speed->x, bRapidMovement);// send feedrate
	
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


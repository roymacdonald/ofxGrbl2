//
//  ofxGrblDevice.hpp
//   ofxGrbl
//
//  Created by Roy Macdonald on 7/9/19.
//
//

#pragma once
#pragma once

#include "ofMain.h"
#include "ofxGrblSettings.h"
#include "ofxDropdown.h"
#include "ofxGui.h"



namespace ofxGrbl{

class device;

template <typename T>
class parameter
{
public:
	parameter(){}
	void setup(device* parentDevice, const std::string& addr, ofParameter<T>& mom)
	{
		address = addr;
		_parentDevice = parentDevice;
		
		param.makeReferenceTo(mom);
		listener = make_unique<ofEventListener>(param.newListener(this, &parameter::paramChanged));
	}
	std::string address;
	ofParameter<T> param;
	unique_ptr<ofEventListener> listener = nullptr;
	
	void setWithoutEventNotifications(T value)
	{
		_bDontSend = true;
		param = value;
		_bDontSend = false;
	}
	
	void paramChanged(T&)
	{
		if(!_bDontSend)
			send();
	}
	
	void send();
	
private:
	
	const bool _isFloat = std::is_floating_point<T>::value;
	
	device* _parentDevice = nullptr;

	bool _bDontSend = false;
	
	
	
};


class device{
	
public:
	
	device();
	
	void setup();
	
	void setup(string _port);
	
	// serial
	void connect();
	void connect(string _port);
	bool isConnected();
	
	vector<std::string> getAvailablePorts();
	
	void close();
	
	void saveStrokesToGCodeFile(const std::string& _path);
	void loadGCodeFromFile(const std::string& _path);
	
	void sendMessage(const std::string& _msg, bool direct = false);
	
	
	
	void clear();
	
	void setOrigin(const glm::vec3& _homePos);
	void goHome();
	
	void enableSpindle(bool _enable, bool _direct = false);
	bool isSpindleEnabled();
	void setSpindleSpeed(int _speed, bool _direct = false);
	
	
	
	void killAlarmLock();
	
	void moveRight(float _mm);
	void moveLeft(float _mm);
	void moveUp(float _mm);
	void moveDown(float _mm);
	
	void setPosition(const glm::vec3& _pos, bool bRapidMovement = false, bool _sendDirect = false, ofxGrblPositionMode positionMode = OFXGRBL_ABSOLUTE);
	
	// Settings
	void saveSettings(const std::string& settingsFileName = "");
	void loadSettings(const std::string& settingsFileName = "");

	
	void send(const ofPath& path);
	void send(const ofPolyline& poly);
	void send(const std::vector<ofPolyline>& polys);
	void send(const std::vector<glm::vec3>& points);
	
	// force the z position of the polys and paths sent
	void send(const ofPath& path, float z);
	void send(const ofPolyline& poly, float z);
	void send(const std::vector<ofPolyline>& polys, float z);
	void send(const std::vector<glm::vec3>& points, float z);
	
	
	/// sets the device to use either millimeters or inches as the units being used for the commands. Default is millimeters
	/// Possible parameters passed are either OFXGRBL_MILLIMETERS or OFXGRBL_INCHES
	void setUnits(ofxGrblUnits newUnits);
	
	
	// events
	ofEvent<bool> UpDownEvent;
	ofEvent<glm::vec3> PositionEvent;
	
	ofRectangle getArea(){return areaRect;}
	
	
	void penUp();
	void penDown();
	
	ofParameterGroup& getSettingsParams(){return _settings.parameters;}

	ofxGrblSettings& getSettings(){return _settings;}
	
	void sendSettings();
	
	bool canSend(){return isReadyToSend;}
	
	
	ofxPanel gui;
	
	
protected:
	
	std::unique_ptr<ofxDropdown> ports = nullptr;
//	std::unique_ptr<ofxIntDropdown> baudrates = nullptr;
	
	void portChanged(std::string& port);
//	void baudrateChanged(int& bd);
	ofEventListeners dropdownListeners;
	
	void populatePortsDropdown();
//	void populateBaudratesDropdown();
	
	string vec3ToGcode( const glm::vec3& _vec);
	
	glm::vec3 currentPos;
	string settingsFileName;
	ofRectangle areaRect;

	void update(ofEventArgs&);
	ofEventListener updateListener;

	void sendUnits();

	
	ofSerial serial;
	bool bConnected;
	bool isDeviceReady;
	ofParameter<string> port = {"port", ""};
//	ofParameter<int> baudrate = {"baudrate", 115200, 300, 12000000};

	int baudrate = 115200;
	
	void _closeSerial();
	
private:
	
	
	bool bIgnorePortChange = false;
	
	
	vector<string> sendQueList;

	
	
	float lastFeedRateSent[2] = {0.0, 0.0};
	
	string getFeedRateString(const float& newFeedrate, bool bRapidMovement);
	
	ofxGrblPositionMode positionMode = OFXGRBL_ABSOLUTE;
	string getPositionModeString(ofxGrblPositionMode newMode);
	
//	bool firstTimeLoad;
	string status;

	bool isReadyToSend;
	
	string readBuffer;

	bool isDown = false;
	bool bSpindle = false;
	
	vector<ofPolyline> polylines;
	
	// Grbl Settings
	ofxGrblSettings _settings;
	
	ofEventListeners settingsListeners;

	std::map<std::string, parameter<float>> floatParamMap;
	std::map<std::string, parameter<bool>> boolParamMap;
	std::map<std::string, parameter<int>> intParamMap;
	std::map<std::string, parameter<glm::vec3>> vec3ParamMap;
	
	void readSettingString(const string& msg);

	template<typename T>
	bool setParamFromString(std::map<std::string, parameter<T>>& group, const string& key, const string& value)
	{
		if(group.count(key))
		{
			group[key].setWithoutEventNotifications(ofFromString<T>(value));
			return true;
		}
		return false;
	}
	
	template<>
	bool setParamFromString<glm::vec3>(std::map<std::string, parameter<glm::vec3>>& group, const string& key, const string& value)
	{
//		if(group.count(key))
//		{
			if(key.size() == 4)
			{
				auto k = key.substr(0, key.size()-1)+"0";
//				cout <<"key: " << key <<  " k: " << k << "\n";
				if(group.count(k))
				{
					auto v = vec3ParamMap[k].param.get();
					string s;
					s +=key.back();
//					cout << "s " << s << "\n";
					v[ofToInt(s)] = ofToFloat(value);
					vec3ParamMap[k].setWithoutEventNotifications(v);
					return true;
				}
			}
			
			
//			group[key].param.setWithoutEventNotifications(ofFromString<glm::vec3>(value));
//			return true;
//		}
		return false;
	}
	
	
	
	
	template<typename T>
	void addToMap(std::map<std::string, parameter<T>> & paramMap, const std::string& addr, ofParameter<T>& param)
	{
		paramMap[addr].setup(this, addr, param);
	}

};




}

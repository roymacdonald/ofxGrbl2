//
//  ofxGrblUtils.h
//  example-SimpleGrbl
//
//  Created by Roy Macdonald on 7/11/19.
//
//

#pragma once

namespace ofxGrbl{

#define OFX_GRBL_FLOAT_RES 6
	
	
	bool checkMoveCommand(const std::string& _line){
		return (_line.find('X') != string::npos) || (_line.find('Y') != string::npos) || (_line.find('Z') != string::npos);
	}
	//--------------------------------------------------------------
	glm::vec3 gcodeToVec3(const std::string& _line){
		glm::vec3 _result = {0.,0.,0.};
		vector<string> _commands = ofSplitString(_line, " ", true);
		for (int i = 0; i < _commands.size(); i++) {
			if (_commands[i][0] == 'X') {
				if (_commands[i].size() == 1) {
					// space parse
					_result.x = ofToFloat(_commands[i + 1]); // use next character
				}
				else {
					// no space parse
					_result.x = ofToFloat(_commands[i].substr(1));
				}
			}
			else if (_commands[i][0] == 'Y') {
				if (_commands[i].size() == 1) {
					// space parse
					_result.y = ofToFloat(_commands[i + 1]); // use next character
				}
				else {
					// no space parce
					_result.y = ofToFloat(_commands[i].substr(1));
				}
			}
			else if (_commands[i][0] == 'Z') {
				if (_commands[i].size() == 1) {
					// space parse
					_result.z = ofToFloat(_commands[i + 1]); // use next character
				}
				else {
					// no space parce
					_result.z = ofToFloat(_commands[i].substr(1));
				}
			}
		}
		
		return _result;
	}
	//--------------------------------------------------------------
	string vec3ToGcode( glm::vec3 _vec, const ofRectangle& clampRect  ) {
		if(!clampRect.isEmpty()){
			_vec.x = ofClamp(_vec.x, clampRect.getMinX(),clampRect.getMaxX());
			_vec.y = ofClamp(_vec.y, clampRect.getMinY(),clampRect.getMaxY());
			
			std::cout << "clamping vec" << _vec << "  rect " << clampRect << std::endl;
			
		}
		
		return " X" + ofToString(_vec.x , OFX_GRBL_FLOAT_RES) + " Y" + ofToString(_vec.y, OFX_GRBL_FLOAT_RES) + " Z" + ofToString(_vec.z, OFX_GRBL_FLOAT_RES);;
		
	}
	
	
}

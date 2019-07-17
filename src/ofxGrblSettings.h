#pragma once
#include "ofMain.h"

enum ofxGrblMode{
	OFXGRBL_SPINDLE,
	OFXGRBL_LASER,
	OFXGRBL_PLOTTER
} ;
enum ofxGrblUnits{
	OFXGRBL_MILLIMETERS,
	OFXGRBL_INCHES
};
class ofxGrblSettings{
	// About Grbl Settings
	//	https://github.com/grbl/grbl/wiki/Configuring-Grbl-v0.9

public:

ofxGrblSettings(){
	parameters.setName("GRBL SETTINGS");
	parameters.add(stepPulse);
	parameters.add(stepIdleDelay);
	parameters.add(stepPortInvertMask);
	parameters.add(dirPortInvertMask);
	parameters.add(stepEnableInvert);
	parameters.add(limitPinsInvert);
	parameters.add(probePinInvert);
	parameters.add(statusReportMask);
	parameters.add(junctionDeviat);
	parameters.add(arcTolerance);
	parameters.add(reportInches);
	parameters.add(softLimits);
	parameters.add(hardLimits);
	parameters.add(homingCycles);
	parameters.add(homingDirInver);
	parameters.add(homingFeed);
	parameters.add(homingSeek);
	parameters.add(homingDebounce);
	parameters.add(homingPullOff);
	parameters.add(homePosition);
	parameters.add(step);
	parameters.add(maxTravel);
//	parameters.add(stepParMillimeter);
	parameters.add(accel);
	parameters.add(maxSpeed);
//	parameters.add(stepInvertX);
//	parameters.add(stepInvertY);
//	parameters.add(stepInvertZ);
	parameters.add(mode);
	parameters.add(spindleSpeed);
	parameters.add(maxSpindleSpeed);
	parameters.add(minSpindleSpeed);
//	parameters.add(laserModeEnable);
	parameters.add(isUseZAxis);
	parameters.add(upPos);
	parameters.add(downPos);



}


	
	
	ofParameter<int> stepPulse =          { "Step Pulse", 10  };            // $0   (step pulse, usec)
	ofParameter<int> stepIdleDelay =      { "Step Idle Delay", 25  };       // $1   (step idle delay, msec)
	ofParameter<int> stepPortInvertMask = { "Step Port Invert Mask", 0  };  // $2   (step port invert mask:00000000)
	ofParameter<int> dirPortInvertMask =  { "Dir Port Invert Mask", 0  };   // $3   (dir port invert mask:00000110)
	ofParameter<bool> stepEnableInvert =  { "Step Enable Invert", false  }; // $4   (step enable invert, bool)
	ofParameter<bool> limitPinsInvert =   { "Limit Pins Invert", false  };  // $5   (limit pins invert, bool)
	ofParameter<bool> probePinInvert =    { "Probe Pin Invert", false  };   // $6   (probe pin invert, bool)
	ofParameter<int> statusReportMask =   { "Status Report Mask", 1  };     // $10  (status report mask:00000011
	ofParameter<float> junctionDeviat =   { "Junction Deviation", 0.020};   // $11  (junction deviation, mm);
	ofParameter<float> arcTolerance =     { "Arc Tolerance", 0.002};        // $12  (arc tolerance, mm);
	ofParameter<bool> reportInches =      { "Report Inches", false  };      // $13  (report inches, bool);
	ofParameter<bool> softLimits =        { "Soft Limits", false  };        // $20  (soft limits, bool)
	ofParameter<bool> hardLimits =        { "Hard Limits", true  };         // $21  (hard limits, bool)
	ofParameter<bool> homingCycles =      { "Homing Cycles", true  };       // $22  (homing cycle, bool)
	ofParameter<int> homingDirInver =     { "Homing Dir Inver", 11  };      // $23  (homing dir invert mask:00000001)tMask;
	ofParameter<float> homingFeed =       { "Homing Feed", 500};            // $24  (homing feed, mm/min)
	ofParameter<float> homingSeek =       { "Homing Seek", 2000 };          // $25  (homing seek, mm/min)
	ofParameter<int> homingDebounce =     { "Homing Debounce", 250  };      // $26  (homing debounce, msec)
	ofParameter<float> homingPullOff =    { "Homing Pull-off", 5};          // $27  (homing pull-off, mm)ullOff;
	ofParameter<float> maxSpindleSpeed =  { "Maximum spindle speed", 255 }; // $30  Maximum spindle speed. Sets PWM to 100% duty cycle.
	ofParameter<float> minSpindleSpeed =  { "Minimum spindle speed", 0   }; // $31  Minimum spindle speed. Sets PWM to 0.4% or lowest duty cycle.

	ofParameter<ofxGrblUnits> units =      { "Units",   OFXGRBL_MILLIMETERS};   // G21 (All units in mm) G20 (All units in inches)

	// Step
	// Per axis travel resolution in steps per millimeter.
	// units : step/mm 
	ofParameter<glm::vec3> step = {"Step Resolution",
		{
			87.580, // $100 (x, step/mm)
			87.580, // $101 (y, step/mm)
			4000.0  // $102 (z, step/mm)
		} 
	};
	

	// Max Rate
	// X-axis maximum rate. Used as G0 rapid rate.
	// units : mm/min
	ofParameter<glm::vec3> maxSpeed = {"Max Speed", 
		{
			10000,  //$110 (x max rate, mm/min)
			10000, 	//$111 (y max rate, mm/min)
			1000	//$112 (z max rate, mm/min)
		}
	};
	
	ofParameter<glm::vec3> speed = {"Speed",
		{
			8000,
			8000,
			100
		}
	};
	
	
	ofParameter<glm::vec3> homePosition = {"Home Position", {0.0f,0.0f,0.0f}}; 

	// Accelaration
	// Per axis acceleration. Used for motion planning to not exceed motor torque and lose steps. 
	// units : mm/sec^2     
	ofParameter<glm::vec3> accel = {"Acceleration", 
		{
			400.0, 	//$120 (X-axis acceleration, mm/sec^2)
			400.0,  //$121 (Y-axis acceleration, mm/sec^2)
			1000.0	//$122 (Z-axis acceleration, mm/sec^2)
		}
	};

	// Max Travel
	// Maximum travel distance per axis from homing switch. 
	// Determines valid machine space for soft-limits and homing search distances.
	// units : millimeters
	ofParameter<glm::vec3> maxTravel = {"Max Travel", 
		{
			290.0, //$130 (x max travel, mm)
			350.0, //$131 (y max travel, mm)
			200.0  //$132 (z max travel, mm)
		}
	};
	

	
	// Step Invert
//	ofParameter<bool> stepInvertX = {"Step Invert X", false};
//	ofParameter<bool> stepInvertY = {"Step Invert Y", false};
//	ofParameter<bool> stepInvertZ = {"Step Invert Z", false};

	// Mode (Spindle/Laser/Plotter)
	ofParameter<ofxGrblMode> mode = {"mode", OFXGRBL_PLOTTER };

	// Spindle Speed (S0-S1000)
	ofParameter<float> spindleSpeed = {"Spindle Speed", 0};
	// Protter Push Distance (0-100mm)
//	/ofParameter<float> pushDistance = {"Push Distance", };

	// Z Axis Settings
	ofParameter<bool> isUseZAxis = {"is Use Z Axis", true };
	ofParameter<float> upPos	 = {"Up Pos", 60};
	ofParameter<float> downPos   = {"Down Pos", 90};


	ofParameterGroup parameters;

	
	//--------------------------------------------------------------
	void save(const std::string& settingsFileName) {
		ofLogVerbose(" ofxGrbl ") << "saveSettings to " << settingsFileName ;
		ofXml xml;
		ofSerialize(xml, parameters);
		xml.save(settingsFileName);
	}
	//--------------------------------------------------------------
	bool load(const std::string& settingsFileName) {
		ofLogVerbose(" ofxGrbl ") << "loadSettings" ;
		
		ofXml xml;
		if(xml.load(settingsFileName)){
			ofDeserialize(xml, parameters);
			return true;
		}
		return false;
	}
	
};

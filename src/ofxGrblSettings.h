#pragma once
#include "ofMain.h"

#include "ofxGrblConstants.h"
#include "ofxGui.h"
#include "ofxDropdown.h"

class ParameterXYZ
: public ofParameterGroup
{
public:
	ParameterXYZ(const std::string & name, const glm::vec2& val, const glm::vec2& _min, const glm::vec2& _max )
	{
		setName(name);
		add(xy.set("xy", val.x, _min.x, _max.x));
		add(z.set("z", val.y, _min.y, _max.y));
		
	}

	
	virtual ~ParameterXYZ()
	{
		
	}
	
	
	ofParameter<float> xy;
	ofParameter<float> z;
};


class ofxGrblSettings{
	// About Grbl Settings
	//	https://github.com/grbl/grbl/wiki/Configuring-Grbl-v0.9

public:

ofxGrblSettings(){
	parameters.setName("GRBL SETTINGS");
    guiParams.setName("GRBL SETTINGS");
	
	steppersParams.setName("Steppers");
	homingParams.setName("Homing");
	limitsParams.setName("Limits");
	zAxisParams.setName("Z-Axis");
	spindleParams.setName("Spindle");
	otherParams.setName("Other");
	
	steppersParams.add(stepPulse);
	steppersParams.add(stepIdleDelay);
	steppersParams.add(stepPortInvertMask);
	steppersParams.add(dirPortInvertMask);
	steppersParams.add(stepEnableInvert);
	steppersParams.add(step);
	steppersParams.add(accel);
	steppersParams.add(maxSpeed);
	steppersParams.add(speed);

	otherParams.add(probePinInvert);
	otherParams.add(statusReportMask);
	otherParams.add(junctionDeviat);
	otherParams.add(arcTolerance);
	otherParams.add(reportInches);

	limitsParams.add(limitPinsInvert);
	limitsParams.add(softLimits);
	limitsParams.add(hardLimits);

	homingParams.add(homingCycles);
	homingParams.add(homingDirInver);
	homingParams.add(homingFeed);
	homingParams.add(homingSeek);
	homingParams.add(homingDebounce);
	homingParams.add(homingPullOff);

	spindleParams.add(spindleSpeed);
	spindleParams.add(maxSpindleSpeed);
	spindleParams.add(minSpindleSpeed);
	parameters.add(laserModeEnable);
	zAxisParams.add(isUseZAxis);
	zAxisParams.add(upPos);
	zAxisParams.add(downPos);


	parameters.add(origin);
	
	parameters.add(maxTravel);

	parameters.add(mode);


	parameters.add(steppersParams);
	parameters.add(homingParams);
	parameters.add(limitsParams);
	parameters.add(zAxisParams);
	parameters.add(spindleParams);
	parameters.add(otherParams);

    
    
    std::map<int, string> modeOptions = {
            {(int)OFXGRBL_SPINDLE, "SPINDLE"},
            {(int)OFXGRBL_LASER, "LASER"},
            {(int)OFXGRBL_PLOTTER, "PLOTTER"}
    };
    
    
    
    
    
    
    modeDropdown = std::make_unique<ofxIntDropdown>(mode, modeOptions);
    
    modeDropdown->enableCollapseOnSelection();
    modeDropdown->disableMultipleSelection();
    
    guiParams.add(origin);
    guiParams.add(maxTravel);
    
//    parameters.add(laserModeEnable);
    
    guiParams.add(steppersParams);
    guiParams.add(homingParams);
    guiParams.add(limitsParams);
    guiParams.add(zAxisParams);
    guiParams.add(spindleParams);
    guiParams.add(otherParams);
    
    modeListener = mode.newListener(this, &ofxGrblSettings::modeChanged);
    
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
	ofParameter<bool> laserModeEnable =   { "Laser Mode", false};           // $32
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
	ParameterXYZ maxSpeed = {"Max Speed",
		{
			16000,  //$110 (x max rate, mm/min)
//			16000, 	//$111 (y max rate, mm/min)
			1000	//$112 (z max rate, mm/min)
		},
		{100.,100.},//,0.},
		{20000.,5000.}
	};
	
	ParameterXYZ speed = {"Speed",
		{
			9000,
//			14000,
			100
		},
		{100.,100.},
		{20000.,25000.}
	};
	
	
	ofParameter<glm::vec3> origin = {"Home Position", {0.0f,0.0f,0.0f}};

	// Accelaration
	// Per axis acceleration. Used for motion planning to not exceed motor torque and lose steps. 
	// units : mm/sec^2     
	ParameterXYZ accel = {"Acceleration",
		{
			1500.0, 	//$120 (X-axis acceleration, mm/sec^2)
//			6000.0,  //$121 (Y-axis acceleration, mm/sec^2)
			1000.0	//$122 (Z-axis acceleration, mm/sec^2)
		},
		{0.,0.},
		{10000.,50000.}
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
	ofParameter<int> mode = {"mode", OFXGRBL_PLOTTER, 0, 2 };

	// Spindle Speed (S0-S1000)
	ofParameter<float> spindleSpeed = {"Spindle Speed", 0};
	// Protter Push Distance (0-100mm)
//	/ofParameter<float> pushDistance = {"Push Distance", };

	// Z Axis Settings
	ofParameter<bool> isUseZAxis = {"is Use Z Axis", true };
	ofParameter<float> upPos	 = {"Up Pos", 60, 0, 180};
	ofParameter<float> downPos   = {"Down Pos", 10, 0, 180};


	ofParameterGroup parameters;
	ofParameterGroup steppersParams;
	ofParameterGroup homingParams;
	ofParameterGroup limitsParams;
	ofParameterGroup zAxisParams;
	ofParameterGroup spindleParams;
	ofParameterGroup otherParams;
//	//--------------------------------------------------------------
//	void save(const std::string& settingsFileName) {
//		ofLogVerbose(" ofxGrbl ") << "saveSettings to " << settingsFileName ;
//		ofXml xml;
//		ofSerialize(xml, parameters);
//		xml.save(settingsFileName);
//
//	}
//	//--------------------------------------------------------------
//	bool load(const std::string& settingsFileName) {
//		ofLogVerbose(" ofxGrbl ") << "loadSettings" ;
//
//		ofXml xml;
//		if(xml.load(settingsFileName)){
//			ofDeserialize(xml, parameters);
//			return true;
//		}
//		return false;
//	}
//    
    
    void putIntoGui(ofxPanel& gui){
        gui.add(modeDropdown.get());
        gui.add(guiParams);
    }
private:
    
    void modeChanged(int&){
        laserModeEnable = (mode.get() == OFXGRBL_LASER);
    }
    
    std::unique_ptr<ofxIntDropdown> modeDropdown = nullptr;
    ofParameterGroup guiParams;
    ofEventListener modeListener;
	
};

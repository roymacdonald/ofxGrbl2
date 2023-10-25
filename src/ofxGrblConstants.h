//
//  ofxGrblConstants.h
//  example-SimpleGrbl
//
//  Created by Roy Macdonald on 7/16/19.
//
//

#pragma once
#include <string>


#define OFX_GRBL_FLOAT_RES 6


//enum ofxGrblMode{
#define OFXGRBL_SPINDLE 0
#define OFXGRBL_LASER 1
#define OFXGRBL_PLOTTER 2
//} ;

enum ofxGrblUnits{
	OFXGRBL_MILLIMETERS,
	OFXGRBL_INCHES
};

enum ofxGrblPositionMode {
	OFXGRBL_ABSOLUTE,
	OFXGRBL_RELATIVE
};


// The following defines have been taken from the GRBL firmware. These are returned by the machine in case of error, alarm or debuging.
// https://github.com/grbl/grbl/blob/master/grbl/report.h

// Define Grbl status codes. Valid values (0-255)
#define GRBL_STATUS_OK                      0
#define GRBL_STATUS_EXPECTED_COMMAND_LETTER 1
#define GRBL_STATUS_BAD_NUMBER_FORMAT       2
#define GRBL_STATUS_INVALID_STATEMENT       3
#define GRBL_STATUS_NEGATIVE_VALUE          4
#define GRBL_STATUS_SETTING_DISABLED        5
#define GRBL_STATUS_SETTING_STEP_PULSE_MIN  6
#define GRBL_STATUS_SETTING_READ_FAIL       7
#define GRBL_STATUS_IDLE_ERROR              8
#define GRBL_STATUS_SYSTEM_GC_LOCK          9
#define GRBL_STATUS_SOFT_LIMIT_ERROR        10
#define GRBL_STATUS_OVERFLOW                11
#define GRBL_STATUS_MAX_STEP_RATE_EXCEEDED  12
#define GRBL_STATUS_CHECK_DOOR              13
#define GRBL_STATUS_LINE_LENGTH_EXCEEDED    14
#define GRBL_STATUS_TRAVEL_EXCEEDED         15
#define GRBL_STATUS_INVALID_JOG_COMMAND     16
#define GRBL_STATUS_SETTING_DISABLED_LASER  17

#define GRBL_STATUS_GCODE_UNSUPPORTED_COMMAND       20
#define GRBL_STATUS_GCODE_MODAL_GROUP_VIOLATION     21
#define GRBL_STATUS_GCODE_UNDEFINED_FEED_RATE       22
#define GRBL_STATUS_GCODE_COMMAND_VALUE_NOT_INTEGER 23
#define GRBL_STATUS_GCODE_AXIS_COMMAND_CONFLICT     24
#define GRBL_STATUS_GCODE_WORD_REPEATED             25
#define GRBL_STATUS_GCODE_NO_AXIS_WORDS             26
#define GRBL_STATUS_GCODE_INVALID_LINE_NUMBER       27
#define GRBL_STATUS_GCODE_VALUE_WORD_MISSING        28
#define GRBL_STATUS_GCODE_UNSUPPORTED_COORD_SYS     29
#define GRBL_STATUS_GCODE_G53_INVALID_MOTION_MODE   30
#define GRBL_STATUS_GCODE_AXIS_WORDS_EXIST          31
#define GRBL_STATUS_GCODE_NO_AXIS_WORDS_IN_PLANE    32
#define GRBL_STATUS_GCODE_INVALID_TARGET            33
#define GRBL_STATUS_GCODE_ARC_RADIUS_ERROR          34
#define GRBL_STATUS_GCODE_NO_OFFSETS_IN_PLANE       35
#define GRBL_STATUS_GCODE_UNUSED_WORDS              36
#define GRBL_STATUS_GCODE_G43_DYNAMIC_AXIS_ERROR    37
#define GRBL_STATUS_GCODE_MAX_VALUE_EXCEEDED        38

// Define Grbl alarm codes. Valid values (1-255). 0 is reserved.
#define GRBL_ALARM_HARD_LIMIT_ERROR      1
#define GRBL_ALARM_SOFT_LIMIT_ERROR      2
#define GRBL_ALARM_ABORT_CYCLE           3
#define GRBL_ALARM_PROBE_FAIL_INITIAL    4
#define GRBL_ALARM_PROBE_FAIL_CONTACT    5
#define GRBL_ALARM_HOMING_FAIL_RESET     6
#define GRBL_ALARM_HOMING_FAIL_DOOR      7
#define GRBL_ALARM_HOMING_FAIL_PULLOFF   8
#define GRBL_ALARM_HOMING_FAIL_APPROACH  9

// Define Grbl feedback message codes. Valid values (0-255).
#define GRBL_MESSAGE_CRITICAL_EVENT   1
#define GRBL_MESSAGE_ALARM_LOCK       2
#define GRBL_MESSAGE_ALARM_UNLOCK     3
#define GRBL_MESSAGE_ENABLED          4
#define GRBL_MESSAGE_DISABLED         5
#define GRBL_MESSAGE_SAFETY_DOOR_AJAR 6
#define GRBL_MESSAGE_CHECK_LIMITS     7
#define GRBL_MESSAGE_PROGRAM_END      8
#define GRBL_MESSAGE_RESTORE_DEFAULTS 9
#define GRBL_MESSAGE_SPINDLE_RESTORE  10
#define GRBL_MESSAGE_SLEEP_MODE       11


static std::string getGrblStatusString(int status)
{

	switch(status)
	{
		case GRBL_STATUS_OK: return "Ok";
		case GRBL_STATUS_EXPECTED_COMMAND_LETTER: return "Expected Command Letter";
		case GRBL_STATUS_BAD_NUMBER_FORMAT: return "Bad Number Format";
		case GRBL_STATUS_INVALID_STATEMENT: return "Invalid Statement";
		case GRBL_STATUS_NEGATIVE_VALUE: return "Negative Value";
		case GRBL_STATUS_SETTING_DISABLED: return "Setting Disabled";
		case GRBL_STATUS_SETTING_STEP_PULSE_MIN: return "Setting Step Pulse Min";
		case GRBL_STATUS_SETTING_READ_FAIL: return "Setting Read Fail";
		case GRBL_STATUS_IDLE_ERROR: return "Idle Error";
		case GRBL_STATUS_SYSTEM_GC_LOCK: return "System Gc Lock";
		case GRBL_STATUS_SOFT_LIMIT_ERROR: return "Soft Limit Error";
		case GRBL_STATUS_OVERFLOW: return "Overflow";
		case GRBL_STATUS_MAX_STEP_RATE_EXCEEDED: return "Max Step Rate Exceeded";
		case GRBL_STATUS_CHECK_DOOR: return "Check Door";
		case GRBL_STATUS_LINE_LENGTH_EXCEEDED: return "Line Length Exceeded";
		case GRBL_STATUS_TRAVEL_EXCEEDED: return "Travel Exceeded";
		case GRBL_STATUS_INVALID_JOG_COMMAND: return "Invalid Jog Command";
		case GRBL_STATUS_SETTING_DISABLED_LASER: return "Setting Disabled Laser";
		case GRBL_STATUS_GCODE_UNSUPPORTED_COMMAND: return "Gcode Unsupported Command";
		case GRBL_STATUS_GCODE_MODAL_GROUP_VIOLATION: return "Gcode Modal Group Violation";
		case GRBL_STATUS_GCODE_UNDEFINED_FEED_RATE: return "Gcode Undefined Feed Rate";
		case GRBL_STATUS_GCODE_COMMAND_VALUE_NOT_INTEGER: return "Gcode Command Value Not Integer";
		case GRBL_STATUS_GCODE_AXIS_COMMAND_CONFLICT: return "Gcode Axis Command Conflict";
		case GRBL_STATUS_GCODE_WORD_REPEATED: return "Gcode Word Repeated";
		case GRBL_STATUS_GCODE_NO_AXIS_WORDS: return "Gcode No Axis Words";
		case GRBL_STATUS_GCODE_INVALID_LINE_NUMBER: return "Gcode Invalid Line Number";
		case GRBL_STATUS_GCODE_VALUE_WORD_MISSING: return "Gcode Value Word Missing";
		case GRBL_STATUS_GCODE_UNSUPPORTED_COORD_SYS: return "Gcode Unsupported Coord Sys";
		case GRBL_STATUS_GCODE_G53_INVALID_MOTION_MODE: return "Gcode G53 Invalid Motion Mode";
		case GRBL_STATUS_GCODE_AXIS_WORDS_EXIST: return "Gcode Axis Words Exist";
		case GRBL_STATUS_GCODE_NO_AXIS_WORDS_IN_PLANE: return "Gcode No Axis Words In Plane";
		case GRBL_STATUS_GCODE_INVALID_TARGET: return "Gcode Invalid Target";
		case GRBL_STATUS_GCODE_ARC_RADIUS_ERROR: return "Gcode Arc Radius Error";
		case GRBL_STATUS_GCODE_NO_OFFSETS_IN_PLANE: return "Gcode No Offsets In Plane";
		case GRBL_STATUS_GCODE_UNUSED_WORDS: return "Gcode Unused Words";
		case GRBL_STATUS_GCODE_G43_DYNAMIC_AXIS_ERROR: return "Gcode G43 Dynamic Axis Error";
		case GRBL_STATUS_GCODE_MAX_VALUE_EXCEEDED: return "Gcode Max Value Exceeded";
	}

	return "Invalid error code";
}

static std::string getGrblAlarmString(int alarm)
{
	switch(alarm)
	{
		case GRBL_ALARM_HARD_LIMIT_ERROR: return "Hard Limit Error";
		case GRBL_ALARM_SOFT_LIMIT_ERROR: return "Soft Limit Error";
		case GRBL_ALARM_ABORT_CYCLE: return "Abort Cycle";
		case GRBL_ALARM_PROBE_FAIL_INITIAL: return "Probe Fail Initial";
		case GRBL_ALARM_PROBE_FAIL_CONTACT: return "Probe Fail Contact";
		case GRBL_ALARM_HOMING_FAIL_RESET: return "Homing Fail Reset";
		case GRBL_ALARM_HOMING_FAIL_DOOR: return "Homing Fail Door";
		case GRBL_ALARM_HOMING_FAIL_PULLOFF: return "Homing Fail Pulloff";
		case GRBL_ALARM_HOMING_FAIL_APPROACH: return "Homing Fail Approach";

	}
	return "Invalid alarm code";
}

static std::string getGrblMessageCodeString(int code)
{
	switch(code)
	{
		case GRBL_MESSAGE_CRITICAL_EVENT: return "Critical Event";
		case GRBL_MESSAGE_ALARM_LOCK: return "Alarm Lock";
		case GRBL_MESSAGE_ALARM_UNLOCK: return "Alarm Unlock";
		case GRBL_MESSAGE_ENABLED: return "Enabled";
		case GRBL_MESSAGE_DISABLED: return "Disabled";
		case GRBL_MESSAGE_SAFETY_DOOR_AJAR: return "Safety Door Ajar";
		case GRBL_MESSAGE_CHECK_LIMITS: return "Check Limits";
		case GRBL_MESSAGE_PROGRAM_END: return "Program End";
		case GRBL_MESSAGE_RESTORE_DEFAULTS: return "Restore Defaults";
		case GRBL_MESSAGE_SPINDLE_RESTORE: return "Spindle Restore";
		case GRBL_MESSAGE_SLEEP_MODE: return "Sleep Mode";
	}
	return "Invalid message code";
}

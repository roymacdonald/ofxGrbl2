//
//  ofxGrblConstants.h
//  example-SimpleGrbl
//
//  Created by Roy Macdonald on 7/16/19.
//
//

#pragma once

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

#pragma once
#include "enum.h"

//#include "enum.h"

// Configuration for input from receiver
struct InputConfig
{
	int ppmInput;
	int totalChannels;
	unsigned long ppmChannelValueMaxError;
	unsigned long ppmBlankTime;
	unsigned int minChannelValue;
	unsigned int maxChannelValue;
	int modePin;
};

// Configuration for servo
struct ServoConfig
{
	int pin;
	int center;
	int min;
	int max;

	bool operator==(const ServoConfig& rhs)
	{
		return
			pin == rhs.pin &&
			center == rhs.center &&
			min == rhs.min &&
			max == rhs.max;
	}
};


/// <summary>
/// Configures an analog channel (e.g. ESC, steering) 
/// </summary>
struct AnalogChannel
{
	int channel;
	int min;
	int max;
	int dbMin;
	int dbMax;

	bool operator==(const AnalogChannel& rhs)
	{
		return
			channel == rhs.channel &&
			min == rhs.min &&
			max == rhs.max &&
			dbMin == rhs.dbMin &&
			dbMax == rhs.dbMax;
	}
};

/// <summary>
/// A special analog channel for throttle, with an e-brake threshold
/// </summary>
struct ThrottleChannel : AnalogChannel
{
	ThrottleChannel() {}
	ThrottleChannel(int channel, int min, int max, int dbMin, int dbMax, int eBrakeThreshold) : AnalogChannel{ channel, min, max, dbMin, dbMax }, eBrakeThreshold{ eBrakeThreshold } {}

	int eBrakeThreshold;
};

/// <summary>
/// Configures a latching switch, e.g. pull back on right stick to select forward / reverse
/// </summary>
struct LatchChannel
{
	int channel;
	int min;
	int max;
};

/// <summary>
/// Configures a 2-way switch, e.g. a toggle on the transmitter to turn a function on and off
/// </summary>
struct SwitchChannelTwoWay
{
	int channel;
	int high;
};

/// <summary>
/// Configures a 3-way switch, e.g. a toggle on the transmitter to select a mode or function
/// </summary>
typedef struct SwitchChannelThreeWay
{
	int channel;
	int high;
	int low;
};

// Configuration for lighting modes
struct LightModeConfig {
	int brakeIntensityMax;
	int brakeIntensityLow;
	int reverseIntensity;
	int headlightIntensityMax;
	int headlightIntensityLow;
	int rooflightIntensityMax;
	int rooflightIntensityLow;
	int floodlightIntensity;
};

// Configuration for Control methods
struct ControlConfig
{
	int maxFailsafeCount;
	LatchChannel gearChannel;
	LatchChannel cruiseChannel;
	ThrottleChannel throttleChannel;
	AnalogChannel steeringChannel;
	AnalogChannel winchOperationChannel;
	LatchChannel lightsOnChannel;
	LatchChannel lightsOffChannel;
	SwitchChannelTwoWay floodlightChannel;
	SwitchChannelThreeWay winchSelectChannel;
	ServoConfig throttleServo;
	ServoConfig steeringServo;
	ServoConfig winch1Servo;
	ServoConfig winch2Servo;
	LightModeConfig lightModeConfig;
	int fwdAccelInertia; // tODO: this could be neater - we could build the inertias here like we do with servos?
	int fwdDecelInertia;
	int fwdBrakeInertia;
	int revAccelInertia;
	int revDecelInertia;
	int revBrakeInertia;
	int cruiseInertia;
	int steeringInertia;
	int aux1Channel;
	int aux1Center;
};

// Configuration for lighting output
struct LightOutputConfig {
	int headLightPin;
	int brakePin;
	int reversePin;
	int roofLightPin;
	int floodlightPin;
	int dashLightPinR;
	int dashLightPinG;
	int dashLightPinB;
};

// Channel input values
struct InputSetting
{
	int channel[8];

	Mode mode;

	bool operator==(const InputSetting& rhs)
	{
		return
			channel[0] == rhs.channel[0] &&
			channel[1] == rhs.channel[1] &&
			channel[2] == rhs.channel[2] &&
			channel[3] == rhs.channel[3] &&
			channel[4] == rhs.channel[4] &&
			channel[5] == rhs.channel[5] &&
			channel[6] == rhs.channel[6] &&
			channel[7] == rhs.channel[7];
	}
};

/// <summary>
/// Dash light control values
/// </summary>
struct DashLightSetting
{
	int redIntensity;
	int greenIntensity;
	int blueIntensity;

	bool operator==(const DashLightSetting& rhs)
	{
		return
			redIntensity == rhs.redIntensity &&
			greenIntensity == rhs.greenIntensity &&
			blueIntensity == rhs.blueIntensity;
	}
};

// Light control values
struct LightSetting {
	int brakeIntensity;
	int reverseIntensity;
	int headLightIntensity;
	int roofLightIntensity;
	int floodlightIntensity;
	DashLightSetting dashLightSetting;

	bool operator==(const LightSetting& rhs)
	{
		return
			brakeIntensity == rhs.brakeIntensity &&
			reverseIntensity == rhs.reverseIntensity &&
			headLightIntensity == rhs.headLightIntensity &&
			roofLightIntensity == rhs.roofLightIntensity &&
			floodlightIntensity == rhs.floodlightIntensity &&
			dashLightSetting == rhs.dashLightSetting;
	}
};


// Converted control values
struct ControlSetting
{
	int motorSpeed;
	int steering;
	int aux1;
	int winch1;
	int winch2;
	Gear gear;
	Cruise cruise;
	LightSetting lightSetting;
	bool useInertia; // TODO: consider using an enum (Inertia = on, off)
};

// Winch settings
typedef struct WinchSetting
{
	int winch1;
	int winch2;
};
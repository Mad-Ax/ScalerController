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

// Configuration for lighting modes
struct LightModeConfig {
	int brakeIntensityMax;
	int brakeIntensityLow;
	int reverseIntensity;
	int headlightIntensityMax;
	int headlightIntensityLow;
	int rooflightIntensityMax;
	int rooflightIntensityLow;
//	int failsafeFlashDelay;
};

// Configuration for Control methods
struct ControlConfig
{
	int maxFailsafeCount;
	LatchChannel gearChannel;
	LatchChannel cruiseChannel;
	ThrottleChannel throttleChannel;
	AnalogChannel steeringChannel;
	LatchChannel lightsOnChannel;
	LatchChannel lightsOffChannel;
	ServoConfig throttleServo;
	ServoConfig steeringServo;
	LightModeConfig lightModeConfig;
//	int switchHigh;
//	int switchLow;
	int fwdAccelInertia;
	int fwdDecelInertia;
	int fwdBrakeInertia;
	int revAccelInertia;
	int revDecelInertia;
	int revBrakeInertia;
	int eBrakeThreshold;
};

// Configuration for lighting output
struct LightOutputConfig {
	int headLightPin;
	int brakePin;
	int reversePin;
	int roofLightPin;
//	int failsafePin;
};

// Channel input values
struct InputSetting
{
	int channel[8];

	int mode;

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

// Light control values
struct LightSetting {
	int brakeIntensity;
	int reverseIntensity;
	int headLightIntensity;
	int roofLightIntensity;

	bool operator==(const LightSetting& rhs)
	{
		return
			brakeIntensity == rhs.brakeIntensity &&
			reverseIntensity == rhs.reverseIntensity &&
			headLightIntensity == rhs.headLightIntensity &&
			roofLightIntensity == rhs.roofLightIntensity;
	}
};

// Converted control values
struct ControlSetting
{
	int motorSpeed;
	int steering;
	Gear gear;
	Cruise cruise;
	LightSetting lightSetting;
};

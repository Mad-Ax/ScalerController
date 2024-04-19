#pragma once
#include "enum.h"

// Configuration for servo
typedef struct ServoConfig
{
	// TODO: do we need to store center and EPs for all servos, or should this be a single static setting for all servos?
	const int& pin;
	const int& center;
	const int& minimum;
	const int& maximum;

	ServoConfig(
		const int& pin,
		const int& center,
		const int& minimum,
		const int& maximum) :
		pin(pin),
		center(center),
		minimum(minimum),
		maximum(maximum)
	{
	}

	bool operator==(const ServoConfig& rhs)
	{
		return
			pin == rhs.pin &&
			center == rhs.center &&
			minimum == rhs.minimum &&
			maximum == rhs.maximum;
	}
};


/// <summary>
/// Configures an analog channel (e.g. ESC, steering) 
/// </summary>
typedef struct AnalogChannel
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
typedef struct ThrottleChannel : AnalogChannel
{
	ThrottleChannel() {}
	ThrottleChannel(int channel, int min, int max, int dbMin, int dbMax, int eBrakeThreshold) : AnalogChannel{ channel, min, max, dbMin, dbMax }, eBrakeThreshold{ eBrakeThreshold } {}

	int eBrakeThreshold;
};

/// <summary>
/// Configures a latching switch, e.g. pull back on right stick to select forward / reverse
/// </summary>
typedef struct LatchChannel
{
	int channel;
	int min;
	int max;
};

/// <summary>
/// Configures a 2-way switch, e.g. a toggle on the transmitter to turn a function on and off
/// </summary>
typedef struct SwitchChannelTwoWay
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
typedef struct LightModeConfig
{
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
typedef struct ControlConfig
{
	int maxFailsafeCount;
	LatchChannel& gearChannel;
	LatchChannel& cruiseChannel;
	ThrottleChannel& throttleChannel;
	AnalogChannel& steeringChannel;
	AnalogChannel& winchOperationChannel;
	LatchChannel& lightsOnChannel;
	LatchChannel& lightsOffChannel;
	SwitchChannelTwoWay& floodlightChannel;
	SwitchChannelThreeWay& winchSelectChannel;
	const ServoConfig& throttleServo;
	const ServoConfig& steeringServo;
	const ServoConfig& winch1Servo;
	const ServoConfig& winch2Servo;
	LightModeConfig& lightModeConfig;

	ControlConfig(
		int maxFailsafeCount,
		LatchChannel& gearChannel,
		LatchChannel& cruiseChannel,
		ThrottleChannel& throttleChannel,
		AnalogChannel& steeringChannel,
		AnalogChannel& winchOperationChannel,
		LatchChannel& lightsOnChannel,
		LatchChannel& lightsOffChannel,
		SwitchChannelTwoWay& floodlightChannel,
		SwitchChannelThreeWay& winchSelectChannel,
		const ServoConfig& throttleServo,
		const ServoConfig& steeringServo,
		const ServoConfig& winch1Servo,
		const ServoConfig& winch2Servo,
		LightModeConfig& lightModeConfig) :
		maxFailsafeCount(maxFailsafeCount),
		gearChannel(gearChannel),
		cruiseChannel(cruiseChannel),
		throttleChannel(throttleChannel),
		steeringChannel(steeringChannel),
		winchOperationChannel(winchOperationChannel),
		lightsOnChannel(lightsOnChannel),
		lightsOffChannel(lightsOffChannel),
		floodlightChannel(floodlightChannel),
		winchSelectChannel(winchSelectChannel),
		throttleServo(throttleServo),
		steeringServo(steeringServo),
		winch1Servo(winch1Servo),
		winch2Servo(winch2Servo),
		lightModeConfig(lightModeConfig) {}
};

// Configuration for lighting output
typedef struct LightOutputConfig {
	int headLightPin;
	int brakePin;
	int reversePin;
	int roofLightPin;
	int floodlightPin;
//	int failsafePin;
};

// Channel input values
typedef struct InputSetting
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

// Light control values
typedef struct LightSetting {
	int brakeIntensity;
	int reverseIntensity;
	int headLightIntensity;
	int roofLightIntensity;
	int floodlightIntensity;

	bool operator==(const LightSetting& rhs)
	{
		return
			brakeIntensity == rhs.brakeIntensity &&
			reverseIntensity == rhs.reverseIntensity &&
			headLightIntensity == rhs.headLightIntensity &&
			roofLightIntensity == rhs.roofLightIntensity &&
			floodlightIntensity == rhs.floodlightIntensity;
	}
};

// Converted control values
typedef struct ControlSetting
{
	int motorSpeed;
	int steering;
	int winch1;
	int winch2;
	Gear gear;
	Cruise cruise;
	LightSetting lightSetting;
};

// Winch settings
typedef struct WinchSetting
{
	int winch1;
	int winch2;
};


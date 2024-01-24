#pragma once

//#include "enum.h"

// Configuration for input from receiver
struct InputConfig {
	int ppmInput;
	unsigned long ppmChannelValueMaxError;
	unsigned long ppmBlankTime;
};

// Configuration for servo
struct ServoConfig {
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

struct ChannelConfig {
	int channel;
	int min;
	int max;
	int dbMin;
	int dbMax;

	bool operator==(const ChannelConfig& rhs)
	{
		return
			channel == rhs.channel &&
			min == rhs.min &&
			max == rhs.max &&
			dbMin == rhs.dbMin &&
			dbMax == rhs.dbMax;
	}
};

//// Configuration for lighting modes
//struct LightModeConfig {
//	bool lights;
//	int indicatorTime;
//	int brakeMax;
//	int brakeMed;
//	int headlightMax;
//	int headlightMed;
//	int headlightMin;
//	int failsafeFlashDelay;
//};

// Configuration for Control methods
struct ControlConfig {
	int maxFailsafeCount;
//	ChannelConfig mode;
//	ChannelConfig gear;
	ChannelConfig throttleChannel;
	ChannelConfig steeringChannel;
//	ChannelConfig indicator;
//	ChannelConfig lights;
	ServoConfig throttleServo;
	ServoConfig steeringServo;
//	LightModeConfig lightModeConfig;
//	int switchHigh;
//	int switchLow;
//	int fwdAccelInertia;
//	int fwdDecelInertia;
//	int fwdBrakeInertia;
//	int revAccelInertia;
//	int revDecelInertia;
//	int revBrakeInertia;
//	int eBrakeThreshold;
//	//TODO: Option for 4 aux servos?
};

//// Configuration for lighting output
//struct LightOutputConfig {
//	int indicatorLeftPin;
//	int indicatorRightPin;
//	int brakeLightPin;
//	int headLightPin;
//	int fogLightFrontPin;
//	int fogLightRearPin;
//	int roofLightPin;
//	int reverseLightPin;
//	int failsafePin;
//};

// Channel input values
struct InputSetting {
	int channel[8];

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

//// Light control values
//struct LightSetting {
//	int indicatorLeftIntensity;
//	int indicatorRightIntensity;
//	int brakeLightIntensity;
//	int headLightIntensity;
//	int frontFogIntensity;
//	int rearFogIntensity;
//	int roofLightIntensity;
//	int reverseLightIntensity;
//
//	bool operator==(const LightSetting& rhs)
//	{
//		return
//			indicatorLeftIntensity == rhs.indicatorLeftIntensity &&
//			indicatorRightIntensity == rhs.indicatorRightIntensity &&
//			brakeLightIntensity == rhs.brakeLightIntensity &&
//			headLightIntensity == rhs.headLightIntensity &&
//			frontFogIntensity == rhs.frontFogIntensity &&
//			rearFogIntensity == rhs.rearFogIntensity &&
//			roofLightIntensity == rhs.roofLightIntensity &&
//			reverseLightIntensity == rhs.reverseLightIntensity;
//	}
//};

// Converted control values
struct ControlSetting {
	int motorSpeed;
	int steering;
//	LightSetting lightSetting;
};

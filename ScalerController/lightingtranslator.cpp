#include "lightingtranslator.h"

//LightingTranslator::LightingTranslator(IInputTranslator* translator, ControlConfig config)
LightingTranslator::LightingTranslator(
	ControlConfig config,
	ILatchTranslator* lightsOnTranslator,
	ILatchTranslator* lightsOffTranslator,
	ISwitchTranslatorTwoWay* floodlightTranslator)
{
	this->config = config;
	this->lightsOnTranslator = lightsOnTranslator;
	this->lightsOffTranslator = lightsOffTranslator;
	this->floodlightTranslator = floodlightTranslator;
	this->currentLightMode = LightMode::Off;
}

LightingTranslator::~LightingTranslator()
{
}

// Translates the light settings based on current input
LightSetting LightingTranslator::translateLightSetting(InputSetting input, Gear gear, Cruise cruise, bool useInertia, HardwareSerial& ser)
{
	// Check if we're incrementing the lights on situation
	LatchChannel onChannel = this->config.lightsOnChannel;
	int onValue = input.channel[onChannel.channel];

	if (this->lightsOnTranslator->translateLatch(onChannel, onValue))
	{
		// We have requested to increment the light mode
		switch (currentLightMode)
		{
		case LightMode::Off:
			currentLightMode = LightMode::Dipped;
			break;

		case LightMode::Dipped:
			currentLightMode = LightMode::Rooflights;
			break;

		case LightMode::Rooflights:
			currentLightMode = LightMode::Full;
			break;
		}
	}

	// Check if we're decrementing the lights on situation
	LatchChannel offChannel = this->config.lightsOffChannel;
	int offValue = input.channel[offChannel.channel];

	if (this->lightsOffTranslator->translateLatch(offChannel, offValue))
	{
		// We have requested to increment the light mode
		switch (currentLightMode)
		{
		case LightMode::Full:
			currentLightMode = LightMode::Rooflights;
			break;

		case LightMode::Rooflights:
			currentLightMode = LightMode::Dipped;
			break;

		case LightMode::Dipped:
			currentLightMode = LightMode::Off;
			break;
		}
	}

	// Create our new light setting and set the intensities according to the mode
	LightSetting newSetting;
	switch (currentLightMode)
	{
	case LightMode::Off:
		newSetting.headLightIntensity = 0;
		newSetting.roofLightIntensity = 0;
		break;

	case LightMode::Dipped:
		newSetting.headLightIntensity = this->config.lightModeConfig.headlightIntensityLow;
		newSetting.roofLightIntensity = 0;
		break;

	case LightMode::Rooflights:
		newSetting.headLightIntensity = this->config.lightModeConfig.headlightIntensityLow;
		newSetting.roofLightIntensity = this->config.lightModeConfig.rooflightIntensityLow;
		break;

	case LightMode::Full:
		newSetting.headLightIntensity = this->config.lightModeConfig.headlightIntensityMax;
		newSetting.roofLightIntensity = this->config.lightModeConfig.rooflightIntensityMax;
		break;
	}
	
	// Set the brake light intensity
	newSetting.brakeIntensity = this->translateBrakeLight(input, currentLightMode);

	// Set the reverse light intensity
	newSetting.reverseIntensity = this->translateReverseLight(gear);

	// Set the floodlight intensity
	newSetting.floodlightIntensity = this->translateFloodlight(input);

	// Set the dash light intensity
	newSetting.dashLightSetting = this->translateDashLight(cruise, useInertia);
	
	return newSetting;
}

// Translates the brake light setting based on throttle input and current lighting mode
int LightingTranslator::translateBrakeLight(InputSetting input, LightMode lightMode)
{
	ThrottleChannel channel = config.throttleChannel;
	int inputVal = input.channel[channel.channel];

	if (inputVal < config.throttleChannel.dbMin)
	{
		// We are braking - return the high intensity value for the brake light
		return config.lightModeConfig.brakeIntensityMax;
	}

	// We are not braking

	if (lightMode == LightMode::Off)
	{
		// Lights are off - return 0 signal
		return 0;
	}

	// Lights are on - return low intensity tail lights
	return this->config.lightModeConfig.brakeIntensityLow;
}

// Translates the reverse light setting based on gear setting
int LightingTranslator::translateReverseLight(Gear gear)
{
	switch (gear)
	{
	case Gear::Forward:
		return 0;

	case Gear::Reverse:
		return config.lightModeConfig.reverseIntensity;
	}

	return 0;
}

// Translates the floodlight setting based on switch setting
int LightingTranslator::translateFloodlight(InputSetting input)
{
	// Check if the floodlight channel is on
	SwitchChannelTwoWay floodlightChannel = this->config.floodlightChannel;
	int switchValue = input.channel[floodlightChannel.channel];
	if (this->floodlightTranslator->translateSwitch(floodlightChannel, switchValue))
	{
		return config.lightModeConfig.floodlightIntensity;
	}

	return 0;
}

// Translates the dash light setting based on inertia, cruise, and failsafe
DashLightSetting LightingTranslator::translateDashLight(Cruise cruise, bool useInertia)
{
	if (cruise == Cruise::On)
	{
		return this->config.lightModeConfig.cruiseDashLight;
	}

	if (useInertia)
	{
		return this->config.lightModeConfig.inertiaDashLight;
	}

	return this->config.lightModeConfig.directDashLight;
}
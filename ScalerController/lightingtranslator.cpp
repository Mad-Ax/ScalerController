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

//// Translates the indicator input to the desired output depending on mode
//Indicator LightingTranslator::translateIndicator(int input)
//{
//	return static_cast<Indicator>(translator->translateThreeWaySwitch(input, config.switchHigh, config.switchLow));
//}
//
//// Translate the light mode based on the switch setting
//LightMode LightingTranslator::translateLightMode(int input, LightMode currentLightMode)
//{
//	// if for some bizarre reason we've ended up with a Max setting, reset it to Off
//	if (currentLightMode == LightMode::Max) currentLightMode = LightMode::Off;
//
//	// get the current switch position
//	ThreeWayPosition sw = translator->translateThreeWaySwitch(input, config.switchHigh, config.switchLow);
//
//	if (sw == ThreeWayPosition::PosB)
//	{
//		// switch is neutral - no change.  Unset the latch if it was set.
//		lightModeLatch = false;
//		return currentLightMode;
//	}
//
//	// switch is in the 'light mode change' position - user has requested to change the 
//	// light setting.  Do not change the mode if the latch has been set so we don't cycle
//	// modes quickly.
//	if (lightModeLatch)
//		return currentLightMode;
//
//	// latch has not been set.  We set the latch now so that we don't cycle modes if the 
//	// user keeps the switch in position for more than a single program loop.
//	lightModeLatch = true;
//	int newSetting = static_cast<int>(currentLightMode) + 1;
//	if (newSetting == static_cast<int>(LightMode::Max))
//		newSetting = 0;
//
//	return static_cast<LightMode>(newSetting);
//}
//
//// Translates the flash lamps setting based on lightmode and input
//MainBeam LightingTranslator::translateMainBeam(int input, LightMode currentLightMode, MainBeam currentMainBeam)
//{
//	// get the current switch position
//	ThreeWayPosition sw = translator->translateThreeWaySwitch(input, config.switchHigh, config.switchLow);
//
//	if (sw == ThreeWayPosition::PosA)
//	{
//		// If lights are off, flash the main beams
//		if (currentLightMode == LightMode::Off || currentLightMode == LightMode::Sidelights)
//			return MainBeam::Flash;
//
//		// Switch is in the 'main beam' position - user has requested to change the light
//		// setting.  Do not change the mode if the latch has been set so we don't cycle 
//		// modes quickly.
//		if (mainBeamLatch)
//			return currentMainBeam;
//		
//		// No latch is set - set the latch to prevent rapid mode cycling
//		mainBeamLatch = true;
//
//		// Otherwise, turn main beams on or off depending on current setting
//		if (currentMainBeam == MainBeam::On)
//			return MainBeam::Off;
//
//		return MainBeam::On;
//	}
//
//	// stick is not in toggle position - no need to set main beam on or off.
//	// Unset the latch if it was set.
//	mainBeamLatch = false;
//
//	// If headlights not on, turn main beams off
//	if (currentLightMode == LightMode::Off || currentLightMode == LightMode::Sidelights)
//		return MainBeam::Off;
//
//	// If lights are on, return whatever the current setting is
//	return currentMainBeam;
//}

// Translates the light settings based on current input
LightSetting LightingTranslator::translateLightSetting(InputSetting input, Gear gear)
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
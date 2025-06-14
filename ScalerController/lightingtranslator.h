#pragma once

#include "ilightingtranslator.h"
#include "ilatchtranslator.h"
#include "iswitchtranslatortwoway.h"

class LightingTranslator : public ILightingTranslator
{
public:
	LightingTranslator(
		ControlConfig config,
		ILatchTranslator* lightsOnTranslator,
		ILatchTranslator* lightsOffTranslator,
		ISwitchTranslatorTwoWay* floodlightTranslator);
	~LightingTranslator();

	/// <summary>
	/// Translates the light settings based on current input
	/// </summary>
	/// <param name="input">Raw channel input data from the receiver</param>
	/// <param name="gear">Current gear</param>
	/// <param name="driveMode">Current drive mode</param>
	/// <returns>The light setting</returns>
	LightSetting translateLightSetting(InputSetting input, Gear gear, DriveMode driveMode, HardwareSerial& ser);

private:
	ControlConfig config;
	ILatchTranslator* lightsOnTranslator;
	ILatchTranslator* lightsOffTranslator;
	ISwitchTranslatorTwoWay* floodlightTranslator;
	LightMode currentLightMode;

	// Translates the brake light setting based on throttle input and current lighting mode
	int translateBrakeLight(InputSetting input, LightMode lightMode);

	// Translates the reverse light setting based on gear setting
	int translateReverseLight(Gear gear);

	// Translates the floodlight setting based on switch setting
	int translateFloodlight(InputSetting input);

	// Translates the dash light setting based on gear and drive mode
	DashLightSetting translateDashLight(DriveMode driveMode);
};
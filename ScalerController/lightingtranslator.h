#pragma once

#include "ilightingtranslator.h"
#include "ilatchtranslator.h"

class LightingTranslator : public ILightingTranslator
{
public:
	LightingTranslator(
		ControlConfig config,
		ILatchTranslator* lightsOnTranslator,
		ILatchTranslator* lightsOffTranslator);
//	LightingTranslator(IInputTranslator* inputTranslator, ControlConfig config);
	~LightingTranslator();
//
//	// Translates the indicator input to the desired output depending on mode
//	Indicator translateIndicator(int input);
//
//	// Translate the light mode based on the switch setting
//	LightMode translateLightMode(int input, LightMode currentLightMode);
//
//	// Translates the flash lamps setting based on lightmode and input
//	MainBeam translateMainBeam(int input, LightMode currentLightMode, MainBeam currentMainBeam);

	/// <summary>
	/// Translates the light settings based on current input
	/// </summary>
	/// <param name="input">Raw channel input data from the receiver</param>
	/// <param name="gear">Current gear</param>
	/// <returns></returns>
	LightSetting translateLightSetting(InputSetting input, Gear gear);

private:
	ControlConfig config;
	ILatchTranslator* lightsOnTranslator;
	ILatchTranslator* lightsOffTranslator;
	LightMode currentLightMode;
//	IInputTranslator* translator;
//	bool lightModeLatch = false;
//	bool mainBeamLatch = false;

	// Translates the brake light setting based on throttle input and current lighting mode
	int translateBrakeLight(InputSetting input, LightMode lightMode);

	// Translates the reverse light setting based on gear setting
	int translateReverseLight(Gear gear);
};
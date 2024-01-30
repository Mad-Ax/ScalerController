#pragma once

#include "ilightingtranslator.h"
//#include "iinputtranslator.h"

class LightingTranslator : public ILightingTranslator
{
public:
	LightingTranslator(ControlConfig config);
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
//
	// Translates the brake light setting based on throttle input and current lighting mode
	int translateBrakeLight(InputSetting input);
//	BrakeLight translateBrakeLight(int input, LightMode currentLightMode);

	// Translates the reverse light setting based on gear setting
	int translateReverseLight(Gear gear);

private:
	ControlConfig config;
//	IInputTranslator* translator;
//	bool lightModeLatch = false;
//	bool mainBeamLatch = false;
};
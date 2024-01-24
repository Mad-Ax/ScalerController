//#pragma once
//
//#include "enum.h"
//
//class ILightingTranslator
//{
//public:
//	// Translates the indicator input to the desired output depending on mode
//	virtual	Indicator translateIndicator(int input) = 0;
//
//	// Translate the light mode based on the switch setting
//	virtual LightMode translateLightMode(int input, LightMode currentLightMode) = 0;
//
//	// Translates the main beam setting based on lightmode and input
//	virtual MainBeam translateMainBeam(int input, LightMode currentLightMode, MainBeam currentMainBeam) = 0;
//
//	// Translates the brake light setting based on throttle input and current lighting mode
//	virtual BrakeLight translateBrakeLight(int input, LightMode currentLightMode) = 0;
//
//	// Translates the reverse light setting based on gear setting
//	virtual bool translateReverseLight(Gear gear) = 0;
//};
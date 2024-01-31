#pragma once

#include "structs.h"

class ILightingTranslator
{
public:
//	// Translates the indicator input to the desired output depending on mode
//	virtual	Indicator translateIndicator(int input) = 0;
//
//	// Translate the light mode based on the switch setting
//	virtual LightMode translateLightMode(int input, LightMode currentLightMode) = 0;
//
//	// Translates the main beam setting based on lightmode and input
//	virtual MainBeam translateMainBeam(int input, LightMode currentLightMode, MainBeam currentMainBeam) = 0;

	/// <summary>
	/// Translates the light settings based on current input
	/// </summary>
	/// <param name="input">Raw channel input data from the receiver</param>
	/// <param name="gear">Current gear</param>
	/// <returns></returns>
	virtual LightSetting translateLightSetting(InputSetting input, Gear gear) = 0;
};
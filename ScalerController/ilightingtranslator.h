#pragma once

#include "structs.h"

class ILightingTranslator
{
public:
	/// <summary>
	/// Translates the light settings based on current input
	/// </summary>
	/// <param name="input">Raw channel input data from the receiver</param>
	/// <param name="gear">Current gear</param>
	/// <param name="driveMode">Current drive mode</param>
	/// <returns>The light setting</returns>
	virtual LightSetting translateLightSetting(InputSetting input, Gear gear, DriveMode driveMode, HardwareSerial& ser) = 0;
};
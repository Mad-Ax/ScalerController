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
	/// <param name="cruise">Current cruise setting</param>
	/// <param name="useInertia">Current inertia setting</param>
	/// <returns></returns>
	virtual LightSetting translateLightSetting(InputSetting input, Gear gear, Cruise cruise, bool useInertia, HardwareSerial& ser) = 0;
};
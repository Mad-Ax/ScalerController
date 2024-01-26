#pragma once

#include "structs.h"

class IInputTranslator
{
public:
	// Translates a 3-way switch value into 3 possible output values
//	virtual ThreeWayPosition translateThreeWaySwitch(int input, int switchHigh, int switchLow) = 0;

	// Translates an analog stick input to a servo output
	virtual int translateStickInput(int input, AnalogChannel channel, ServoConfig servo) = 0;
};
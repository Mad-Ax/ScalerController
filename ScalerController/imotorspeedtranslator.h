#pragma once

#include "iinertia.h"

class IMotorSpeedTranslator
{
public:
	// Translates the input into a motor speed
	virtual int translateMotorSpeed(int currentMotorSpeed, int input, int desiredMotorSpeed) const = 0;
};
#pragma once

#include "structs.h"
#include "enum.h"

class IControlTranslator
{
public:
	// Checks for a fail condition on the input
	// Returns true if the input values have failed to update for more than a set number of reads
	virtual bool checkFailsafe(InputSetting input) = 0;

	// Translates the channel input value to the requested gear
	virtual Gear translateGear(InputSetting input, Gear lastGear) = 0;

	// Translates the motor speed
	virtual int translateMotorSpeed(InputSetting input, Gear gear) = 0;

	// Translates the steering angle
	virtual int translateSteering(InputSetting input) = 0;
};
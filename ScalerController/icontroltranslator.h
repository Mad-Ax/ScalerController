#pragma once

#include "structs.h"

class IControlTranslator
{
public:
	// Checks for a fail condition on the input
	// Returns true if the input values have failed to update for more than a set number of reads
	virtual bool checkFailsafe(InputSetting input) = 0;

//	// Translates the requested mode
//	virtual Mode translateMode(int input) = 0;
//
//	// Translates the requested gear
//	virtual Gear translateGear(int input) = 0;
//
//	// Translates the motor speed
//	virtual int translateMotorSpeed(int currentMotorSpeed, int input, Gear gear) = 0;
//
//	// Translates the steering angle
//	virtual int translateSteering(int input) = 0;
};
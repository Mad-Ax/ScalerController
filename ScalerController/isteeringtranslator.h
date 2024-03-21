#pragma once

#include "iinertia.h"
#include "HardwareSerial.h"

class ISteeringTranslator
{
public:
	// Translates the input into a steering position
	virtual int translateSteering(int currentSteering, int desiredSteering, HardwareSerial& ser) const = 0;
};
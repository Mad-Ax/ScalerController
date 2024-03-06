#pragma once

#include "iinertia.h"

class ISteeringTranslator
{
public:
	// Translates the input into a steering position
	virtual int translateSteering(int currentSteering, int desiredSteering, IInertia* inertia) = 0;
};
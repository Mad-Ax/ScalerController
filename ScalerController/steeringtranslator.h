#pragma once

#include "isteeringtranslator.h"
#include "structs.h"

class SteeringTranslator : public ISteeringTranslator
{
public:
	SteeringTranslator(AnalogChannel channel, ServoConfig servo);
	~SteeringTranslator();

	// Translates the input into a steering position
	int translateDriveSteering(int currentSteering, int desiredSteering, IInertia* inertia);

private:
	AnalogChannel channel;
	ServoConfig servo;
};
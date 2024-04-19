#pragma once

#include "isteeringtranslator.h"
#include "structs.h"
#include "HardwareSerial.h"

class SteeringTranslator : public ISteeringTranslator
{
public:
	SteeringTranslator(const IInertia*& inertia);
	~SteeringTranslator();

	// Translates the input into a steering position
	int translateSteering(int currentSteering, int desiredSteering, HardwareSerial& ser) const;

private:
	const IInertia*& inertia;
};
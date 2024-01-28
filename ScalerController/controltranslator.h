#pragma once

//#include "Arduino.h"
#include "icontroltranslator.h"
#include "iinputtranslator.h"
#include "imotorspeedtranslator.h"
#include "iinertia.h"

class ControlTranslator : public IControlTranslator
{
public:
	ControlTranslator(
		ControlConfig config,
		IInputTranslator* inputTranslator,
		IMotorSpeedTranslator* motorSpeedTranslator,
		IInertia* forwardAccel,
		IInertia* forwardDecel,
		IInertia* forwardBrake,
		IInertia* reverseAccel,
		IInertia* reverseDecel,
		IInertia* reverseBrake);
	~ControlTranslator();

	// Checks for a fail condition on the input
	// Return true if the input values have failed to update for more than a set number of reads
	bool checkFailsafe(InputSetting input);

	// Translates the channel input value to the requested gear
	Gear translateGear(InputSetting input, Gear lastGear);

	// Translates the motor speed
	int translateMotorSpeed(InputSetting input, Gear gear, int currentMotorSpeed, HardwareSerial& ser);

	// Translates the steering angle
	int translateSteering(InputSetting input);

private:
	int failsafeCount;
	InputSetting *lastInput = nullptr;
	ControlConfig config;
	bool gearLatching;
	IInputTranslator* inputTranslator;
	IMotorSpeedTranslator* motorSpeedTranslator;
	IInertia* forwardAccel;
	IInertia* forwardDecel;
	IInertia* forwardBrake;
	IInertia* reverseAccel;
	IInertia* reverseDecel;
	IInertia* reverseBrake;
};

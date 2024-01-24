#pragma once

//#include "Arduino.h"
#include "icontroltranslator.h"
//#include "iinputtranslator.h"
//#include "imotorspeedtranslator.h"
//#include "iinertia.h"

class ControlTranslator : public IControlTranslator
{
public:
	ControlTranslator(
		ControlConfig config
//		IInputTranslator* inputTranslator, 
//		IMotorSpeedTranslator* motorSpeedTranslator,
//		IInertia* forwardAccel,
//		IInertia* forwardDecel,
//		IInertia* forwardBrake,
//		IInertia* reverseAccel,
//		IInertia* reverseDecel,
//		IInertia* reverseBrake);
);
	~ControlTranslator();

	// Checks for a fail condition on the input
	// Return true if the input values have failed to update for more than a set number of reads
	bool checkFailsafe(InputSetting input);
//
//	// Translates the requested mode
//	Mode translateMode(int input);
//
//	// Translates the requested gear
//	Gear translateGear(int input);
//
//	// Translates the motor speed
//	int translateMotorSpeed(int currentMotorSpeed, int input, Gear gear);
//
//	// Translates the steering angle
//	int translateSteering(int input);
//
private:
	int failsafeCount;
	InputSetting* lastInput = nullptr;
	ControlConfig config;
//	IInputTranslator* inputTranslator;
//	IMotorSpeedTranslator* motorSpeedTranslator;
//	IInertia* forwardAccel;
//	IInertia* forwardDecel;
//	IInertia* forwardBrake;
//	IInertia* reverseAccel;
//	IInertia* reverseDecel;
//	IInertia* reverseBrake;
};

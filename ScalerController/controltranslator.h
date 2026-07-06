#pragma once

//#include "Arduino.h"
#include "icontroltranslator.h"
#include "iinputtranslator.h" // TODO: M: remove
#include "isteeringtranslator.h"
#include "imotorspeedtranslator.h"
#include "ilatchtranslator.h"
#include "iswitchtranslatorthreeway.h"
#include "iinertia.h"

class ControlTranslator : public IControlTranslator
{
public:
	ControlTranslator(
		ControlConfig config,
		IInputTranslator* inputTranslator,
		ISteeringTranslator* steeringTranslator,
		IMotorSpeedTranslator* motorSpeedTranslator,
		ILatchTranslator* gearTranslator,
		ILatchTranslator* driveModeTranslator,
		IInertia* forwardAccel,
		IInertia* forwardDecel,
		IInertia* forwardBrake,
		IInertia* reverseAccel,
		IInertia* reverseDecel,
		IInertia* reverseBrake,
		IInertia* cruiseInertia,
		IInertia* steeringInertia,
		ISwitchTranslatorThreeWay* winchSelectTranslator);
	~ControlTranslator();

	// Checks for a fail condition on the input
	// Return true if the input values have failed to update for more than a set number of reads
	bool checkFailsafe(InputSetting input);

	// Translates the channel input value to the requested gear
	Gear translateGear(InputSetting input, Gear currentGear);

	// Translates the channel input value to the requested drive mode
	DriveMode translateDriveMode(InputSetting input, Gear currentGear, DriveMode currentDriveMode);

	// Translates the motor speed in crawl mode
	int ControlTranslator::translateCrawlSpeed(InputSetting input);

	// Translates the motor speed in drive mode
	int translateMotorSpeed(InputSetting input, Gear gear, int currentMotorSpeed, HardwareSerial& ser);

	// Translates the motor speed in cruise mode
	int translateCruiseSpeed(InputSetting input, Gear gear, int currentMotorSpeed, HardwareSerial& ser);

	// Translates the steeering angle in crawl mode
	int translateCrawlSteering(InputSetting input);

	// Translates the steering angle in drive and cruise mode
	int translateDriveSteering(InputSetting input, int currentSteering, HardwareSerial& ser);

	// Translates the winch operation settings
	WinchSetting translateWinch(InputSetting input) const;

private:
	int failsafeCount;
	InputSetting *lastInput = nullptr;
	ControlConfig config;
	IInputTranslator* inputTranslator; // TODO: M: remove if not used
	ISteeringTranslator* steeringTranslator;
	IMotorSpeedTranslator* motorSpeedTranslator;
	ILatchTranslator* gearTranslator;
	ILatchTranslator* driveModeTranslator;
	IInertia* forwardAccel;
	IInertia* forwardDecel;
	IInertia* forwardBrake;
	IInertia* reverseAccel;
	IInertia* reverseDecel;
	IInertia* reverseBrake;
	IInertia* cruiseInertia;
	IInertia* steeringInertia;
	ISwitchTranslatorThreeWay* winchSelectTranslator; // TODO: M: check feature/referencerefactor to remove all the inertias from here, if it doesn't reintroduce the bug..?
};

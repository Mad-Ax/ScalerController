#pragma once

//#include "Arduino.h"
#include "icontroltranslator.h"
#include "isteeringtranslator.h"
#include "imotorspeedtranslator.h"
#include "ilatchtranslator.h"
#include "iswitchtranslatorthreeway.h"

class ControlTranslator : public IControlTranslator
{
public:
	ControlTranslator(
		ControlConfig& config,
		const int& throttleServoCenter,
		const int& steeringServoPin,
		const int& steeringServoCenter,
		const int& steeringServoMin,
		const int& steeringServoMax,
		const ISteeringTranslator& steeringTranslator,
		const IMotorSpeedTranslator& forwardMotorSpeedTranslator,
		const IMotorSpeedTranslator& reverseMotorSpeedTranslator,
		ILatchTranslator& gearTranslator,
		ILatchTranslator& cruiseTranslator,
		const ISwitchTranslatorThreeWay& winchSelectTranslator);
	~ControlTranslator();

	// Checks for a fail condition on the input
	// Return true if the input values have failed to update for more than a set number of reads
	bool checkFailsafe(const InputSetting& input);

	// Translates the channel input value to the requested gear
	Gear translateGear(const InputSetting& input, Gear lastGear);

	// Translates the channel input value to the requested cruise value
	Cruise translateCruise(const InputSetting& input, Cruise lastCruise);

	// Translates the motor speed in drive mode
	int translateMotorSpeed(const InputSetting& input, Gear gear, int currentMotorSpeed, HardwareSerial& ser) const;

	// Translates the motor speed in cruise mode
	int translateCruiseSpeed(const InputSetting& input, Gear gear, int currentMotorSpeed, HardwareSerial& ser) const;

	// Translates the steering angle
	int translateSteering(const InputSetting& input, int currentSteering, HardwareSerial& ser) const;

	// Translates the winch operation settings
	WinchSetting translateWinch(const InputSetting& input) const;

private:
	int failsafeCount;
	InputSetting *lastInput = nullptr;
	ControlConfig& config;
	const ServoConfig& throttleServo;
	const ServoConfig& steeringServo;
	const ISteeringTranslator& steeringTranslator;
	const IMotorSpeedTranslator& forwardMotorSpeedTranslator;
	const IMotorSpeedTranslator& reverseMotorSpeedTranslator;
	ILatchTranslator& gearTranslator;
	ILatchTranslator& cruiseTranslator;
	const ISwitchTranslatorThreeWay& winchSelectTranslator;
};

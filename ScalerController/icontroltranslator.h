#pragma once

#include "structs.h"
#include "enum.h"
#include "HardwareSerial.h"

class IControlTranslator
{
public:
	// Checks for a fail condition on the input
	// Returns true if the input values have failed to update for more than a set number of reads
	virtual bool checkFailsafe(const InputSetting& input) = 0;

	// Translates the channel input value to the requested gear
	virtual Gear translateGear(const InputSetting& input, Gear lastGear) = 0;

	// Translates the channel input value to the requested cruise value
	virtual Cruise translateCruise(const InputSetting& input, Cruise lastCruise) = 0;

	/// <summary>
	/// Translates the motor speed in drive mode 
	/// </summary>
	/// <param name="input">Input values decoded from the transmitter</param>
	/// <param name="gear">Current gear</param>
	/// <param name="currentMotorSpeed">Current motor speed</param>
	/// <param name="ser">Serial (for debugging - remove in production)</param>
	/// <returns>An integer value to be sent to the ESC</returns>
	virtual int translateMotorSpeed(const InputSetting& input, Gear gear, int currentMotorSpeed, HardwareSerial& ser) const = 0;

	/// <summary>
	/// Translates the motor speed in cruise mode
	/// </summary>
	/// <param name="input">Input values decoded from the transmitter</param>
	/// <param name="gear">Current gear</param>
	/// <param name="currentMotorSpeed">Current motor speed</param>
	/// <param name="ser">Serial (for debugging - remove in production)</param>
	/// <returns>An integer value to be sent to the ESC</returns>
	virtual int translateCruiseSpeed(const InputSetting& input, Gear gear, int currentMotorSpeed, HardwareSerial& ser) const = 0;

	// Translates the steering angle
	virtual int translateSteering(const InputSetting& input, int currentSteering, HardwareSerial& ser) const = 0;

	// Translates the winch operation settings
	virtual WinchSetting translateWinch(const InputSetting& input) const = 0;
};
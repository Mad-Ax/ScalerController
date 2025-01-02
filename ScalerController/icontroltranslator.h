#pragma once

#include "structs.h"
#include "enum.h"
#include "HardwareSerial.h"

class IControlTranslator
{
public:
	// Checks for a fail condition on the input
	// Returns true if the input values have failed to update for more than a set number of reads
	virtual bool checkFailsafe(InputSetting input) = 0;

	// Translates the channel input value to the requested gear
	virtual Gear translateGear(InputSetting input, Gear lastGear) = 0;

	/// <summary>
	/// Translates the channel input value to the requested cruise value
	/// </summary>
	/// <param name="input">Input values decoded from the transmitter</param>
	/// <param name="lastCruise">Current cruise setting</param>
	/// <param name="currentGear">Currently selected gear</param>
	/// <returns>A Cruise enum representing the current cruise mode</returns>
	virtual Cruise translateCruise(InputSetting input, Cruise lastCruise, Gear currentGear) = 0;

	/// <summary>
	/// Translates the motor speed in drive mode 
	/// </summary>
	/// <param name="input">Input values decoded from the transmitter</param>
	/// <param name="gear">Current gear</param>
	/// <param name="currentMotorSpeed">Current motor speed</param>
	/// <param name="ser">Serial (for debugging - remove in production)</param>
	/// <returns>An integer value to be sent to the ESC</returns>
	virtual int translateMotorSpeed(InputSetting input, Gear gear, int currentMotorSpeed, HardwareSerial& ser) = 0;

	/// <summary>
	/// Translates the motor speed in cruise mode
	/// </summary>
	/// <param name="input">Input values decoded from the transmitter</param>
	/// <param name="gear">Current gear</param>
	/// <param name="currentMotorSpeed">Current motor speed</param>
	/// <param name="ser">Serial (for debugging - remove in production)</param>
	/// <returns>An integer value to be sent to the ESC</returns>
	virtual int translateCruiseSpeed(InputSetting input, Gear gear, int currentMotorSpeed, HardwareSerial& ser) = 0;

	/// <summary>
	/// Translates the steering angle
	/// </summary>
	/// <param name="input">Input values decoded from the transmitter</param>
	/// <param name="currentSteering">Current steering value</param>
	/// <param name="useInertia">Specifies if inertia should be used to translate steering</param>
	/// <param name="ser">Serial (for debugging - remove in production)</param>
	/// <returns>An integer value to be sent to the steering servo</returns>
	virtual int translateSteering(InputSetting input, int currentSteering, bool useInertia, HardwareSerial& ser) = 0;

	// Translates the winch operation settings
	virtual WinchSetting translateWinch(InputSetting input) const = 0;
};
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
	virtual Gear translateGear(InputSetting input, Gear currentGear) = 0;

	/// <summary>
	/// Translates the channel input value to the requested drive mode
	/// </summary>
	/// <param name="input">Input values decoded from the transmitter</param>
	/// <param name="currentGear">Currently selected gear</param>
	/// <param name="currentDriveMode">Current drive mode setting</param>
	/// <returns>A Cruise enum representing the current cruise mode</returns>
	virtual DriveMode translateDriveMode(InputSetting input, Gear currentGear, DriveMode currentDriveMode) = 0;

	/// <summary>
	/// Translates the motor speed in crawl mode
	/// </summary>
	/// <param name="input"Input values decoded from the transmitter></param>
	virtual int translateCrawlSpeed(InputSetting input) = 0;

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

	// Translates the steeering angle in crawl mode

	/// <summary>
	/// Translates the steeering angle in crawl mode
	/// </summary>
	/// <param name="input">Input values decoded from the transmitter</param>
	/// <returns>An integer value to be sent to the steering servo</returns>
	virtual int translateCrawlSteering(InputSetting input) = 0;

	/// <summary>
	/// Translates the steering angle in drive and cruise mode
	/// </summary>
	/// <param name="input">Input values decoded from the transmitter</param>
	/// <param name="currentSteering">Current steering value</param>
	/// <param name="ser">Serial (for debugging - remove in production)</param>
	/// <returns>An integer value to be sent to the steering servo</returns>
	virtual int translateDriveSteering(InputSetting input, int currentSteering, HardwareSerial& ser) = 0;

	// Translates the winch operation settings
	virtual WinchSetting translateWinch(InputSetting input) const = 0;
};
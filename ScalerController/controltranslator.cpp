#include "controltranslator.h"
#include "Arduino.h"

ControlTranslator::ControlTranslator(
	ControlConfig config,
	IInputTranslator* inputTranslator,
	IMotorSpeedTranslator* motorSpeedTranslator,
	IInertia* forwardAccel,
	IInertia* forwardDecel,
	IInertia* forwardBrake,
	IInertia* reverseAccel,
	IInertia* reverseDecel,
	IInertia* reverseBrake)
{
	this->config = config;
	this->inputTranslator = inputTranslator;
	this->motorSpeedTranslator = motorSpeedTranslator;
	this->forwardAccel = forwardAccel;
	this->forwardDecel = forwardDecel;
	this->forwardBrake = forwardBrake;
	this->reverseAccel = reverseAccel;
	this->reverseDecel = reverseDecel;
	this->reverseBrake = reverseBrake;
}

ControlTranslator::~ControlTranslator()
{
}

// Checks for a fail condition on the input
// Returns true if the input values have failed to update for more than a set number of reads
bool ControlTranslator::checkFailsafe(InputSetting input)
{
	if (lastInput == nullptr)
	{
		lastInput = new InputSetting(input);
		failsafeCount = 0;
		return false;
	}

	bool failed = true;
	for (int channel = 0; channel < 8; channel++)
	{
		failed &= lastInput->channel[channel] == input.channel[channel];
	}

	if (failed)
	{
		failsafeCount++;
	}
	else
	{
		failsafeCount = 0;
	}

	delete lastInput;
	lastInput = new InputSetting(input);

	// Note: this allows a failsafe condition to be reversed when reception
	// resumes.  Pending results of testing, it may be that we need to go to
	// a permanent failsafe or add a throttle to the failsafe so that the
	// controller will wait for an amount of time before restarting.
	return (failed && (failsafeCount > this->config.maxFailsafeCount));
}

// Translates the requested gear
Gear ControlTranslator::translateGear(InputSetting input, Gear lastGear)
{
	// TODO: M: make sure we can't get reverse if we're on cruise

	// If the throttle is not under the forward deadband, we should not change gear
	if (input.channel[this->config.throttleChannel.channel] > this->config.throttleChannel.dbMax)
	{
		return lastGear;
	}

	// TODO: M: all this code can go into a latching function

	LatchChannel channel = this->config.gearChannel;
	int value = input.channel[channel.channel];

	if (value > channel.min && value < channel.max)
	{
		// We are within the range to change

		// If we are already latching, we should not change the value
		if (gearLatching)
		{
			return lastGear;
		}
		else
		{
			// Record the latching state
			gearLatching = true;

			if (lastGear == Gear::Forward)
			{
				return Gear::Reverse;
			}
			else
			{
				return Gear::Forward;
			}
		}
	}
	else
	{
		// We are outside of the latch range - Remove any latch
		gearLatching = false;

		return lastGear;
	}
}

// Translates the motor speed
int ControlTranslator::translateMotorSpeed(InputSetting input, Gear gear, int currentMotorSpeed, HardwareSerial &ser)
{
	int desiredMotorSpeed;
	AnalogChannel channel = config.throttleChannel;
	ServoConfig servo = config.throttleServo;
	int inputVal = input.channel[channel.channel];

	switch (gear)
	{
	case Gear::Forward:
		// Calculate applied forward throttle
		if (inputVal > channel.dbMax)
		{
			// Throttle is on
			desiredMotorSpeed = map(inputVal, channel.dbMax, channel.max, servo.center, servo.max);
		}
		else if (inputVal < channel.dbMax && inputVal > channel.dbMin)
		{
			// Coasting
			desiredMotorSpeed = servo.center;
		}
		else
		{
			// Brake is on
			desiredMotorSpeed = map(inputVal, channel.min, channel.dbMin, servo.min, servo.center);
		}

		return motorSpeedTranslator->translateMotorSpeed(currentMotorSpeed, inputVal, desiredMotorSpeed, forwardAccel, forwardDecel, forwardBrake);
		
	case Gear::Reverse:
		// Calculate applied reverse throttle
		if (inputVal > channel.dbMax)
		{
			// Throttle is on
			desiredMotorSpeed = map(inputVal, channel.dbMax, channel.max, servo.center, servo.min);
		}
		else if (inputVal < channel.dbMax && inputVal > channel.dbMin)
		{
			// Coasting
			desiredMotorSpeed = servo.center;
		}
		else
		{
			// Brake is on
			desiredMotorSpeed = map(inputVal, channel.min, channel.dbMin, servo.max, servo.center);
		}

		return motorSpeedTranslator->translateMotorSpeed(currentMotorSpeed, inputVal, desiredMotorSpeed, reverseAccel, reverseDecel, reverseBrake);
	}

	// Failsafe
	return servo.center;
}

int ControlTranslator::translateSteering(InputSetting input)
{
	return this->inputTranslator->translateStickInput(input.channel[config.steeringChannel.channel], config.steeringChannel, config.steeringServo);
}
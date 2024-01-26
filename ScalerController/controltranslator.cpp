#include "controltranslator.h"

ControlTranslator::ControlTranslator(
	ControlConfig config,
	IInputTranslator* inputTranslator
//	IMotorSpeedTranslator* motorSpeedTranslator,
//	IInertia* forwardAccel,
//	IInertia* forwardDecel,
//	IInertia* forwardBrake,
//	IInertia* reverseAccel,
//	IInertia* reverseDecel,
//	IInertia* reverseBrake)
)
{
	this->config = config;
	this->inputTranslator = inputTranslator;
//	this->motorSpeedTranslator = motorSpeedTranslator;
//	this->forwardAccel = forwardAccel;
//	this->forwardDecel = forwardDecel;
//	this->forwardBrake = forwardBrake;
//	this->reverseAccel = reverseAccel;
//	this->reverseDecel = reverseDecel;
//	this->reverseBrake = reverseBrake;
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
	// If the throttle is not under the forward deadband, we should not change gear
	if (input.channel[this->config.throttleChannel.channel] > this->config.throttleChannel.dbMax)
	{
		return lastGear;
	}

	// TODO: M: all this code can go into a latching function

	LatchChannel chCfg = this->config.gearChannel;
	int chVal = input.channel[chCfg.channel];

	if (chVal > chCfg.min && chVal < chCfg.max)
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
//int ControlTranslator::translateMotorSpeed(int currentMotorSpeed, int input, Gear gear)
int ControlTranslator::translateMotorSpeed(InputSetting input, Gear gear)
{
	int desiredMotorSpeed;
	AnalogChannel ch = config.throttleChannel;
	int inputVal = input.channel[ch.channel];

	if (inputVal < ch.dbMax)
	{
		// If we are in center / brake position, return neutral
		return config.throttleServo.center;
	}

	switch (gear)
	{
	case Gear::Forward:
		// Pass back the value we received
		return inputVal;

//		// Calculate applied forward throttle
//		if (input > config.throttleChannel.dbMax)
//			desiredMotorSpeed = map(input, config.throttleChannel.dbMax, config.throttleChannel.max, config.throttleServo.center, config.throttleServo.max);
//		else if (input < config.throttleChannel.dbMin)
//			desiredMotorSpeed = map(input, config.throttleChannel.min, config.throttleChannel.dbMin, config.throttleServo.min, config.throttleServo.center);
//		else
//			desiredMotorSpeed = config.throttleServo.center;
//
//		return motorSpeedTranslator->translateMotorSpeed(currentMotorSpeed, input, desiredMotorSpeed, forwardAccel, forwardDecel, forwardBrake);
//		
	case Gear::Reverse:
		// Translate the forward value into a reverse value, and pass it back
		return ((inputVal - config.throttleServo.center) * -1) + config.throttleServo.center;

//		// Calculate applied reverse throttle
//		if (input > config.throttleChannel.dbMax)
//			desiredMotorSpeed = map(input, config.throttleChannel.dbMax, config.throttleChannel.max, config.throttleServo.center, config.throttleServo.min);
//		else if (input < config.throttleChannel.dbMin)
//			desiredMotorSpeed = map(input, config.throttleChannel.min, config.throttleChannel.dbMin, config.throttleServo.max, config.throttleServo.center);
//		else
//			desiredMotorSpeed = config.throttleServo.center;
//
//		return motorSpeedTranslator->translateMotorSpeed(currentMotorSpeed, input, desiredMotorSpeed, reverseAccel, reverseDecel, reverseBrake);
	}

	// Failsafe
	return config.throttleServo.center;
}

int ControlTranslator::translateSteering(InputSetting input)
{
	return this->inputTranslator->translateStickInput(input.channel[config.steeringChannel.channel], config.steeringChannel, config.steeringServo);
}
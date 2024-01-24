#include "controltranslator.h"

ControlTranslator::ControlTranslator(
	ControlConfig config
//	IInputTranslator* inputTranslator, 
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
//	this->inputTranslator = inputTranslator;
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
	
	lastInput = new InputSetting(input);

	// Note: this allows a failsafe condition to be reversed when reception
	// resumes.  Pending results of testing, it may be that we need to go to
	// a permanent failsafe or add a throttle to the failsafe so that the
	// controller will wait for an amount of time before restarting.
	return (failed && (failsafeCount > this->config.maxFailsafeCount));
}

//// Translates the requested mode
//Mode ControlTranslator::translateMode(int input)
//{
//	return static_cast<Mode>(inputTranslator->translateThreeWaySwitch(input, config.switchHigh, config.switchLow));
//}
//
//// Translates the requested gear
//Gear ControlTranslator::translateGear(int input)
//{
//	return static_cast<Gear>(inputTranslator->translateThreeWaySwitch(input, config.switchHigh, config.switchLow));
//}
//
//// Translates the motor speed
//int ControlTranslator::translateMotorSpeed(int currentMotorSpeed, int input, Gear gear)
//{
//	int desiredMotorSpeed;
//	switch (gear)
//	{
//	case Gear::Neutral:
//		// Coast to stationary regardless of throttle position
//		if (currentMotorSpeed > config.throttleServo.center)
//		{
//			// Truck is coasting forwards - work out coast / brake speed
//			return motorSpeedTranslator->translateMotorSpeed(currentMotorSpeed, input, config.throttleServo.center, forwardAccel, forwardDecel, forwardBrake);
//		}
//		if (currentMotorSpeed < config.throttleServo.center)
//		{
//			// Truck is coasting backwards - work out coast / brake speed
//			return motorSpeedTranslator->translateMotorSpeed(currentMotorSpeed, input, config.throttleServo.center, reverseAccel, reverseDecel, reverseBrake);
//		}
//		// Truck is already stationary
//		return config.throttleServo.center;
//
//	case Gear::Forward:
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
//	case Gear::Reverse:
//		// Calculate applied reverse throttle
//		if (input > config.throttleChannel.dbMax)
//			desiredMotorSpeed = map(input, config.throttleChannel.dbMax, config.throttleChannel.max, config.throttleServo.center, config.throttleServo.min);
//		else if (input < config.throttleChannel.dbMin)
//			desiredMotorSpeed = map(input, config.throttleChannel.min, config.throttleChannel.dbMin, config.throttleServo.max, config.throttleServo.center);
//		else
//			desiredMotorSpeed = config.throttleServo.center;
//
//		return motorSpeedTranslator->translateMotorSpeed(currentMotorSpeed, input, desiredMotorSpeed, reverseAccel, reverseDecel, reverseBrake);
//	}
//
//	// Failsafe
//	return config.throttleServo.center;
//}
//
//int ControlTranslator::translateSteering(int input)
//{
//	return inputTranslator->translateStickInput(input, config.steeringChannel, config.steeringServo);
//}
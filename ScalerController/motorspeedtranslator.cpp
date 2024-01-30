#include "motorspeedtranslator.h"

MotorSpeedTranslator::MotorSpeedTranslator(AnalogChannel channel, ServoConfig servo)
{
	this->channel = channel;
	this->servo = servo;
}

MotorSpeedTranslator::~MotorSpeedTranslator()
{
}

int MotorSpeedTranslator::translateMotorSpeed(
	int currentMotorSpeed, 
	int input, 
	int desiredMotorSpeed, 
	IInertia* accel, 
	IInertia* decel, 
	IInertia* brake)
{
	if (input < channel.dbMin)
	{
		// User is actively braking - use the brake map
		return brake->map(currentMotorSpeed, desiredMotorSpeed);
	}

	if (desiredMotorSpeed > currentMotorSpeed)
	{
		// Accelerating - use the acceleration map
		return accel->map(currentMotorSpeed, desiredMotorSpeed);
	}
	if (desiredMotorSpeed < currentMotorSpeed)
	{
		// Decelerating - use the deceleration map
		return decel->map(currentMotorSpeed, desiredMotorSpeed);
	}

	// Holding steady
	return currentMotorSpeed;
}
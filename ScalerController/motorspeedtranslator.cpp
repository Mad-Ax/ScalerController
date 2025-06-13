#include "motorspeedtranslator.h"

MotorSpeedTranslator::MotorSpeedTranslator(AnalogChannel channel, ServoConfig servo)
{
	this->channel = channel;
	this->servo = servo; // TODO: M: don't think we need the servo here
}

MotorSpeedTranslator::~MotorSpeedTranslator()
{
}

// TODO: M: naming bug: when reversing, decel is accel; accel is decel; we need a better way to name this.  Also could be an issue when we reverse throttle for reverse-installed motors?
int MotorSpeedTranslator::translateMotorSpeed(
	int currentMotorSpeed,
	int input,
	int desiredMotorSpeed,
	bool useInertia,
	IInertia* accel, 
	IInertia* decel, 
	IInertia* brake)
{// TODO: M: useInertia no longer used here - remove it
	if (!useInertia)
	{
		return desiredMotorSpeed;
	}

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
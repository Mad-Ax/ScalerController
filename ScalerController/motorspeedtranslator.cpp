#include "motorspeedtranslator.h"

MotorSpeedTranslator::MotorSpeedTranslator(
	AnalogChannel& channel,
	const IInertia& accel,
	const IInertia& decel,
	const IInertia& brake) :
	channel(channel),
	accel(accel),
	decel(decel),
	brake(brake)
{
}

MotorSpeedTranslator::~MotorSpeedTranslator()
{
}

int MotorSpeedTranslator::translateMotorSpeed(
	int currentMotorSpeed,
	int input,
	int desiredMotorSpeed) const
{
	if (input < channel.dbMin)
	{
		// User is actively braking - use the brake map
		return this->brake.map(currentMotorSpeed, desiredMotorSpeed);
	}

	if (desiredMotorSpeed > currentMotorSpeed)
	{
		// Accelerating - use the acceleration map
		return this->accel.map(currentMotorSpeed, desiredMotorSpeed);
	}
	if (desiredMotorSpeed < currentMotorSpeed)
	{
		// Decelerating - use the deceleration map
		return this->decel.map(currentMotorSpeed, desiredMotorSpeed);
	}

	// Holding steady
	return currentMotorSpeed;
}
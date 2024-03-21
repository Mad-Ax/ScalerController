#include "motorspeedtranslator.h"

MotorSpeedTranslator::MotorSpeedTranslator(AnalogChannel channel, ServoConfig servo, const IInertia& accel, const IInertia& decel, const IInertia& brake)
	: accel(accel), decel(decel), brake(brake)
{
	this->channel = channel;
	this->servo = servo; // TODO: M: don't think we need the servo here
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
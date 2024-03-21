#pragma once

#include "imotorspeedtranslator.h"
#include "structs.h"

class MotorSpeedTranslator : public IMotorSpeedTranslator
{
public:
	MotorSpeedTranslator(AnalogChannel channel, ServoConfig servo, const IInertia& accel, const IInertia& decel, const IInertia& brake);
	~MotorSpeedTranslator();

	// Translates the input into a motor speed
	int translateMotorSpeed(int currentMotorSpeed, int input, int desiredMotorSpeed, IInertia* accel, IInertia* decel, IInertia* brake);

	// Translates the input into a motor speed
	int translateMotorSpeed(int currentMotorSpeed, int input, int desiredMotorSpeed) const;

private:
	AnalogChannel channel;
	ServoConfig servo;
	const IInertia& accel;
	const IInertia& decel;
	const IInertia& brake;
};
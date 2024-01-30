#include "controltranslator.h"
#include "Arduino.h"

ControlTranslator::ControlTranslator(
	ControlConfig config,
	IInputTranslator* inputTranslator,
	IMotorSpeedTranslator* motorSpeedTranslator,
	ILatchTranslator* gearTranslator,
	ILatchTranslator* cruiseTranslator,
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
	this->gearTranslator = gearTranslator;
	this->cruiseTranslator = cruiseTranslator;
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

// Translates the channel input value to the requested gear value
Gear ControlTranslator::translateGear(InputSetting input, Gear lastGear)
{
	// TODO: M: make sure we can't get reverse if we're on cruise

	// If the throttle is not under the forward deadband, we should not change gear
	if (input.channel[this->config.throttleChannel.channel] > this->config.throttleChannel.dbMax)
	{
		return lastGear;
	}
	LatchChannel channel = this->config.gearChannel;
	int value = input.channel[channel.channel];

	int lastLatchValue = static_cast<int>(lastGear);
	int newLatchValue = this->gearTranslator->translateLatch(channel, value, lastLatchValue);
	if (lastLatchValue == newLatchValue)
	{
		return lastGear;
	}

	return static_cast<Gear>(newLatchValue);
}

// Translates the channel input value to the requested cruise value
Cruise ControlTranslator::translateCruise(InputSetting input, Cruise lastCruise)
{
	LatchChannel channel = this->config.cruiseChannel;
	int value = input.channel[channel.channel];

	int lastLatchValue = static_cast<int>(lastCruise);
	int newLatchValue = this->cruiseTranslator->translateLatch(channel, value, lastLatchValue);
	if (lastLatchValue == newLatchValue)
	{
		return lastCruise;
	}

	return static_cast<Cruise>(newLatchValue);
}

// Translates the motor speed in drive mode
int ControlTranslator::translateMotorSpeed(InputSetting input, Gear gear, int currentMotorSpeed, HardwareSerial &ser)
{
	int desiredMotorSpeed;
	ThrottleChannel channel = config.throttleChannel;
	ServoConfig servo = config.throttleServo;
	int inputVal = input.channel[channel.channel];

	if (inputVal < channel.eBrakeThreshold)
	{
		// We have applied the e-brake - come to full stop
		return servo.center;
	}

	switch (gear)
	{
	case Gear::Forward:
		// Calculate applied forward throttle
		if (inputVal > channel.dbMax)
		{
			// Throttle is on
			desiredMotorSpeed = map(inputVal, channel.dbMax, channel.max, servo.center, servo.max);
		}
		else if (inputVal < channel.dbMax && inputVal >= channel.dbMin)
		{
			// Coasting to stationary
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
		else if (inputVal < channel.dbMax && inputVal >= channel.dbMin)
		{
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

// Translates the motor speed in cruise mode
int ControlTranslator::translateCruiseSpeed(InputSetting input, Gear gear, int currentMotorSpeed, HardwareSerial& ser)
{
	int desiredMotorSpeed;
	ThrottleChannel channel = config.throttleChannel;
	ServoConfig servo = config.throttleServo;
	int inputVal = input.channel[channel.channel];

	if (inputVal < channel.eBrakeThreshold)
	{
		// We have applied the e-brake - come to full stop
		return servo.center;
	}

	switch (gear)
	{
	case Gear::Forward:
		// Calculate applied foward throttle
		if (inputVal > channel.dbMax)
		{
			// Throttle is on - increase the set speed by the amount of throttle added

			// Get the value of throttle applied, in terms of half a servo output
			int throttleValue = map(inputVal, channel.dbMax, channel.max, servo.center, servo.max - ((servo.max - servo.center) / 2) );

			// Subtract the centerpoint from the absolute throtteValue to determine the increased value requested by the driver,
			// and add it to the current motor speed to get our new desired speed
			desiredMotorSpeed = constrain((throttleValue - servo.center) + currentMotorSpeed, servo.center, servo.max);
		}
		else if (inputVal < channel.dbMax && inputVal >= channel.dbMin)
		{
			// No throttle applied - continue at a constant speed
			desiredMotorSpeed = currentMotorSpeed;
		}
		else
		{
			// Brake is on - decrease the set speed by the amount of throttle subtracted

			// Get the value of brake applied, in terms of servo output
			int brakeValue = map(inputVal, channel.min, channel.dbMin, servo.min + ((servo.center - servo.min) / 2), servo.center);

			// Subtract the absolute brake value from the centerpoint to determine the decreased value requested by the driver,
			// and subtract it from the current motor speed to get our new desired speed
			desiredMotorSpeed = constrain(currentMotorSpeed - (servo.center - brakeValue), servo.center, servo.max);
		}

		return motorSpeedTranslator->translateMotorSpeed(currentMotorSpeed, inputVal, desiredMotorSpeed, forwardAccel, forwardDecel, forwardBrake);

	case Gear::Reverse:
		// TODO: M: same as above but backwards
		return servo.center;
	}

	// Failsafe
	return servo.center;
}

int ControlTranslator::translateSteering(InputSetting input)
{
	return this->inputTranslator->translateStickInput(input.channel[config.steeringChannel.channel], config.steeringChannel, config.steeringServo);
}
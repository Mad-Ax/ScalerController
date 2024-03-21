#include "controltranslator.h"
#include "Arduino.h"

ControlTranslator::ControlTranslator(
	ControlConfig config,
	const ISteeringTranslator& steeringTranslator,
	const IMotorSpeedTranslator& forwardMotorSpeedTranslator,
	const IMotorSpeedTranslator& reverseMotorSpeedTranslator,
	ILatchTranslator* gearTranslator,
	ILatchTranslator* cruiseTranslator,
	ISwitchTranslatorThreeWay* winchSelectTranslator) :
	steeringTranslator(steeringTranslator),
	forwardMotorSpeedTranslator(forwardMotorSpeedTranslator),
	reverseMotorSpeedTranslator(reverseMotorSpeedTranslator)
{
	this->config = config;
	this->gearTranslator = gearTranslator;
	this->cruiseTranslator = cruiseTranslator;
	this->winchSelectTranslator = winchSelectTranslator;
}

ControlTranslator::~ControlTranslator()
{
}

// Checks for a fail condition on the input
// Returns true if the input values have failed to update for more than a set number of reads
bool ControlTranslator::checkFailsafe(const InputSetting& input)
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
Gear ControlTranslator::translateGear(const InputSetting& input, Gear lastGear)
{
	// TODO: M: make sure we can't get reverse if we're on cruise

	// If the throttle is not under the forward deadband, we should not change gear
	if (input.channel[this->config.throttleChannel.channel] > this->config.throttleChannel.dbMax)
	{
		return lastGear;
	}

	const LatchChannel& channel = this->config.gearChannel;
	int value = input.channel[channel.channel];

	if (this->gearTranslator->translateLatch(value))
	{
		// We have requested to change gear
		switch (lastGear)
		{
		case Gear::Forward:
			return Gear::Reverse;

		case Gear::Reverse:
			return Gear::Forward;
		}
	}
	
	return lastGear;
}

// Translates the channel input value to the requested cruise value
Cruise ControlTranslator::translateCruise(const InputSetting& input, Cruise lastCruise)
{
	const LatchChannel& channel = this->config.cruiseChannel;
	int value = input.channel[channel.channel];

	if (this->cruiseTranslator->translateLatch(value))
	{
		// We have requested to change the cruise mode
		switch (lastCruise)
		{
		case Cruise::Off:
			return Cruise::On;

		case Cruise::On:
			return Cruise::Off;
		}
	}

	return lastCruise;
}

// Translates the motor speed in drive mode
int ControlTranslator::translateMotorSpeed(const InputSetting& input, Gear gear, int currentMotorSpeed, HardwareSerial &ser) const
{
	int desiredMotorSpeed;
	const ThrottleChannel& channel = config.throttleChannel;
	const ServoConfig& servo = config.throttleServo;
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

		return this->forwardMotorSpeedTranslator.translateMotorSpeed(currentMotorSpeed, inputVal, desiredMotorSpeed);
		
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

		return this->reverseMotorSpeedTranslator.translateMotorSpeed(currentMotorSpeed, inputVal, desiredMotorSpeed);
	}

	// Failsafe
	return servo.center;
}

// Translates the motor speed in cruise mode
int ControlTranslator::translateCruiseSpeed(const InputSetting& input, Gear gear, int currentMotorSpeed, HardwareSerial& ser) const
{
	int desiredMotorSpeed;
	const ThrottleChannel& channel = config.throttleChannel;
	const ServoConfig& servo = config.throttleServo;
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

		return forwardMotorSpeedTranslator.translateMotorSpeed(currentMotorSpeed, inputVal, desiredMotorSpeed);

	case Gear::Reverse:
		// Calculate applied reverse throttle
		if (inputVal > channel.dbMax)
		{
			// Throttle is on - increase the set speed by the amount of throttle added

			// Get the value of throttle applied, in terms of half a servo output
			int throttleValue = map(inputVal, channel.dbMax, channel.max, servo.center, servo.max - ((servo.max - servo.center) / 2));

			// Subtract the centerpoint from the absolute throtteValue to determine the increased value requested by the driver,
			// and subtract it from the current motor speed to get our new desired speed
			desiredMotorSpeed = constrain(currentMotorSpeed - (throttleValue - servo.center), servo.min, servo.center);
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
			// and add it to the current motor speed to get our new desired speed
			desiredMotorSpeed = constrain(currentMotorSpeed + (servo.center - brakeValue), servo.min, servo.center);
		}

		return reverseMotorSpeedTranslator.translateMotorSpeed(currentMotorSpeed, inputVal, desiredMotorSpeed);
	}

	// Failsafe
	return servo.center;
}

int ControlTranslator::translateSteering(const InputSetting& input, int currentSteering, HardwareSerial& ser) const
{
	const AnalogChannel& channel = config.steeringChannel;
	const ServoConfig& servo = config.steeringServo;
	int inputVal = input.channel[channel.channel];

	auto desiredSteering = map(inputVal, channel.min, channel.max, servo.min, servo.max);

	// TODO M: isn't it possible to store "current steering" inside the translator..?
	return this->steeringTranslator.translateSteering(currentSteering, desiredSteering, ser);
}

WinchSetting ControlTranslator::translateWinch(const InputSetting& input)
{
	const SwitchChannelThreeWay& selectChannel = config.winchSelectChannel;
	const AnalogChannel& operateChannel = config.winchOperationChannel;
	const ServoConfig& winch1Servo = config.winch1Servo;
	const ServoConfig& winch2Servo = config.winch2Servo;
	int selectInputVal = input.channel[selectChannel.channel];
	int operateInputVal = input.channel[operateChannel.channel];
		
	WinchSetting setting;

	auto winchSwitch = this->winchSelectTranslator->translateSwitch(selectInputVal);

	switch (winchSwitch)
	{
	case ThreeWayPosition::PosA:
		setting.winch2 = map(operateInputVal, operateChannel.min, operateChannel.max, winch2Servo.min, winch2Servo.max);
		setting.winch1 = winch1Servo.center;
		break;

	case ThreeWayPosition::PosB:
		setting.winch1 = map(operateInputVal, operateChannel.min, operateChannel.max, winch1Servo.min, winch1Servo.max);
		setting.winch2 = winch2Servo.center;
		break;

	case ThreeWayPosition::PosC:
	default:
		setting.winch1 = winch1Servo.center;
		setting.winch2 = winch2Servo.center;
		break;
	}

	return setting;
}
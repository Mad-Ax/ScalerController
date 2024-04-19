#include "control.h"

// Initializes a new instance of the object with the required config data
Control::Control(
	const int& throttleServoCenter,
	IControlTranslator& controlTranslator,
	ILightingTranslator& lightingTranslator,
	ControlConfig& controlConfig) :
	throttleServoCenter(throttleServoCenter),
	controlTranslator(controlTranslator),
	lightingTranslator(lightingTranslator),
	config(controlConfig)
{
	setting.cruise = Cruise::Off;
	setting.gear = Gear::Forward;

	// TODO: is this a potential issue when setting the const controlconfig ..?
	setting.motorSpeed = throttleServoCenter;
	setting.steering = config.steeringServo.center;
	setting.winch1 = config.winch1Servo.center;
	setting.winch2 = config.winch2Servo.center;
	setting.lightSetting.brakeIntensity = config.lightModeConfig.brakeIntensityMax;
}

void Control::translate(const InputSetting& input, HardwareSerial &ser)
{
	// Check for failsafe condition and shut off truck
	if (this->controlTranslator.checkFailsafe(input))
	{
		setting.motorSpeed = throttleServoCenter;
		setting.steering = config.steeringServo.center;
		setting.winch1 = config.winch1Servo.center;
		setting.winch2 = config.winch2Servo.center;
		setting.lightSetting.brakeIntensity = config.lightModeConfig.brakeIntensityMax;
		setting.lightSetting.reverseIntensity = 0;
		setting.lightSetting.headLightIntensity = 0;
		setting.lightSetting.roofLightIntensity = 0;
		setting.lightSetting.brakeIntensity = 0;

		// TODO: M: all other channels to centre
		// TODO: failsafe flash mode
		return;
	}

	Serial.print("free memory is :");
	Serial.println(freeMemory());

	// If we are in pass-thru mode, translate the steering and ESC only
	// Set all other channels to centre
	if (input.mode == Mode::PassThru) // TODO: M: we might want to use the control translator so we can remove dependency on config object
	{
		ser.println("we're in pass-thru mode!");

		setting.motorSpeed = input.channel[config.throttleChannel.channel];
		setting.steering = input.channel[config.steeringChannel.channel];
		setting.winch1 = config.winch1Servo.center;
		setting.winch2 = config.winch2Servo.center;
		setting.lightSetting.brakeIntensity = 0;
		setting.lightSetting.reverseIntensity = 0;
		setting.lightSetting.headLightIntensity = 0;
		setting.lightSetting.roofLightIntensity = 0;

		// TODO: M: all other channels to centre
		return;
	}

	ser.println("wtff!!??");

	// Get the selected gear (if the motor is stationary)
	if (setting.motorSpeed == this->throttleServoCenter)
	{
		setting.gear = this->controlTranslator.translateGear(input, setting.gear);
	}

	// Determine if cruise is on or off
	setting.cruise = this->controlTranslator.translateCruise(input, setting.cruise);
	
	// Get the requested motor speed
	switch (setting.cruise)
	{
	case Cruise::Off:
		setting.motorSpeed = this->controlTranslator.translateMotorSpeed(input, setting.gear, setting.motorSpeed, ser);
		break;

	case Cruise::On:
		setting.motorSpeed = this->controlTranslator.translateCruiseSpeed(input, setting.gear, setting.motorSpeed, ser);
		break;
	}

	// Get the steering servo position
	setting.steering = this->controlTranslator.translateSteering(input, setting.steering, ser);

	// TODO: reinstate lighting (why is it broken?)
	// Get the light setting
	setting.lightSetting = this->lightingTranslator.translateLightSetting(input, setting.gear);

	// Get the winch setting
	auto winchSetting = this->controlTranslator.translateWinch(input);
	setting.winch1 = winchSetting.winch1;
	setting.winch2 = winchSetting.winch2;
}

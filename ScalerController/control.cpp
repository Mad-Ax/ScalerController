#include "control.h"

// Initializes a new instance of the object with the required config data
Control::Control(IControlTranslator* controlTranslator, ILightingTranslator* lightingTranslator, ControlConfig controlConfig)
{
	this->controlTranslator = controlTranslator;
	this->lightingTranslator = lightingTranslator;
	config = controlConfig;
//	forwardAccel = new Inertia(controlConfig.fwdAccelInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
//	forwardDecel = new Inertia(controlConfig.fwdDecelInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
//	forwardBrake = new Inertia(controlConfig.fwdBrakeInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
//	reverseAccel = new Inertia(controlConfig.revAccelInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
//	reverseDecel = new Inertia(controlConfig.revDecelInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
//	reverseBrake = new Inertia(controlConfig.revBrakeInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
//
//	setting.lightSetting.brakeLightIntensity = 0;

	setting.useInertia = true;
	setting.cruise = Cruise::Off;
	setting.gear = Gear::Forward;
	setting.motorSpeed = config.throttleServo.center;
	setting.steering = config.steeringServo.center;
	setting.aux1 = config.aux1Center;
	setting.winch1 = config.winch1Servo.center;
	setting.winch2 = config.winch2Servo.center;
	setting.lightSetting.brakeIntensity = config.lightModeConfig.brakeIntensityMax;
	setting.winchMode = WinchMode::Off;
	setting.crawlModeWhenWinching = (config.winchOperationChannel.channel == config.gearChannel.channel);
}

void Control::translate(InputSetting input, HardwareSerial &ser)
{
	// Check for failsafe condition and shut off truck
	if (this->controlTranslator->checkFailsafe(input))
	{
		setting.motorSpeed = config.throttleServo.center;
		setting.steering = config.steeringServo.center;
		setting.aux1 = config.aux1Center;
		setting.winch1 = config.winch1Servo.center;
		setting.winch2 = config.winch2Servo.center;
		setting.lightSetting.brakeIntensity = config.lightModeConfig.brakeIntensityMax;
		setting.lightSetting.reverseIntensity = 0;
		setting.lightSetting.headLightIntensity = 0;
		setting.lightSetting.roofLightIntensity = 0;
		setting.lightSetting.brakeIntensity = 0;

		// Set the failsafe on the dash LED
		setting.lightSetting.dashLightSetting.redIntensity = 255; // TODO: M: get from a config section somehow
		setting.lightSetting.dashLightSetting.greenIntensity = 0; // TODO: M: get from a config section somehow
		setting.lightSetting.dashLightSetting.blueIntensity = 0; // TODO: M: get from a config section somehow

		// TODO: M: all other channels to centre

		return;
	}

	// If we are in pass-thru mode, translate the steering and ESC only
	// Set all other channels to centre
	if (input.mode == Mode::PassThru) // TODO: M: we might want to use the control translator so we can remove dependency on config object
	{
		setting.motorSpeed = input.channel[config.throttleChannel.channel];
		setting.steering = input.channel[config.steeringChannel.channel];
		setting.aux1 = input.channel[config.aux1Channel];
		setting.winch1 = config.winch1Servo.center;
		setting.winch2 = config.winch2Servo.center;
		setting.lightSetting.brakeIntensity = 0;
		setting.lightSetting.reverseIntensity = 0;
		setting.lightSetting.headLightIntensity = 0;
		setting.lightSetting.roofLightIntensity = 0;
		setting.lightSetting.dashLightSetting.redIntensity = 0;
		setting.lightSetting.dashLightSetting.greenIntensity = 0;
		setting.lightSetting.dashLightSetting.blueIntensity = 0;

		// TODO: M: all other channels to centre
		return;
	}

	// Get the winch setting
	auto winchSetting = this->controlTranslator->translateWinch(input);
	setting.winch1 = winchSetting.winch1Position;
	setting.winch2 = winchSetting.winch2Position;

	// Determine if we need to reset the operation mode
	if (resetAfterWinching(setting.crawlModeWhenWinching, winchSetting.winchMode, setting.winchMode))
	{
		// If we have turned off the winch, and we share a winch and gear channel,
		// we should reset to Forward gear, turn off cruise, and use inertia
		setting.useInertia = true;
		setting.gear = Gear::Forward;
		setting.cruise = Cruise::Off;
	}
	else if (setCrawlModeWhileWinching(setting.crawlModeWhenWinching, winchSetting.winchMode))
	{
		// If we have started winching, and we share a winch and gear channel, we
		// should set to crawl mode
		setting.useInertia = false;
		setting.gear = Gear::Forward;
		setting.cruise = Cruise::Off;
	}
	else
	{
		// In non-winch scenarios, or when we don't share a winch channel, we can
		// go ahead and set our gear and inertia modes in the normal way

		// Determine if inertia is on or off
		setting.useInertia = this->controlTranslator->translateInertia(input, setting.useInertia, setting.gear, setting.cruise);

		// Get the selected gear (if the motor is stationary)
		if (setting.motorSpeed == this->config.throttleServo.center)
		{
			setting.gear = this->controlTranslator->translateGear(input, setting.gear);
		}

		// Determine if cruise is on or off
		setting.cruise = this->controlTranslator->translateCruise(input, setting.cruise, setting.gear);
	}

	setting.winchMode = winchSetting.winchMode;

	// Get the requested motor speed
	switch (setting.cruise)
	{
	case Cruise::Off:
		setting.motorSpeed = this->controlTranslator->translateMotorSpeed(input, setting.gear, setting.motorSpeed, setting.useInertia, ser);
		break;

	case Cruise::On:
		setting.motorSpeed = this->controlTranslator->translateCruiseSpeed(input, setting.gear, setting.motorSpeed, ser);
		break;
	}

	// Get the steering servo position - do not use inertia if a) useAccelInertia = true, or cruise is on
	setting.steering = this->controlTranslator->translateSteering(input, setting.steering, setting.useInertia || setting.cruise == Cruise::On, ser);

	// Get the light setting
	setting.lightSetting = this->lightingTranslator->translateLightSetting(input, setting.gear, setting.cruise, setting.useInertia, ser);

	// Get the aux channel settings - these are pass-thru and have no servo mapping
	setting.aux1 = input.channel[config.aux1Channel];
}

// If winch operation and gear select share the same channel, and we have
// just come out of winch mode, we should reset the drive mode and gear
bool Control::resetAfterWinching(bool crawlModeWhenWinching, WinchMode newWinchMode, WinchMode oldWinchMode)
{
	if (!crawlModeWhenWinching)
	{
		return false;
	}

	if (newWinchMode == WinchMode::Off && (oldWinchMode == WinchMode::Winch1 || oldWinchMode == WinchMode::Winch2))
	{
		return true;
	}

	return false;
}

// If we use crawl mode when winching, and we are in winch mode, we should
// use crawl mode
bool Control::setCrawlModeWhileWinching(bool crawlModeWhenWinching, WinchMode winchMode)
{
	if (!crawlModeWhenWinching)
	{
		return false;
	}

	return (winchMode == WinchMode::Winch1 || winchMode == WinchMode::Winch2);
}
#include "control.h"

// Initializes a new instance of the object with the required config data
//Control::Control(IControlTranslator* controlTranslator, ILightingTranslator* lightingTranslator, ControlConfig controlConfig)
Control::Control(IControlTranslator* controlTranslator, ControlConfig controlConfig)
{
	this->controlTranslator = controlTranslator;
//	this->lightingTranslator = lightingTranslator;
	config = controlConfig;
//	forwardAccel = new Inertia(controlConfig.fwdAccelInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
//	forwardDecel = new Inertia(controlConfig.fwdDecelInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
//	forwardBrake = new Inertia(controlConfig.fwdBrakeInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
//	reverseAccel = new Inertia(controlConfig.revAccelInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
//	reverseDecel = new Inertia(controlConfig.revDecelInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
//	reverseBrake = new Inertia(controlConfig.revBrakeInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
//
//	setting.lightSetting.brakeLightIntensity = 0;

	setting.cruise = Cruise::Off;
	setting.gear = Gear::Forward;
	setting.motorSpeed = config.throttleServo.center;
	setting.steering = config.steeringServo.center;
}

void Control::translate(InputSetting input, HardwareSerial &ser)
{
	// Check for failsafe condition and shut off truck
	if (this->controlTranslator->checkFailsafe(input))
	{
		setting.motorSpeed = config.throttleServo.center;
		setting.steering = config.steeringServo.center;
//		setting.lightSetting.brakeLightIntensity = config.lightModeConfig.brakeMax;
//		setting.lightSetting.reverseLightIntensity = LOW;
//		setting.lightSetting.headLightIntensity = 0;
//		setting.lightSetting.frontFogIntensity = 0;
//		setting.lightSetting.rearFogIntensity = 0;
//		setting.lightSetting.roofLightIntensity = 0;
//
//		setting.lightSetting.indicatorLeftIntensity = setting.lightSetting.indicatorRightIntensity = this->indicatorToggle(this->config.lightModeConfig.failsafeFlashDelay);
		return;
	}

	// If we are in pass-thru mode, translate the steering and ESC only
	// Set all other channels to centre
	if (input.mode == 0) // TODO: M: we might want to use the control translator so we can remove dependency on config object
	{
		setting.motorSpeed = input.channel[config.throttleChannel.channel];
		setting.steering = input.channel[config.steeringChannel.channel];

		// TODO: M: all other channels to centre
		return;
	}

	// Get the selected gear (if the motor is stationary)
	if (setting.motorSpeed == this->config.throttleServo.center)
	{
		setting.gear = this->controlTranslator->translateGear(input, setting.gear);
	}

	// Determine if cruise is on or off
	setting.cruise = this->controlTranslator->translateCruise(input, setting.cruise);
	
	// Get the requested motor speed
	switch (setting.cruise)
	{
	case Cruise::Off:
		setting.motorSpeed = this->controlTranslator->translateMotorSpeed(input, setting.gear, setting.motorSpeed, ser);
		break;

	case Cruise::On:
		setting.motorSpeed = this->controlTranslator->translateCruiseSpeed(input, setting.gear, setting.motorSpeed, ser);
		break;
	}

	//ser.println(setting.motorSpeed);

	// Get the steering servo position
	setting.steering = this->controlTranslator->translateSteering(input);

//	// Get the indicator setting
//	if (config.indicator.channel > -1)
//		translateIndicator(input.channel[config.indicator.channel]);
//
//	if (config.lights.channel > -1)
//	{
//		// Get the light mode
//		translateLightMode(input.channel[config.lights.channel]);
//
//		// Get the main beam setting
//		translateMainBeam(input.channel[config.lights.channel]);
//
//		// Get the brake light setting
//		translateBrakeLight(input.channel[config.throttleChannel.channel]);
//
//		// Get the reverse light setting
//		translateReverseLight();
//	}
}

//// Translates the indicator input to the desired output depending on mode
//void Control::translateIndicator(int input)
//{
//	if (mode == Mode::Off || mode == Mode::Park)
//	{
//		this->indicator = Indicator::Off;
//		this->indicatorTime = 0;
//		this->indicatorOn = false;
//		setting.lightSetting.indicatorLeftIntensity = LOW;
//		setting.lightSetting.indicatorRightIntensity = LOW;
//		return;
//	}
//
//	auto ind = lightingTranslator->translateIndicator(input);
//	if (ind != this->indicator)
//	{
//		this->indicator = ind;
//		this->indicatorTime = 0;
//		this->indicatorOn = false;
//	}
//
//	switch (this->indicator)
//	{
//	case Indicator::Left:
//		setting.lightSetting.indicatorRightIntensity = LOW;
//		setting.lightSetting.indicatorLeftIntensity = this->indicatorToggle(this->config.lightModeConfig.indicatorTime);
//		break;
//	case Indicator::Right:
//		setting.lightSetting.indicatorLeftIntensity = LOW;
//		setting.lightSetting.indicatorRightIntensity = this->indicatorToggle(this->config.lightModeConfig.indicatorTime);
//		break;
//	case Indicator::Off:
//		setting.lightSetting.indicatorLeftIntensity = LOW;
//		setting.lightSetting.indicatorRightIntensity = LOW;
//		break;
//	}
//}
//
//// Translate the light mode based on the switch setting
//void Control::translateLightMode(int input)
//{
//	if (mode == Mode::Park)
//		return;
//
//	lightMode = lightingTranslator->translateLightMode(input, lightMode);
//
//	switch (lightMode)
//	{
//	case LightMode::Off:
//		setting.lightSetting.headLightIntensity = 0;
//		setting.lightSetting.frontFogIntensity = LOW;
//		setting.lightSetting.rearFogIntensity = LOW;
//		setting.lightSetting.roofLightIntensity = LOW;
//		break;
//
//	case LightMode::Sidelights:
//		setting.lightSetting.headLightIntensity = config.lightModeConfig.headlightMin;
//		setting.lightSetting.frontFogIntensity = LOW;
//		setting.lightSetting.rearFogIntensity = LOW;
//		setting.lightSetting.roofLightIntensity = LOW;
//		break;
//
//	case LightMode::Headlights:
//		setting.lightSetting.headLightIntensity = config.lightModeConfig.headlightMed;
//		setting.lightSetting.frontFogIntensity = LOW;
//		setting.lightSetting.rearFogIntensity = LOW;
//		setting.lightSetting.roofLightIntensity = HIGH;
//		break;
//
//	case LightMode::Foglights:
//		setting.lightSetting.headLightIntensity = config.lightModeConfig.headlightMed;
//		setting.lightSetting.frontFogIntensity = HIGH;
//		setting.lightSetting.rearFogIntensity = HIGH;
//		setting.lightSetting.roofLightIntensity = HIGH;
//		break;
//	}
//}
//
//// Translates the main beam (flash or on/off toggle depending on headlight setting)
//void Control::translateMainBeam(int input)
//{
//	if (mode == Mode::Park)
//		return;
//
//	mainBeam = lightingTranslator->translateMainBeam(input, lightMode, mainBeam);
//
//	switch (mainBeam)
//	{
//	case MainBeam::Flash:
//	case MainBeam::On:
//		setting.lightSetting.headLightIntensity = config.lightModeConfig.headlightMax;
//		break;
//	}
//}
//
//// Translates the brake light (on, off, sidelight depending on stick / headlight setting)
//void Control::translateBrakeLight(int input)
//{
//	if (mode == Mode::Park)
//		return;
//
//	switch (lightingTranslator->translateBrakeLight(input, lightMode))
//	{
//	case BrakeLight::Off:
//		setting.lightSetting.brakeLightIntensity = 0;
//		break;
//
//	case BrakeLight::Braking:
//		setting.lightSetting.brakeLightIntensity = config.lightModeConfig.brakeMax;
//		break;
//
//	case BrakeLight::Sidelight:
//		setting.lightSetting.brakeLightIntensity = config.lightModeConfig.brakeMed;
//		break;
//	};
//}
//
//// Translates the reverse light setting based on mode and gear
//void Control::translateReverseLight()
//{
//	if (mode != Mode::Drive)
//	{
//		setting.lightSetting.reverseLightIntensity = LOW;
//		return;
//	}
//
//	setting.lightSetting.reverseLightIntensity = lightingTranslator->translateReverseLight(gear) ? HIGH : LOW;
//}
//
//int Control::indicatorToggle(int delay)
//{
//	unsigned long timeNow = millis();
//
//	// This does not do accurate timing.  We are assuming that the Arduino code will loop fast enough
//	// that a slight delay in switching the indicator will not be noticeable, but if we happen to
//	// hold up the Arduino loop elsewhere we will get an indicator stuck on or off.
//	// Worse still, we might cause the throttle and steering to stick open.
//	if (this->indicatorTime + delay <= timeNow)
//	{
//		this->indicatorTime = timeNow;
//		this->indicatorOn = !this->indicatorOn;
//	}
//
//	return this->indicatorOn ? HIGH : LOW;
//
//	//// If indicators were off, start by turning on
//	//if (this->indicatorTime == 0)
//	//{
//	//	this->indicatorOn = true;
//	//	return HIGH;
//	//}
//}
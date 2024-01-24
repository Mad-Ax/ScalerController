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
}

void Control::translate(InputSetting input)
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

//	// Get the requested mode
//	translateMode(input.channel[config.mode.channel]);
//
//	// Get the requested gear
//	translateGear(input.channel[config.gear.channel]);

	// Get the requested motor speed
	translateMotorSpeed(input.channel[config.throttleChannel.channel]);

	// Get the steering servo position
	translateSteering(input.channel[config.steeringChannel.channel]);

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

//// Translates the requested mode
//void Control::translateMode(int input)
//{
//	mode = controlTranslator->translateMode(input);
//}
//
//// Translates the requested gear with the current mode to determine the selected gear
//void Control::translateGear(int input)
//{
//	switch (mode)
//	{
//	case Mode::Off:
//	case Mode::Park:
//		gear = Gear::Neutral;
//		return;
//	case Mode::Drive:
//		gear = controlTranslator->translateGear(input);
//		return;
//	}
//
//	gear = Gear::Neutral;
//	return;
//}

// Translates the input into a motor speed
void Control::translateMotorSpeed(int input)
{
	setting.motorSpeed = input;
//	switch (this->mode)
//	{
//	case Mode::Off:
//	case Mode::Park:
//		setting.motorSpeed = config.throttleServo.center;
//		return;
//
//	case Mode::Drive:
//		if (input < config.eBrakeThreshold)
//		{
//			// E-brake applied - use drag brake to stop truck immediately regardless of gear
//			setting.motorSpeed = config.throttleServo.center;
//			return;
//		}
//
//		setting.motorSpeed = controlTranslator->translateMotorSpeed(setting.motorSpeed, input, gear);
//		return;
//	}
//
//	// Default response if no other conditions met - apply drag brake
//	this->setting.motorSpeed = this->config.throttleServo.center;
}

// Translates the steering position input to desired output depending on mode
void Control::translateSteering(int input)
{
	setting.steering = input;
	//setting.steering = controlTranslator->translateSteering(input);
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
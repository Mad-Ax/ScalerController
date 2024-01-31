 
/*
	Name:       ScalerController.ino
	Created:	January 2024
	Author:     Alex Sparey
*/

#include "defines.h"
#include "input.h"
#include "control.h"
#include "output.h"
#include "structs.h"
#include "ppmwrapper.h"
#include "controltranslator.h"
#include "inputtranslator.h"
#include "lightingtranslator.h"
#include "inertia.h"
#include "motorspeedtranslator.h"
#include "latchtranslator.h"
#include "outputservo.h"
#include "outputlights.h"

// Global pointers
Input* input;
Control* control;
Output* output;

unsigned long loopTime;

void setup()
{
	InputConfig inputConfig = {
		PPM_IN,
		PPM_CHANNELS,
		PPM_INPUT_MAX_ERROR,
		PPM_BLANK_TIME,
		PPM_MIN_CHANNEL_VALUE,
		PPM_MAX_CHANNEL_VALUE
	};
	PpmWrapper* ppmWrapper = new PpmWrapper(inputConfig);

	input = new Input(inputConfig, ppmWrapper);

	LatchChannel gearChannel = {
		CHN_GEAR_CH - 1,
		CHN_GEAR_SELECT_MIN,
		CHN_GEAR_SELECT_MAX
	};

	LatchChannel cruiseChannel = {
		CHN_CRUISE_CH - 1,
		CHN_CRUISE_SELECT_MIN,
		CHN_CRUISE_SELECT_MAX
	};

	ThrottleChannel throttleChannel {
		CHN_THROTTLE_CH - 1,
		CHN_THROTTLE_MIN,
		CHN_THROTTLE_MAX,
		CHN_THROTTLE_DEADBAND_MIN,
		CHN_THROTTLE_DEADBAND_MAX,
		CHN_THROTTLE_EBRAKE_THRESHOLD
	};

	AnalogChannel steeringChannel = {
		CHN_STEERING_CH - 1,
		CHN_STEERING_MIN,
		CHN_STEERING_MAX,
		CHN_STEERING_DEADBAND_MIN,
		CHN_STEERING_DEADBAND_MAX
	};

	LatchChannel lightsOnChannel{
		CHN_LIGHTS - 1,
		CHN_LIGHTS_ON_SELECT_MIN,
		CHN_LIGHTS_ON_SELECT_MAX
	};

	LatchChannel lightsOffChannel{
		CHN_LIGHTS - 1,
		CHN_LIGHTS_OFF_SELECT_MIN,
		CHN_LIGHTS_OFF_SELECT_MAX
	};

	ServoConfig throttleServo = {
		SVO_ESC_PIN,
		SVO_ESC_CENTER,
		SVO_ESC_MIN,
		SVO_ESC_MAX
	};

	ServoConfig steeringServo = {
		SVO_STEERING_PIN,
		SVO_STEERING_CENTER,
		SVO_STEERING_MIN,
		SVO_STEERING_MAX
	};

	LightModeConfig lightModeConfig = {
		BRAKE_MAX_PWM,
		BRAKE_LOW_PWM,
		REVERSE_PWM,
		HEADLIGHT_MAX_PWM,
		HEADLIGHT_LOW_PWM,
		ROOFLIGHT_MAX_PWM,
		ROOFLIGHT_LOW_PWM
//		FAILSAFE_FLASH_TIME,
	};

	ControlConfig controlConfig = {
		MAX_FAILSAFE_COUNT,
//		mode,
		gearChannel,
		cruiseChannel,
		throttleChannel,
		steeringChannel,
		lightsOnChannel,
		lightsOffChannel,
//		indicator,
//		lights,
		throttleServo,
		steeringServo,
		lightModeConfig,
//		SWITCH_HIGH,
//		SWITCH_LOW,
		FWD_ACCEL_INERTIA,
		FWD_DECEL_INERTIA,
		FWD_BRAKE_INERTIA,
		REV_ACCEL_INERTIA,
		REV_DECEL_INERTIA,
		REV_BRAKE_INERTIA,
	};

	IInputTranslator* inputTranslator = new InputTranslator();
	IMotorSpeedTranslator* motorSpeedTranslator = new MotorSpeedTranslator(throttleChannel, throttleServo);
	ILatchTranslator* gearTranslator = new LatchTranslator();
	ILatchTranslator* cruiseTranslator = new LatchTranslator(); // TODO: M: does it make sense for these to take channel as a parm?
	IInertia* forwardAccel = new Inertia(controlConfig.fwdAccelInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
	IInertia* forwardDecel = new Inertia(controlConfig.fwdDecelInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
	IInertia* forwardBrake = new Inertia(controlConfig.fwdBrakeInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
	IInertia* reverseAccel = new Inertia(controlConfig.revAccelInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
	IInertia* reverseDecel = new Inertia(controlConfig.revDecelInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
	IInertia* reverseBrake = new Inertia(controlConfig.revBrakeInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
	IControlTranslator* controlTranslator = new ControlTranslator(
		controlConfig,
		inputTranslator,
		motorSpeedTranslator,
		gearTranslator,
		cruiseTranslator,
		forwardAccel,
		forwardDecel,
		forwardBrake,
		reverseAccel,
		reverseDecel,
		reverseBrake);

	ILatchTranslator* lightsOnTranslator = new LatchTranslator();
	ILatchTranslator* lightsOffTranslator = new LatchTranslator();
	LightingTranslator* lightingTranslator = new LightingTranslator(controlConfig, lightsOnTranslator, lightsOffTranslator);

	control = new Control(controlTranslator, lightingTranslator, controlConfig);

	LightOutputConfig lightOutputConfig = {
		HEADLIGHT_OUT,
		BRAKELIGHT_OUT,
		REVERSE_OUT,
		ROOFLIGHT_OUT,
//		FAILSAFE_OUT
	};

	IOutputServo* outputEsc = new OutputServo(throttleServo);
	IOutputServo* outputSteering = new OutputServo(steeringServo);
//	IOutputLights* outputLights = CHN_LIGHTS > 0 ? new OutputLights(lightModeConfig, lightOutputConfig) : nullptr;
	IOutputLights* outputLights = new OutputLights(lightModeConfig, lightOutputConfig);

	output = new Output(outputEsc, outputSteering, outputLights);

	// Open the serial (for debugging)
	Serial.begin(57600);
}


void loop()
{
	// get the latest input values from the receiver
	input->update();

	//for (int channel = 0; channel < 8; channel++)
	//{
	//	Serial.print(input->setting.channel[channel]);
	//	Serial.print(':');
	//}
	//Serial.println();

	// translate the input values to control values that can be sent to the outputs
	control->translate(input->setting, Serial);

	// pass the translated values to the output
	output->send(control->setting);

	// close out any serial lines
	//Serial.println();

	// Perform the master delay
	while (loopTime + MASTER_DELAY > micros())
	{
	}
	loopTime = micros();
}

 
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
//#include "lightingtranslator.h"
#include "inertia.h"
#include "motorspeedtranslator.h"
#include "outputservo.h"
//#include "outputlights.h"

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

//	ChannelConfig mode = {
//		CHN_MODE_CH - 1
//	};
//
//	ChannelConfig gear = {
//		CHN_GEAR - 1
//	};

	LatchChannel gearChannel = {
		CHN_GEAR_CH - 1,
		CHN_GEAR_SELECT_MIN,
		CHN_GEAR_SELECT_MAX
	};

	AnalogChannel throttleChannel = {
		CHN_THROTTLE_CH - 1,
		CHN_THROTTLE_MIN,
		CHN_THROTTLE_MAX,
		CHN_THROTTLE_DEADBAND_MIN,
		CHN_THROTTLE_DEADBAND_MAX
	};

	AnalogChannel steeringChannel = {
		CHN_STEERING_CH - 1,
		CHN_STEERING_MIN,
		CHN_STEERING_MAX,
		CHN_STEERING_DEADBAND_MIN,
		CHN_STEERING_DEADBAND_MAX
	};

//	ChannelConfig indicator = {
//		CHN_INDICATOR - 1
//	};
//
//	ChannelConfig lights = {
//		CHN_LIGHTS - 1
//	};

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

//	LightModeConfig lightModeConfig = {
//		CHN_LIGHTS > 0,
//		INDICATOR_TIME,
//		BRAKE_MAX_PWM,
//		BRAKE_MED_PWM,
//		HEADLIGHT_MAX_PWM,
//		HEADLIGHT_MED_PWM,
//		HEADLIGHT_MIN_PWM,
//		FAILSAFE_FLASH_TIME,
//	};

	ControlConfig controlConfig = {
		MAX_FAILSAFE_COUNT,
//		mode,
		gearChannel,
		throttleChannel,
		steeringChannel,
//		indicator,
//		lights,
		throttleServo,
		steeringServo,
//		lightModeConfig,
//		SWITCH_HIGH,
//		SWITCH_LOW,
		FWD_ACCEL_INERTIA,
		FWD_DECEL_INERTIA,
		FWD_BRAKE_INERTIA,
		REV_ACCEL_INERTIA,
		REV_DECEL_INERTIA,
		REV_BRAKE_INERTIA,
//		E_BRAKE_THRESHOLD
	};

	IInputTranslator* inputTranslator = new InputTranslator();
	IMotorSpeedTranslator* motorSpeedTranslator = new MotorSpeedTranslator(throttleChannel, throttleServo);
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
		forwardAccel,
		forwardDecel,
		forwardBrake,
		reverseAccel,
		reverseDecel,
		reverseBrake);
//	ILightingTranslator* lightingTranslator = new LightingTranslator(inputTranslator, controlConfig);
//
//	control = new Control(controlTranslator, lightingTranslator, controlConfig);
	control = new Control(controlTranslator, controlConfig);
//
//	LightOutputConfig lightOutputConfig = {
//		INDICATOR_L_OUT,
//		INDICATOR_R_OUT,
//		BRAKELIGHT_OUT,
//		HEADLIGHT_OUT,
//		FOGLIGHT_F_OUT,
//		FOGLIGHT_R_OUT,
//		ROOFLIGHT_OUT,
//		REVERSE_OUT,
//		FAILSAFE_OUT
//	};

	IOutputServo* outputEsc = new OutputServo(throttleServo);
	IOutputServo* outputSteering = new OutputServo(steeringServo);
//	IOutputLights* outputLights = CHN_LIGHTS > 0 ? new OutputLights(lightModeConfig, lightOutputConfig) : nullptr;

//	output = new Output(outputEsc, outputSteering, outputLights);
	output = new Output(outputEsc, outputSteering);

	// Open the serial (for debugging)
	Serial.begin(57600);
}


void loop()
{
	// get the latest input values from the receiver
	input->update();

	for (int channel = 0; channel < 8; channel++)
	{
		Serial.print(input->setting.channel[channel]);
		Serial.print(':');
	}
	Serial.println();

	// translate the input values to control values that can be sent to the outputs
	control->translate(input->setting, Serial);

	// pass the translated values to the output
	output->send(control->setting);

	// close out any serial lines
	Serial.println();

	// Perform the master delay
	while (loopTime + MASTER_DELAY > micros())
	{
	}
	loopTime = micros();
}

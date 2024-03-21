 
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
#include "inputtranslator.h" // TODO: M: remove
#include "steeringtranslator.h"
#include "lightingtranslator.h"
#include "inertia.h"
#include "motorspeedtranslator.h"
#include "latchtranslator.h"
#include "outputservo.h"
#include "outputlights.h"
#include "switchtranslatortwoway.h"
#include "switchtranslatorthreeway.h"

// Global pointers
Input* input;
Control* control;
Output* output;

unsigned long loopTime;

void setup()
{
	// Open the serial (for debugging)
	Serial.begin(57600);

	InputConfig inputConfig = {
		PPM_IN,
		PPM_CHANNELS,
		PPM_INPUT_MAX_ERROR,
		PPM_BLANK_TIME,
		PPM_MIN_CHANNEL_VALUE,
		PPM_MAX_CHANNEL_VALUE,
		MODE_PIN
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

	AnalogChannel winchOperationChannel = {
		CHN_WINCH_OPERATE - 1,
		CHN_MIN,
		CHN_MAX,
		CHN_DEADBAND_MIN,
		CHN_DEADBAND_MAX
	};

	LatchChannel lightsOnChannel {
		CHN_LIGHTS - 1,
		CHN_LIGHTS_ON_SELECT_MIN,
		CHN_LIGHTS_ON_SELECT_MAX
	};

	LatchChannel lightsOffChannel {
		CHN_LIGHTS - 1,
		CHN_LIGHTS_OFF_SELECT_MIN,
		CHN_LIGHTS_OFF_SELECT_MAX
	};

	SwitchChannelTwoWay* floodlightChannel = new SwitchChannelTwoWay(); // TODO: make ctor?
	floodlightChannel->channel = CHN_FLOOD_CH - 1;
	floodlightChannel->high = SWITCH_HIGH;
	//SwitchChannelTwoWay floodlightChannel
	//{
	//	CHN_FLOOD_CH - 1,
	//	SWITCH_HIGH
	//};

	SwitchChannelThreeWay winchSelectChannel{
		CHN_WINCH_SELECT - 1,
		SWITCH_HIGH,
		SWITCH_LOW
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

	ServoConfig winch1Servo = {
		SVO_WINCH_1_PIN,
		SVO_CENTER,
		SVO_MIN,
		SVO_MAX
	};

	ServoConfig winch2Servo = {
		SVO_WINCH_2_PIN,
		SVO_CENTER,
		SVO_MIN,
		SVO_MAX
	};

	LightModeConfig lightModeConfig = {
		BRAKE_MAX_PWM,
		BRAKE_LOW_PWM,
		REVERSE_PWM,
		HEADLIGHT_MAX_PWM,
		HEADLIGHT_LOW_PWM,
		ROOFLIGHT_MAX_PWM,
		ROOFLIGHT_LOW_PWM,
		FLOODLIGHT_PWM,
	};

	ControlConfig controlConfig = {
		MAX_FAILSAFE_COUNT,
		gearChannel,
		cruiseChannel,
		throttleChannel,
		steeringChannel,
		winchOperationChannel,
		lightsOnChannel,
		lightsOffChannel,
		floodlightChannel,
		winchSelectChannel,
		throttleServo,
		steeringServo,
		winch1Servo,
		winch2Servo,
		lightModeConfig,
		FWD_ACCEL_INERTIA,
		FWD_DECEL_INERTIA,
		FWD_BRAKE_INERTIA,
		REV_ACCEL_INERTIA,
		REV_DECEL_INERTIA,
		REV_BRAKE_INERTIA,
		STEERING_INERTIA
	};

	IInputTranslator* inputTranslator = new InputTranslator();// TODO: remove when not noeded

	const IInertia* steeringInertia = new Inertia(controlConfig.steeringInertia, controlConfig.steeringServo.min, controlConfig.steeringServo.max);
	const ISteeringTranslator* steeringTranslator = new SteeringTranslator(*steeringInertia);

	IMotorSpeedTranslator* motorSpeedTranslator = new MotorSpeedTranslator(throttleChannel, throttleServo);
	ILatchTranslator* gearTranslator = new LatchTranslator(gearChannel);
	ILatchTranslator* cruiseTranslator = new LatchTranslator(cruiseChannel);
	ISwitchTranslatorThreeWay* winchSelectTranslator = new SwitchTranslatorThreeWay(winchSelectChannel);
	IInertia* forwardAccel = new Inertia(controlConfig.fwdAccelInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
	IInertia* forwardDecel = new Inertia(controlConfig.fwdDecelInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
	IInertia* forwardBrake = new Inertia(controlConfig.fwdBrakeInertia, controlConfig.throttleServo.center, controlConfig.throttleServo.max);
	IInertia* reverseAccel = new Inertia(controlConfig.revAccelInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
	IInertia* reverseDecel = new Inertia(controlConfig.revDecelInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
	IInertia* reverseBrake = new Inertia(controlConfig.revBrakeInertia, controlConfig.throttleServo.min, controlConfig.throttleServo.center);
	IControlTranslator* controlTranslator = new ControlTranslator(
		controlConfig,
		inputTranslator,
		*steeringTranslator,
		motorSpeedTranslator,
		gearTranslator,
		cruiseTranslator,
		winchSelectTranslator,
		forwardAccel,
		forwardDecel,
		forwardBrake,
		reverseAccel,
		reverseDecel,
		reverseBrake);

	ILatchTranslator* lightsOnTranslator = new LatchTranslator(lightsOnChannel);
	ILatchTranslator* lightsOffTranslator = new LatchTranslator(lightsOffChannel);
	ISwitchTranslatorTwoWay* floodlightTranslator = new SwitchTranslatorTwoWay(floodlightChannel);
	LightingTranslator* lightingTranslator = new LightingTranslator(controlConfig, lightsOnTranslator, lightsOffTranslator, floodlightTranslator);

	control = new Control(controlTranslator, lightingTranslator, controlConfig);

	LightOutputConfig lightOutputConfig = {
		HEADLIGHT_OUT,
		BRAKELIGHT_OUT,
		REVERSE_OUT,
		ROOFLIGHT_OUT,
		FLOODLIGHT_OUT
	};

	IOutputServo* outputEsc = new OutputServo(throttleServo);
	IOutputServo* outputSteering = new OutputServo(steeringServo);
	IOutputServo* outputWinch1 = new OutputServo(winch1Servo);
	IOutputServo* outputWinch2 = new OutputServo(winch2Servo);
	IOutputLights* outputLights = new OutputLights(lightModeConfig, lightOutputConfig);

	output = new Output(outputEsc, outputSteering, outputWinch1, outputWinch2, outputLights);
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
	//Serial.println();

	// Perform the master delay
	while (loopTime + MASTER_DELAY > micros())
	{
	}
	loopTime = micros();
}

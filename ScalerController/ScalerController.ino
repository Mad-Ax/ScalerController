 
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
#include "steeringtranslator.h"
#include "lightingtranslator.h"
#include "inertia.h"
#include "motorspeedtranslator.h"
#include "latchtranslator.h"
#include "outputservo.h"
#include "outputlights.h"
#include "switchtranslatortwoway.h"
#include "switchtranslatorthreeway.h"

// Global structs - for config

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

ThrottleChannel throttleChannel{
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

SwitchChannelTwoWay floodlightChannel
{
	CHN_FLOOD_CH - 1,
	SWITCH_HIGH
};

SwitchChannelThreeWay winchSelectChannel{
	CHN_WINCH_SELECT - 1,
	SWITCH_HIGH,
	SWITCH_LOW
};

const ServoConfig& throttleServo = *new ServoConfig(
	SVO_ESC_PIN,
	SVO_ESC_CENTER,
	SVO_ESC_MIN,
	SVO_ESC_MAX
);

const ServoConfig& steeringServo = *new ServoConfig(
	SVO_STEERING_PIN,
	SVO_STEERING_CENTER,
	SVO_STEERING_MIN,
	SVO_STEERING_MAX
);

const ServoConfig& winch1Servo = *new ServoConfig(
	SVO_WINCH_1_PIN,
	SVO_CENTER,
	SVO_MIN,
	SVO_MAX
);

const ServoConfig& winch2Servo = *new ServoConfig(
	SVO_WINCH_2_PIN,
	SVO_CENTER,
	SVO_MIN,
	SVO_MAX
);

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

ControlConfig& controlConfig = *new ControlConfig(
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
	lightModeConfig
);

LightOutputConfig lightOutputConfig = {
	HEADLIGHT_OUT,
	BRAKELIGHT_OUT,
	REVERSE_OUT,
	ROOFLIGHT_OUT,
	FLOODLIGHT_OUT
};

// Global pointers - put here so they are not destroyed at end of setup()
Input* input;
Control* control;
Output* output;

const IPpmWrapper* ppmWrapper;

const IInertia* steeringInertia;
const ISteeringTranslator* steeringTranslator;

const IInertia* forwardAccel;
const IInertia* forwardDecel;
const IInertia* forwardBrake;
const IMotorSpeedTranslator* forwardMotorSpeedTranslator;

const IInertia* reverseAccel;
const IInertia* reverseDecel;
const IInertia* reverseBrake;
const IMotorSpeedTranslator* reverseMotorSpeedTranslator;

ILatchTranslator* gearTranslator;
ILatchTranslator* cruiseTranslator;
const ISwitchTranslatorThreeWay* winchSelectTranslator;

IControlTranslator* controlTranslator;

ILatchTranslator* lightsOnTranslator;
ILatchTranslator* lightsOffTranslator;
ISwitchTranslatorTwoWay* floodlightTranslator;
ILightingTranslator* lightingTranslator;

IOutputServo* outputEsc;
IOutputServo* outputSteering;
IOutputServo* outputWinch1;
IOutputServo* outputWinch2;
IOutputLights* outputLights;

unsigned long loopTime;

void setup()
{
	// Open the serial (for debugging)
	Serial.begin(57600);

	ppmWrapper = new PpmWrapper(PPM_IN, PPM_CHANNEL_COUNT, PPM_INPUT_MAX_ERROR, PPM_BLANK_TIME, PPM_MIN_CHANNEL_VALUE, PPM_MAX_CHANNEL_VALUE);
	
	input = new Input(10, PPM_CHANNEL_COUNT, ppmWrapper);

	steeringInertia = new Inertia(STEERING_INERTIA, SVO_STEERING_MIN, SVO_STEERING_MAX, Serial);
	steeringTranslator = new SteeringTranslator(steeringInertia);

	forwardAccel = new Inertia(FWD_ACCEL_INERTIA, SVO_ESC_CENTER, SVO_ESC_MAX, Serial);
	forwardDecel = new Inertia(FWD_DECEL_INERTIA, SVO_ESC_CENTER, SVO_ESC_MAX, Serial);
	forwardBrake = new Inertia(FWD_BRAKE_INERTIA, SVO_ESC_CENTER, SVO_ESC_MAX, Serial);
	forwardMotorSpeedTranslator = new MotorSpeedTranslator(throttleChannel, *forwardAccel, *forwardDecel, *forwardBrake);

	reverseAccel = new Inertia(REV_ACCEL_INERTIA, SVO_ESC_MIN, SVO_ESC_CENTER, Serial);
	reverseDecel = new Inertia(REV_DECEL_INERTIA, SVO_ESC_MIN, SVO_ESC_CENTER, Serial);
	reverseBrake = new Inertia(REV_BRAKE_INERTIA, SVO_ESC_MIN, SVO_ESC_CENTER, Serial);
	reverseMotorSpeedTranslator = new MotorSpeedTranslator(throttleChannel, *reverseAccel, *reverseDecel, *reverseBrake);

	gearTranslator = new LatchTranslator(gearChannel);
	cruiseTranslator = new LatchTranslator(cruiseChannel);
	winchSelectTranslator = new SwitchTranslatorThreeWay(winchSelectChannel);

	controlTranslator = new ControlTranslator(
		controlConfig,
		SVO_ESC_CENTER,
		SVO_STEERING_PIN,
		SVO_STEERING_CENTER,
		SVO_STEERING_MIN,
		SVO_STEERING_MAX,
		*steeringTranslator,
		*forwardMotorSpeedTranslator,
		*reverseMotorSpeedTranslator,
		*gearTranslator,
		*cruiseTranslator,
		*winchSelectTranslator);

	lightsOnTranslator = new LatchTranslator(lightsOnChannel);
	lightsOffTranslator = new LatchTranslator(lightsOffChannel);
	floodlightTranslator = new SwitchTranslatorTwoWay(floodlightChannel);
	lightingTranslator = new LightingTranslator(controlConfig, *lightsOnTranslator, *lightsOffTranslator, *floodlightTranslator);

	control = new Control(SVO_ESC_CENTER, *controlTranslator, *lightingTranslator, controlConfig);
	
	outputEsc = new OutputServo(throttleServo);
	outputSteering = new OutputServo(steeringServo);
	outputWinch1 = new OutputServo(winch1Servo);
	outputWinch2 = new OutputServo(winch2Servo);
	outputLights = new OutputLights(lightOutputConfig);

	output = new Output(*outputEsc, *outputSteering, *outputWinch1, *outputWinch2, *outputLights);
}


void loop()
{
	// get the latest input values from the receiver
	input->update(Serial);

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




#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char* __brkval;
#endif  // __arm__

int freeMemory() {
	char top;
#ifdef __arm__
	return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
	return &top - __brkval;
#else  // __arm__
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
#pragma once

//#include "control.h"
#include "ioutputservo.h"
#include "ioutputhbridge.h"
#include "structs.h"
#include "ioutputlights.h"

// Output class
// sends control data to Arduino outputs
class Output
{
  private: 
    IOutputServo* outputEsc;
    IOutputServo* outputSteering;
	IOutputServo* outputAux1;
	IOutputServo* outputWinch1Servo;
	IOutputServo* outputWinch2Servo;
	IOutputHbridge* outputWinch1Hbridge;
	IOutputHbridge* outputWinch2Hbridge;
    IOutputLights* outputLights;

	// initialises the class with config values
public: Output(
	IOutputServo* outputEsc,
	IOutputServo* outputSteering,
	IOutputServo* outputAux1,
	IOutputServo* outputWinch1Servo,
	IOutputServo* outputWinch2Servo,
	IOutputHbridge* outputWinch1Hbridge,
	IOutputHbridge* outputWinch2Hbridge,
	IOutputLights* outputLights);

  // Sends the current control settings to the output pins
  public: void send(ControlSetting control);
};
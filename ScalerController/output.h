#pragma once

//#include "control.h"
#include "ioutputservo.h"
#include "structs.h"
#include "ioutputlights.h"

// Output class
// sends control data to Arduino outputs
class Output
{
  private: 
    IOutputServo& outputEsc;
    IOutputServo& outputSteering;
	IOutputServo& outputWinch1;
	IOutputServo& outputWinch2;
    IOutputLights& outputLights;

	// initialises the class with config values
public: Output(
	IOutputServo& outputEsc,
	IOutputServo& outputSteering,
	IOutputServo& outputWinch1,
	IOutputServo& outputWinch2,
	IOutputLights& outputLights);

  // Sends the current control settings to the output pins
  public: void send(ControlSetting& control);
};
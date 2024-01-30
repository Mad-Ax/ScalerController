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
    IOutputServo* outputEsc;
    IOutputServo* outputSteering;
    IOutputLights* outputLights;

	// initialises the class with config values
public: Output(
	IOutputServo* outputEsc,
	IOutputServo* outputSteering,
	IOutputLights* outputLights);

  // Sends the current control settings to the output pins
  public: void send(ControlSetting control);
};
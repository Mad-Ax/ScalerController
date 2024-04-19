#include "output.h"

Output::Output(
	IOutputServo& outputEsc, 
	IOutputServo& outputSteering,
	IOutputServo& outputWinch1,
	IOutputServo& outputWinch2,
	IOutputLights& outputLights) :
outputEsc(outputEsc),
outputSteering(outputSteering),
outputWinch1(outputWinch1),
outputWinch2(outputWinch2),
outputLights(outputLights)
{}

void Output::send(ControlSetting& control)
{
	// output motor control
	this->outputEsc.send(control.motorSpeed);

	// output steering control
	this->outputSteering.send(control.steering);

	// output winch 1
	this->outputWinch1.send(control.winch1);

	// output winch 2
	this->outputWinch2.send(control.winch2);

	// output lights
	this->outputLights.send(control.lightSetting);
}

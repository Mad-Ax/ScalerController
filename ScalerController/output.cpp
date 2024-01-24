#include "output.h"

Output::Output(
	IOutputServo* outputEsc, 
	IOutputServo* outputSteering)
//	IOutputLights* outputLights)
{
	this->outputEsc = outputEsc;
	this->outputSteering = outputSteering;
//	this->outputLights = outputLights;
}

void Output::send(ControlSetting control)
{
	// output motor control
	this->outputEsc->send(control.motorSpeed);

	// output steering control
	this->outputSteering->send(control.steering);

//	// output lights
//	if (this->outputLights != nullptr)
//		this->outputLights->send(control.lightSetting);
}

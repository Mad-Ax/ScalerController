#include "output.h"

Output::Output(
	IOutputServo* outputEsc, 
	IOutputServo* outputSteering,
	IOutputServo* outputAux1,
	IOutputLights* outputLights)
{
	this->outputEsc = outputEsc;
	this->outputSteering = outputSteering;
	this->outputAux1 = outputAux1;
	this->outputLights = outputLights;
}

void Output::send(ControlSetting control)
{
	// output motor control
	this->outputEsc->send(control.motorSpeed);

	// output steering control
	this->outputSteering->send(control.steering);

	// output aux 1 control
	this->outputAux1->send(control.aux1);

	// output lights
	this->outputLights->send(control.lightSetting);
}

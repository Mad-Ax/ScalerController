#include "output.h"

Output::Output(
	IOutputServo* outputEsc, 
	IOutputServo* outputSteering,
	IOutputServo* outputAux1,
	IOutputServo* outputWinch1Servo,
	IOutputServo* outputWinch2Servo,
	IOutputHbridge* outputWinch1Hbridge,
	IOutputHbridge* outputWinch2Hbridge,
	IOutputLights* outputLights)
{
	this->outputEsc = outputEsc;
	this->outputSteering = outputSteering;
	this->outputAux1 = outputAux1;
	this->outputWinch1Servo = outputWinch1Servo;
	this->outputWinch2Servo = outputWinch2Servo;
	this->outputWinch1Hbridge = outputWinch1Hbridge;
	this->outputWinch2Hbridge = outputWinch2Hbridge;
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

	// output winch 1
	this->outputWinch1Servo->send(control.winch1);
	this->outputWinch1Hbridge->send(control.winch1);

	// output winch 2
	this->outputWinch2Servo->send(control.winch2);
	this->outputWinch2Hbridge->send(control.winch2);

	// output lights
	this->outputLights->send(control.lightSetting);
}

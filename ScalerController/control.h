#pragma once

#include "icontroltranslator.h"
#include "ilightingtranslator.h"
#include "HardwareSerial.h"

// Control class
// Translates raw inputted values from the transmitter to output values that will be sent to the outputs
class Control
{
public:
	ControlSetting setting;

	// Constructor
	Control(
		const int& throttleServoCenter,
		IControlTranslator& controlTranslator,
		ILightingTranslator& lightingTranslator,
		ControlConfig& controlConfig);//TODO: remove config when not needed

	// Translate method
	// Translates the requested Input object into a set of values that can be sent to the Output
	void translate(const InputSetting& input, HardwareSerial& ser);

private:
	const int& throttleServoCenter;
	IControlTranslator& controlTranslator;
	ILightingTranslator& lightingTranslator;
	ControlConfig& config;//TODO: remove config when not needed
};

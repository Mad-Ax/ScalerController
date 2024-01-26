#pragma once

#include "iinputtranslator.h"

class InputTranslator : public IInputTranslator
{
public:
	InputTranslator();
	~InputTranslator();

//	// Translates a 3-way switch value into 3 possible output values
//	ThreeWayPosition translateThreeWaySwitch(int input, int switchHigh, int switchLow);

	// Translates an analog stick input to a servo output
	int translateStickInput(int input, AnalogChannel channel, ServoConfig servo);

//	//TODO: tidy this
//private:
//	//int failsafeCount;
//	//InputSetting* lastInput = nullptr;
//	ControlConfig config;
};

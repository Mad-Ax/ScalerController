#include "inputTranslator.h"
#include "Arduino.h"

InputTranslator::InputTranslator()
{
}

InputTranslator::~InputTranslator()
{
}

//// Translates a 3-way switch value into 3 possible output values
//ThreeWayPosition InputTranslator::translateThreeWaySwitch(int input, int switchHigh, int switchLow)
//{
//	if (input >= switchHigh)
//	{
//		return ThreeWayPosition::PosA;
//	}
//	else if (input <= switchLow)
//	{
//		return ThreeWayPosition::PosC;
//	}
//
//	return ThreeWayPosition::PosB;
//}

// Translates an analog stick input to a servo output
int InputTranslator::translateStickInput(int input, AnalogChannel channel, ServoConfig servo)
{
	// If stick is within deadband, return servo center position
	if (input >= channel.dbMin && input <= channel.dbMax)
		return servo.center;

	// Correct any incorrect values
	if (input < channel.min) input = channel.min;
	if (input > channel.max) input = channel.max;

	// If input is less than deadzone, map to value below deadzone
	if (input < channel.dbMin) {
		return map(input, channel.min, channel.dbMin, servo.min, servo.center);
	}

	// If input is greater than deadzone, map to value above deadzone
	return map(input, channel.dbMax, channel.max, servo.center, servo.max);
}
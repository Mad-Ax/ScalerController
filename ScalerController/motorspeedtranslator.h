//#pragma once
//
//#include "imotorspeedtranslator.h"
//#include "structs.h"
//
//class MotorSpeedTranslator : public IMotorSpeedTranslator
//{
//public:
//	MotorSpeedTranslator(ChannelConfig channel, ServoConfig servo);
//	~MotorSpeedTranslator();
//
//	// Translates the input into a motor speed
//	int translateMotorSpeed(int currentMotorSpeed, int input, int desiredMotorSpeed, IInertia* accel, IInertia* decel, IInertia* brake);
//
//private:
//	ChannelConfig channel;
//	ServoConfig servo;
//};
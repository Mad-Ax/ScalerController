//#include "motorspeedtranslator.h"
//
//MotorSpeedTranslator::MotorSpeedTranslator(ChannelConfig channel, ServoConfig servo)
//{
//	this->channel = channel;
//	this->servo = servo;
//}
//
//MotorSpeedTranslator::~MotorSpeedTranslator()
//{
//}
//
//int MotorSpeedTranslator::translateMotorSpeed(
//	int currentMotorSpeed, 
//	int input, 
//	int desiredMotorSpeed, 
//	IInertia* accel, 
//	IInertia* decel, 
//	IInertia* brake)
//{
//	if (input < channel.dbMin)
//	{
//		// User is actively braking
//		return brake->map(currentMotorSpeed, desiredMotorSpeed);
//	}
//
//	if (input < channel.dbMax)
//	{
//		// Stick is at rest - use servo center as deceleration target
//		return decel->map(currentMotorSpeed, servo.center);
//	}
//
//	if (desiredMotorSpeed > currentMotorSpeed)
//	{
//		// Accelerating
//		return accel->map(currentMotorSpeed, desiredMotorSpeed);
//	}
//	if (desiredMotorSpeed < currentMotorSpeed)
//	{
//		// Decelerating
//		return decel->map(currentMotorSpeed, desiredMotorSpeed);
//	}
//
//	// Holding steady
//	return currentMotorSpeed;
//}
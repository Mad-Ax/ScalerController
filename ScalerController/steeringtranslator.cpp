#include "steeringtranslator.h"


SteeringTranslator::SteeringTranslator(const IInertia*& inertia) : inertia(inertia)
{
}

SteeringTranslator::~SteeringTranslator()
{
}

int SteeringTranslator::translateSteering(
	int currentSteering,
	int desiredSteering,
	HardwareSerial& ser) const
{
	ser.print("this is steering translator - ");
	ser.print(currentSteering);
	ser.print(":");
	ser.print(desiredSteering);
	ser.println();

	return this->inertia->map(currentSteering, desiredSteering);
}
#include "steeringtranslator.h"

SteeringTranslator::SteeringTranslator(AnalogChannel channel, ServoConfig config)
{
	this->channel = channel;
	this->servo = servo;
}

SteeringTranslator::~SteeringTranslator()
{
}

int SteeringTranslator::translateSteering(
	int currentSteering,
	int desiredSteering,
	IInertia* inertia) // TODO: why is Inertia here - shouldn't it be in the ctor?
{
	return inertia->map(currentSteering, desiredSteering);
}
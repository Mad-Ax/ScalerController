#pragma once

#include "structs.h"

class IOutputTrailer
{
public:
	// Sends the required light and control settings to the trailer
	virtual void send(LightSetting lightSetting, const unsigned int servoPositions[4]) = 0;
};
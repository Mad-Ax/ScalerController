#pragma once

#include "structs.h"

class IOutputLights
{
public:
	// Sends the required light settings to LED pins
	virtual void send(LightSetting lightSetting) = 0;
};
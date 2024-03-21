#pragma once

#include "structs.h"

class ISwitchTranslatorThreeWay
{
public:
	virtual ThreeWayPosition translateSwitch(int value) const = 0;
};

#pragma once

#include "structs.h"

class ISwitchTranslatorTwoWay
{
public:
	virtual bool translateSwitch(int value) = 0;
};
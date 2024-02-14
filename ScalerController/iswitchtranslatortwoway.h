#pragma once

#include "structs.h"

class ISwitchTranslatorTwoWay
{
public:
	virtual bool translateSwitch(SwitchChannelTwoWay channel, int value) = 0;
};
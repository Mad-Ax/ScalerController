#pragma once

#include "structs.h"

class ISwitchTranslatorTwoWay
{
public:
	virtual bool translateSwitch(SwitchChannelTwoWay channel, int value) = 0; // TODO: see feature/referencerefactor on github - this was refactored for neater operation
};
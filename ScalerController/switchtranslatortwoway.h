#pragma once

#include "iswitchtranslatortwoway.h"

class SwitchTranslatorTwoWay : public ISwitchTranslatorTwoWay
{
public:
	SwitchTranslatorTwoWay();
	~SwitchTranslatorTwoWay();

	bool translateSwitch(SwitchChannelTwoWay channel, int value); // TODO: consider refactor to same pattern as threeway (see feature/referencerefactor)
};
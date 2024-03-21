#pragma once

#include "iswitchtranslatortwoway.h"

class SwitchTranslatorTwoWay : public ISwitchTranslatorTwoWay
{
public:
	SwitchTranslatorTwoWay(SwitchChannelTwoWay channel);
	~SwitchTranslatorTwoWay();

	bool translateSwitch(int value);

private:
	SwitchChannelTwoWay channel;
};
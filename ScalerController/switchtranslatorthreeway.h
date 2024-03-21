#pragma once

#include "iswitchtranslatorthreeway.h"

class SwitchTranslatorThreeWay : public ISwitchTranslatorThreeWay
{
public:
	SwitchTranslatorThreeWay(SwitchChannelThreeWay channel);
	~SwitchTranslatorThreeWay();

	ThreeWayPosition translateSwitch(int value);

private:
	SwitchChannelThreeWay channel;
};
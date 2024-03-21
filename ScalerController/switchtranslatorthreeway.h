#pragma once

#include "iswitchtranslatorthreeway.h"

class SwitchTranslatorThreeWay : public ISwitchTranslatorThreeWay
{
public:
	SwitchTranslatorThreeWay(SwitchChannelThreeWay channel);
	~SwitchTranslatorThreeWay();

	ThreeWayPosition translateSwitch(int value) const;

private:
	SwitchChannelThreeWay channel;
};
#include "switchtranslatorthreeway.h"

SwitchTranslatorThreeWay::SwitchTranslatorThreeWay(SwitchChannelThreeWay channel) : channel(channel)
{
}

SwitchTranslatorThreeWay::~SwitchTranslatorThreeWay()
{
}

ThreeWayPosition SwitchTranslatorThreeWay::translateSwitch(int value) const
{
	if (value > this->channel.high)
	{
		return ThreeWayPosition::PosA;
	}

	if (value < this->channel.low)
	{
		return ThreeWayPosition::PosC;
	}

	return ThreeWayPosition::PosB;
}
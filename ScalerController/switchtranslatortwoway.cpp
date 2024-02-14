#include "switchtranslatortwoway.h"

SwitchTranslatorTwoWay::SwitchTranslatorTwoWay()
{
}

SwitchTranslatorTwoWay::~SwitchTranslatorTwoWay()
{
}

bool SwitchTranslatorTwoWay::translateSwitch(SwitchChannelTwoWay channel, int value)
{
	if (value > channel.high)
	{
		return true;
	}

	return false;
}
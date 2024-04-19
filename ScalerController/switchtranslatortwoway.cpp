#include "switchtranslatortwoway.h"

SwitchTranslatorTwoWay::SwitchTranslatorTwoWay(SwitchChannelTwoWay& channel) : channel(channel)
{
}

SwitchTranslatorTwoWay::~SwitchTranslatorTwoWay()
{
}

bool SwitchTranslatorTwoWay::translateSwitch(int value)
{
	if (value > this->channel.high)
	{
		return true;
	}

	return false;
}
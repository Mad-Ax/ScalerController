#include "ppmWrapper.h"

PpmWrapper::PpmWrapper(
	const byte& ppmInputPin,
	const byte& totalChannels,
	const unsigned int& ppmChannelValueMaxError,
	const unsigned int& ppmBlankTime,
	const unsigned int& minChannelValue,
	const unsigned int& maxChannelValue)
{
	ppm = new PPMReader(ppmInputPin, totalChannels);
	ppm->channelValueMaxError = ppmChannelValueMaxError;
	ppm->blankTime = ppmBlankTime;
	ppm->minChannelValue = minChannelValue;
	ppm->maxChannelValue = maxChannelValue;
}

PpmWrapper::~PpmWrapper()
{
	delete ppm;
}

unsigned long PpmWrapper::latestValidChannelValue(int channel, unsigned long defaultValue) const
{
	return ppm->latestValidChannelValue(channel, defaultValue);
}

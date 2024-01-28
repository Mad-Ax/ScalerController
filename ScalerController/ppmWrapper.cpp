#include "ppmWrapper.h"

PpmWrapper::PpmWrapper(InputConfig inputConfig)
{
	ppm = new PPMReader(inputConfig.ppmInput, inputConfig.totalChannels);
	ppm->channelValueMaxError = inputConfig.ppmChannelValueMaxError;
	ppm->blankTime = inputConfig.ppmBlankTime;
	ppm->minChannelValue = inputConfig.minChannelValue;
	ppm->maxChannelValue = inputConfig.maxChannelValue;
}

PpmWrapper::~PpmWrapper()
{
	delete ppm;
}

unsigned long PpmWrapper::latestValidChannelValue(int channel, unsigned long defaultValue)
{
	return ppm->latestValidChannelValue(channel, defaultValue);
}

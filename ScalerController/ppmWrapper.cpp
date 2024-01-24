#include "ppmWrapper.h"

PpmWrapper::PpmWrapper(InputConfig inputConfig)
{
	ppm = new PPMReader(inputConfig.ppmInput, 8);
	ppm->channelValueMaxError = inputConfig.ppmChannelValueMaxError;
	ppm->blankTime = inputConfig.ppmBlankTime;
}

PpmWrapper::~PpmWrapper()
{
	delete ppm;
}

unsigned long PpmWrapper::latestValidChannelValue(int channel, unsigned long defaultValue)
{
	return ppm->latestValidChannelValue(channel, defaultValue);
}

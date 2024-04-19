#pragma once

#include <PPMReader.h>
#include "structs.h"
#include "ippmwrapper.h"

class PpmWrapper: public IPpmWrapper
{
	public:
		PpmWrapper(
			const byte& ppmInputPin,
			const byte& totalChannels,
			const unsigned int& ppmChannelValueMaxError,
			const unsigned int& ppmBlankTime,
			const unsigned int& minChannelValue,
			const unsigned int& maxChannelValue);
		~PpmWrapper();

		/* Returns the latest received value that was considered valid for the channel (starting from 1).
		* Returns defaultValue if the given channel hasn't received any valid values yet. */
		unsigned long latestValidChannelValue(int channel, unsigned long defaultValue) const;

	private:
		PPMReader* ppm;
};

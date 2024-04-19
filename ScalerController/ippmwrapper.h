#pragma once

class IPpmWrapper
{
	public: 
		/* Returns the latest received value that was considered valid for the channel (starting from 1).
		* Returns defaultValue if the given channel hasn't received any valid values yet. */
		virtual unsigned long latestValidChannelValue(int channel, unsigned long defaultValue) const = 0;
};
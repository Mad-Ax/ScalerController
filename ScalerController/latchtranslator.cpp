#include "latchtranslator.h"

LatchTranslator::LatchTranslator()
{
}

LatchTranslator::~LatchTranslator()
{
}

int LatchTranslator::translateLatch(LatchChannel channel, int value, int lastValue)
{
	if (value > channel.min && value < channel.max)
	{
		// We are within the range to change

		// If we are already latching, we should not change the value
		if (latching)
		{
			return lastValue;
		}
		else
		{
			// Record the latching state
			latching = true;

			if (lastValue == 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		// We are outside of the latch range - Remove any latch
		latching = false;

		return lastValue;
	}
}
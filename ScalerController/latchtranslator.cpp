#include "latchtranslator.h"

LatchTranslator::LatchTranslator()
{
}

LatchTranslator::~LatchTranslator()
{
}

bool LatchTranslator::translateLatch(LatchChannel channel, int value)
{
	if (value <= channel.min || value >= channel.max)
	{
		// We are not within the latch range - unset the latch (if we set it) and return false
		latching = false;
		return false;
	}


	// We are within the range to change

	// If we are already latching, we have been here before and should return false
	if (latching)
	{
		return false;
	}
	else
	{
		// This is our first past - we should record the latching state
		latching = true;

		// And then we should return true
		return true;
	}
}
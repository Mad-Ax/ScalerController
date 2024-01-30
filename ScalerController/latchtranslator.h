#pragma once

#include "ilatchtranslator.h"

class LatchTranslator : public ILatchTranslator
{
public:
	LatchTranslator();
	~LatchTranslator();

	int translateLatch(LatchChannel channel, int value, int lastValue);

private:
	bool latching;
};


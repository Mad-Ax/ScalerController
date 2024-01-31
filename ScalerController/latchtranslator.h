#pragma once

#include "ilatchtranslator.h"

class LatchTranslator : public ILatchTranslator
{
public:
	LatchTranslator();
	~LatchTranslator();

	bool translateLatch(LatchChannel channel, int value);

private:
	bool latching;
};


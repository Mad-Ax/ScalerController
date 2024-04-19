#pragma once

#include "ilatchtranslator.h"

class LatchTranslator : public ILatchTranslator
{
public:
	LatchTranslator(LatchChannel& channel);
	~LatchTranslator();

	bool translateLatch(int value);

private:
	bool latching;
	LatchChannel& channel;
};


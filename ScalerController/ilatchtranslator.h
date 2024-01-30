#pragma once

#include "structs.h";

class ILatchTranslator
{
public:
	virtual int translateLatch(LatchChannel channel, int value, int lastValue) = 0;
};
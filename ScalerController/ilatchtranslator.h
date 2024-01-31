#pragma once

#include "structs.h";

class ILatchTranslator
{
public:
	virtual bool translateLatch(LatchChannel channel, int value) = 0;
};
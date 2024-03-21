#pragma once

#include "structs.h";

class ILatchTranslator
{
public:
	virtual bool translateLatch(int value) = 0;
};
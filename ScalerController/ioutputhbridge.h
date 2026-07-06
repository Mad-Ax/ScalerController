#pragma once

class IOutputHbridge
{
public:
	// Sends the required logic to the H-bridge
	virtual void send(int value) = 0;
};
#ifndef OUTPUTHBRIDGE_H
#define OUTPUTHBRIDGE_H

#include "ioutputhbridge.h"
#include "structs.h"

// OutputHbridge class
// Outputs appropriate logic to the H-bridge pins
class OutputHbridge : public IOutputHbridge
{
private:
	HbridgeConfig config;
	int lastValue;

// Configures the instance with the correct pins
public: OutputHbridge(HbridgeConfig config);

// Sends the required logic to the H-bridge
public: void send(int value);
};

#endif OUTPUTHBRIDGE_H
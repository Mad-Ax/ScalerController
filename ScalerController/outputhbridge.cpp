#include "outputhbridge.h"

// Constructor
// Takes the H-bridge config as a parameter
OutputHbridge::OutputHbridge(HbridgeConfig config)
{
	this->config = config;

	pinMode(config.pinA, OUTPUT);
	pinMode(config.pinB, OUTPUT);
}

// Sends the required logic to the H-bridge
void OutputHbridge::send(int value)
{
	if (lastValue != value)
	{
		lastValue = value;

		if (value > this->config.switchHigh)
		{
			digitalWrite(this->config.pinA, HIGH);
			digitalWrite(this->config.pinB, LOW);
			return;
		}

		if (value < this->config.switchLow)
		{
			digitalWrite(this->config.pinA, LOW);
			digitalWrite(this->config.pinB, HIGH);
			return;
		}

		digitalWrite(this->config.pinA, LOW);
		digitalWrite(this->config.pinB, LOW);
	}
}
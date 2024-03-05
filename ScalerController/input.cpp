#include "input.h"
#include "defines.h"

Input::Input(InputConfig config, IPpmWrapper* ppmWrapper)
{
	this->config = config;
	this->ppmWrapper = ppmWrapper;

	// Set up the mode pin
	pinMode(config.modePin, INPUT_PULLUP);
}

void Input::update()
{
  for(int channel = 0; channel < this->config.totalChannels; channel++)
  {
    this->setting.channel[channel] = ppmWrapper->latestValidChannelValue(channel+1, 0);
  }

  Mode modeSwitch = static_cast<Mode>(digitalRead(this->config.modePin));

  this->setting.mode = modeSwitch;
}
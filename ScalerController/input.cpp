#include "input.h"
#include "defines.h"

Input::Input(InputConfig config, IPpmWrapper* ppmWrapper)
{
	this->config = config;
	this->ppmWrapper = ppmWrapper;
}

void Input::update()
{
  for(int channel = 0; channel < this->config.totalChannels; channel++)
  {
    this->setting.channel[channel] = ppmWrapper->latestValidChannelValue(channel+1, 0);
  }

  this->setting.mode = MODE;
}
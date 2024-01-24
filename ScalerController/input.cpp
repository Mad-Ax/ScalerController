#include "input.h"

Input::Input(IPpmWrapper* ppmWrapper)
{
	this->ppmWrapper = ppmWrapper;
}

void Input::update()
{
  for(int channel = 0; channel < 8; channel++)
  {
    this->setting.channel[channel] = ppmWrapper->latestValidChannelValue(channel+1, 0);  
  }
}
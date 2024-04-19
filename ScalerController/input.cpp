#include "input.h"
#include "defines.h"

Input::Input(const int modePin, const int& totalChannels, const IPpmWrapper*& ppmWrapper) : _modePin(modePin), totalChannels(totalChannels), ppmWrapper(ppmWrapper)
{
	// Set up the mode pin
	pinMode(modePin, INPUT_PULLUP);
}

void Input::update(HardwareSerial& ser)
{
  for(int channel = 0; channel < this->totalChannels; channel++)
  {
    this->setting.channel[channel] = this->ppmWrapper->latestValidChannelValue(channel+1, 0);
  }

  ser.print("Mode pin is : ");
  ser.println(this->_modePin);

  Mode modeSwitch = static_cast<Mode>(digitalRead(this->_modePin));

  this->setting.mode = modeSwitch;
}
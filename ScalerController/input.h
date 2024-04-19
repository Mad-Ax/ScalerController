#pragma once

#include "structs.h"
#include "ippmwrapper.h"

// Input class
// Reads raw PWM values from receiver pins
class Input
{
public:
    InputSetting setting;

	Input(const int modePin, const int& totalChannels, const IPpmWrapper*& ppmWrapper);

    // Update method
    // Updates the class with the latest raw values from the receiver
    void update(HardwareSerial& ser);

private:
    const int _modePin;
    const int& totalChannels;
    const IPpmWrapper*& ppmWrapper;
};

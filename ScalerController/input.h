#pragma once

#include "structs.h"
#include "ippmwrapper.h"
//#include "arduino.h"

// Input class
// Reads raw PWM values from receiver pins
class Input
{
public:
    InputSetting setting;

private:
    InputConfig config;
	IPpmWrapper* ppmWrapper;

public:
	Input(InputConfig config, IPpmWrapper* ppmWrapper);

    // Update method
    // Updates the class with the latest raw values from the receiver
    void update();
};

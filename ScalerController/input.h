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
	IPpmWrapper* ppmWrapper;

public:
	Input(IPpmWrapper* ppmWrapper);

    // Update method
    // Updates the class with the latest raw values from the receiver
    void update();
};

#pragma once

#include "iinertia.h"
#include "HardwareSerial.h"

class Inertia : public IInertia
{
private:
    const int& inertia;
    const int& minimum;
    const int& maximum;
    const int& factor;
    HardwareSerial& ser;

public:
    // Constructor
    Inertia(const int& inertia, const int& minimum, const int& maximum, HardwareSerial& ser);

    // Maps the requested value with inertia
    int map(int current, int requested) const;
};
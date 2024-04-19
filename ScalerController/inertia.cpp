#include "inertia.h"

Inertia::Inertia(
    const int& inertia,
    const int& minimum,
    const int& maximum,
    HardwareSerial& ser) :
    inertia(inertia),
    minimum(minimum),
    maximum(maximum),
    factor((maximum - minimum) / inertia),
    ser(ser)
{
}

int Inertia::map(int current, int requested) const
{
    if (current == requested)
    {
        return current;
    }

    int val;
    if (requested > current)
    {
        val = current + (((requested - current) + this->factor - 1) / this->factor);
    }
    else
    {
        val = current - (((current - requested) + this->factor - 1) / this->factor);
    }

    if (val > this->maximum) return this->maximum;

    if (val < this->minimum) return this->minimum;

    return val;
}
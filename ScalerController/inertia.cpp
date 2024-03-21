#include "inertia.h"

Inertia::Inertia(int inertia, int minimum, int maximum) : inertia(inertia), mininum(mininum), maximum(maximum), factor((maximum - mininum) / inertia)
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

    if (val < this->mininum) return this->mininum;

    return val;
}
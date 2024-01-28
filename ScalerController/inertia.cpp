#include "inertia.h"

Inertia::Inertia(int inertia, int min, int max)
{   
    this->inertia = inertia;
    this->min = min;
    this->max = max;
    this->factor = (max - min) / inertia; 
}

int Inertia::map(int current, int requested)
{
    if (current == requested) return current;

    int val;
    if (requested > current)
    {
        val = current + (((requested - current) + this->factor - 1) / this->factor);
    }
    else
    {
        val = current - (((current - requested) + this->factor - 1) / this->factor);
    }

    if (val > this->max) return max;

    if (val < this->min) return min;

    return val;
}
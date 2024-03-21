#pragma once

#include "iinertia.h"

class Inertia : public IInertia
{
private:
    const int inertia;
    const int mininum;
    const int maximum;
    const int factor;

public:
    // Constructor
    Inertia(int inertia, int minimum, int maximum);

    // Maps the requested value with inertia
    int map(int current, int requested) const;
};
#pragma once

class IInertia
{
public:
	// Maps the requested value with inertia
	virtual int map(int current, int requested) const = 0;
};
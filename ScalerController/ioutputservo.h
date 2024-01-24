#pragma once

class IOutputServo
{
public:
	// Sends the required angle to the servo
	virtual void send(int value) = 0;
};
#include "outputServo.h"

// Constructor
// Takes the servo pin as a parameter
OutputServo::OutputServo(const ServoConfig& config) : config(config)
{
  this->servo.attach(config.pin, config.minimum, config.maximum);
  this->servo.write(config.center);
}

// Sends the required angle to the servo
void OutputServo::send(int value)
{
  if (lastValue != value)
  {
    servo.writeMicroseconds(value);
    lastValue = value;
  }
}

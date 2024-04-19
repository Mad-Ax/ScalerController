#ifndef OUTPUTSERVO_H
#define OUTPUTSERVO_H

#include "Servo.h"
#include "ioutputservo.h"
#include "structs.h"

// OutputServo class
// Outputs values to the servo pin
class OutputServo : public IOutputServo
{
  private:
    const ServoConfig& config;
    Servo servo;
    int lastValue;
  
  // Configures the instance with the correct pin
  public: OutputServo(const ServoConfig& config);

  // Sends the required angle to the servo
  public: void send(int value);
};

#endif OUTPUTSERVO_H

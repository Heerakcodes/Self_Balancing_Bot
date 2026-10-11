
#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include <Arduino.h>

class MotionController
{
private:
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t enablePin;

    bool enabled;
    bool direction;

    float speed;
    unsigned long lastStepTime;
    unsigned long stepInterval;

public:
    MotionController(uint8_t stepPin,
                     uint8_t dirPin,
                     uint8_t enablePin);

    void begin();
    void enable();
    void disable();

    void setSpeed(float stepsPerSecond);
    void setDirection(bool direction);

    void runSpeed();
    void stop();
};

#endif
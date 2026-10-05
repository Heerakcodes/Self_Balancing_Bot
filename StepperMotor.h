#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

class StepperMotor
{
private:
    int stepPin;
    int dirPin;
    int enablePin;

public:
    StepperMotor(int stepPin, int dirPin, int enablePin);

    void begin();

    void enable();
    void disable();

    void setDirection(bool direction);

    void moveSteps(int steps, int speed);
    void stop();
};

#endif
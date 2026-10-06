#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

class StepperMotor
{
private:
    int stepPin;
    int dirPin;
    int enablePin;

    int currentSpeed;
    int maxSpeed;

    bool directionInverted;
    bool currentDirection;

    unsigned long lastStepTime;

public:
    StepperMotor(int stepPin, int dirPin, int enablePin);

    void begin();

    void enable();
    void disable();

    void setDirection(bool direction);
    void setDirectionInverted(bool inverted);

    void setSpeed(int speed);
    void setMaxSpeed(int speed);

    void run();

    void stop();

    bool isRunning();
};

#endif
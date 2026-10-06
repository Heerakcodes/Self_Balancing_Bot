#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "StepperMotor.h"

class MotorController
{
private:
    StepperMotor* leftMotor;
    StepperMotor* rightMotor;

    int maxOutput;

    bool leftMotorInverted;
    bool rightMotorInverted;

public:
    MotorController(StepperMotor* leftMotor,StepperMotor* rightMotor);

    void begin();

    void setMaxOutput(int maxOutput);

    void setMotorInversion(bool leftInverted, bool rightInverted);

    void setOutput(float correction);

    void run();

    void stop();

    bool isRunning();
};

#endif
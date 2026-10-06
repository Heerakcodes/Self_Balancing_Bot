#include <Arduino.h>
#include "StepperMotor.h"

StepperMotor motor(25, 26, 27);

void setup()
{
    motor.begin();
    motor.enable();

    motor.setDirection(true);
}

void loop()
{
    motor.moveSteps(200, 500);

    delay(1000);

    motor.setDirection(false);

    motor.moveSteps(200, 500);

    delay(1000);
}
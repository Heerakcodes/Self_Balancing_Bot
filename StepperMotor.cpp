
#include "StepperMotor.h"

StepperMotor::StepperMotor(uint8_t stepPin,
                           uint8_t dirPin,
                           uint8_t enablePin)
{
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->enablePin = enablePin;

    enabled = false;
    direction = HIGH;
    speed = 0.0f;
    lastStepTime = 0;
    stepInterval = 0;
}

void StepperMotor::begin()
{
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enablePin, OUTPUT);

    digitalWrite(stepPin, LOW);
    digitalWrite(dirPin, direction);

    disable();
}

void StepperMotor::enable()
{
    digitalWrite(enablePin, LOW);
    enabled = true;
}

void StepperMotor::disable()
{
    digitalWrite(enablePin, HIGH);
    enabled = false;
}

void StepperMotor::setDirection(bool newDirection)
{
    if (direction != newDirection)
    {
        direction = newDirection;
        digitalWrite(dirPin, direction);
        lastStepTime = micros();
    }
}

void StepperMotor::setSpeed(float stepsPerSecond)
{
    if (stepsPerSecond < 0.0f)
    {
        setDirection(LOW);
        speed = -stepsPerSecond;
    }
    else
    {
        setDirection(HIGH);
        speed = stepsPerSecond;
    }

    if (speed > 0.0f)
    {
        stepInterval = (unsigned long)(1000000.0f / speed);

        if (stepInterval < 10)
            stepInterval = 10;
    }
    else
    {
        stepInterval = 0;
    }
}

void StepperMotor::runSpeed()
{
    if (!enabled || speed <= 0.0f || stepInterval == 0)
        return;

    unsigned long now = micros();

    if ((unsigned long)(now - lastStepTime) >= stepInterval)
    {
        lastStepTime = now;

        digitalWrite(stepPin, HIGH);
        delayMicroseconds(3);
        digitalWrite(stepPin, LOW);
    }
}

void StepperMotor::stop()
{
    speed = 0.0f;
    stepInterval = 0;
}
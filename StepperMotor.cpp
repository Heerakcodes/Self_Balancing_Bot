#include "StepperMotor.h"
#include <Arduino.h>

// Constructor
StepperMotor::StepperMotor(int stepPin, int dirPin, int enablePin)
{
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->enablePin = enablePin;

    currentSpeed = 0;
    maxSpeed = 2000;

    directionInverted = false;
    currentDirection = true;

    lastStepTime = 0;
}

// Initialize motor pins
void StepperMotor::begin()
{
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enablePin, OUTPUT);

    digitalWrite(stepPin, LOW);

    stop();
    disable();
}

// Enable the motor driver
void StepperMotor::enable()
{
    // Assumes ENABLE is active LOW.
    // Verify this with your actual driver.
    digitalWrite(enablePin, LOW);
}

// Disable the motor driver
void StepperMotor::disable()
{
    // Assumes ENABLE is active LOW.
    digitalWrite(enablePin, HIGH);
}

// Set motor direction
void StepperMotor::setDirection(bool direction)
{
    currentDirection = direction;

    bool actualDirection = direction;

    if (directionInverted)
    {
        actualDirection = !actualDirection;
    }

    digitalWrite(dirPin, actualDirection ? HIGH : LOW);
}

// Reverse the meaning of forward/reverse
void StepperMotor::setDirectionInverted(bool inverted)
{
    directionInverted = inverted;

    // Re-apply the current direction
    setDirection(currentDirection);
}

// Set maximum allowed speed
void StepperMotor::setMaxSpeed(int speed)
{
    if (speed < 1)
    {
        return;
    }

    maxSpeed = speed;

    // If current speed is already above the new limit,
    // reduce it.
    if (currentSpeed > maxSpeed)
    {
        currentSpeed = maxSpeed;
    }
    else if (currentSpeed < -maxSpeed)
    {
        currentSpeed = -maxSpeed;
    }
}

// Set motor speed
//
// Positive speed = one direction
// Negative speed = opposite direction
// Zero = stop
//
// Speed is approximately steps per second.
void StepperMotor::setSpeed(int speed)
{
    // Limit the requested speed
    if (speed > maxSpeed)
    {
        speed = maxSpeed;
    }
    else if (speed < -maxSpeed)
    {
        speed = -maxSpeed;
    }

    // Determine requested direction
    if (speed > 0)
    {
        setDirection(true);
    }
    else if (speed < 0)
    {
        setDirection(false);
    }

    currentSpeed = speed;
}

// Generate step pulses without blocking
void StepperMotor::run()
{
    if (currentSpeed == 0)
    {
        return;
    }

    unsigned long currentTime = micros();

    int speedMagnitude = abs(currentSpeed);

    if (speedMagnitude < 1)
    {
        return;
    }

    // Calculate time between step pulses
    unsigned long stepInterval =
        1000000UL / speedMagnitude;

    // Generate a step when enough time has passed
    if (currentTime - lastStepTime >= stepInterval)
    {
        lastStepTime = currentTime;

        digitalWrite(stepPin, HIGH);

        // Short STEP pulse
        delayMicroseconds(2);

        digitalWrite(stepPin, LOW);
    }
}

// Stop generating step pulses
void StepperMotor::stop()
{
    currentSpeed = 0;

    digitalWrite(stepPin, LOW);
}

// Check whether the motor is currently commanded to run
bool StepperMotor::isRunning()
{
    return currentSpeed != 0;
}
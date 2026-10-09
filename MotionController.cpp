#include "MotionController.h"
#include <Arduino.h>


// ============================================================
// CONSTRUCTOR
// ============================================================

MotionController::MotionController()
{
    // ============================================================
    // ROBOT PARAMETERS
    // ============================================================

    wheelRadius = 0.098;
    // Wheel radius = 98 mm.
    // CHANGE DURING ACTUAL CALIBRATION.

    wheelBase = 0.20;
    // Temporary wheelbase.
    // CHANGE DURING ACTUAL CALIBRATION.


    // ============================================================
    // MOTOR PARAMETERS
    // ============================================================

    motorStepsPerRevolution = 200;
    // NEMA 17 = 200 full steps/revolution.
    // 1.8 degree step angle.

    microsteps = 1;
    // Full-step operation.
    // CHANGE TO MATCH ACTUAL TB6600 SETTING.


    // ============================================================
    // MOTION LIMITS
    // ============================================================

    maxForwardVelocity = 0.30;
    // Maximum forward/backward velocity.
    // CHANGE DURING ACTUAL CALIBRATION.

    maxTurnRate = 1.0;
    // Maximum turning rate.
    // CHANGE DURING ACTUAL CALIBRATION.

    acceleration = 0.20;
    // Maximum linear acceleration.
    // CHANGE DURING ACTUAL CALIBRATION.

    turnAcceleration = 1.0;
    // Maximum turning acceleration.
    // CHANGE DURING ACTUAL CALIBRATION.


    // ============================================================
    // MOTOR LIMIT
    // ============================================================

    maxStepRate = 2000;
    // Maximum step rate.
    // CHANGE DURING ACTUAL MOTOR TESTING.


    // ============================================================
    // INITIAL VALUES
    // ============================================================

    currentCommand = STOP;

    targetVelocity = 0.0;
    targetTurnRate = 0.0;

    currentVelocity = 0.0;
    currentTurnRate = 0.0;

    leftWheelVelocity = 0.0;
    rightWheelVelocity = 0.0;

    leftStepRate = 0;
    rightStepRate = 0;

    previousTime = 0;
}


// ============================================================
// BEGIN
// ============================================================

void MotionController::begin()
{
    currentCommand = STOP;

    targetVelocity = 0.0;
    targetTurnRate = 0.0;

    currentVelocity = 0.0;
    currentTurnRate = 0.0;

    leftWheelVelocity = 0.0;
    rightWheelVelocity = 0.0;

    leftStepRate = 0;
    rightStepRate = 0;

    previousTime = micros();
}


// ============================================================
// SET COMMAND
// ============================================================

void MotionController::setCommand(MotionCommand command)
{
    currentCommand = command;

    switch (command)
    {
        case FORWARD:

            setMotion(
                maxForwardVelocity,
                0.0
            );

            break;


        case BACKWARD:

            setMotion(
                -maxForwardVelocity,
                0.0
            );

            break;


        case LEFT:

            setMotion(
                0.0,
                -maxTurnRate
            );

            break;


        case RIGHT:

            setMotion(
                0.0,
                maxTurnRate
            );

            break;


        case STOP:

            setMotion(
                0.0,
                0.0
            );

            break;
    }
}


// ============================================================
// SET MOTION
// ============================================================

void MotionController::setMotion(
    float forwardVelocity,
    float turnRate
)
{
    // ------------------------------------------------------------
    // LIMIT FORWARD VELOCITY
    // ------------------------------------------------------------

    if (forwardVelocity > maxForwardVelocity)
    {
        forwardVelocity = maxForwardVelocity;
    }

    else if (forwardVelocity < -maxForwardVelocity)
    {
        forwardVelocity = -maxForwardVelocity;
    }


    // ------------------------------------------------------------
    // LIMIT TURN RATE
    // ------------------------------------------------------------

    if (turnRate > maxTurnRate)
    {
        turnRate = maxTurnRate;
    }

    else if (turnRate < -maxTurnRate)
    {
        turnRate = -maxTurnRate;
    }


    // ------------------------------------------------------------
    // STORE TARGET
    // ------------------------------------------------------------

    targetVelocity = forwardVelocity;
    targetTurnRate = turnRate;
}


// ============================================================
// UPDATE
// ============================================================

void MotionController::update()
{
    unsigned long currentTime = micros();


    float dt =
        (currentTime - previousTime) / 1000000.0;


    previousTime = currentTime;


    // ------------------------------------------------------------
    // IGNORE INVALID TIME INTERVAL
    // ------------------------------------------------------------

    if (dt <= 0.0 || dt > 0.1)
    {
        return;
    }


    // ============================================================
    // LINEAR ACCELERATION LIMITING
    // ============================================================

    float velocityDifference =
        targetVelocity - currentVelocity;


    float maximumVelocityChange =
        acceleration * dt;


    if (velocityDifference > maximumVelocityChange)
    {
        velocityDifference = maximumVelocityChange;
    }

    else if (velocityDifference < -maximumVelocityChange)
    {
        velocityDifference = -maximumVelocityChange;
    }


    currentVelocity += velocityDifference;


    // ============================================================
    // TURN ACCELERATION LIMITING
    // ============================================================

    float turnDifference =
        targetTurnRate - currentTurnRate;


    float maximumTurnChange =
        turnAcceleration * dt;


    if (turnDifference > maximumTurnChange)
    {
        turnDifference = maximumTurnChange;
    }

    else if (turnDifference < -maximumTurnChange)
    {
        turnDifference = -maximumTurnChange;
    }


    currentTurnRate += turnDifference;


    // ============================================================
    // UPDATE WHEEL REFERENCES
    // ============================================================

    updateWheelReferences();


    // ============================================================
    // CONVERT TO STEP RATES
    // ============================================================

    leftStepRate =
        velocityToStepRate(leftWheelVelocity);

    rightStepRate =
        velocityToStepRate(rightWheelVelocity);
}


// ============================================================
// UPDATE WHEEL REFERENCES
// ============================================================

void MotionController::updateWheelReferences()
{
    /*
        Differential-drive equations:

        Left:

        VL = V - (ω × L / 2)

        Right:

        VR = V + (ω × L / 2)

        V = forward velocity
        ω = turn rate
        L = wheelbase
    */


    leftWheelVelocity =
        currentVelocity -
        (currentTurnRate * wheelBase / 2.0);


    rightWheelVelocity =
        currentVelocity +
        (currentTurnRate * wheelBase / 2.0);
}


// ============================================================
// VELOCITY → STEP RATE
// ============================================================

int MotionController::velocityToStepRate(float velocity)
{
    if (velocity == 0.0)
    {
        return 0;
    }


    /*
        Wheel circumference:

        C = 2πr
    */

    float wheelCircumference =
        2.0 * PI * wheelRadius;


    /*
        Wheel revolutions per second:

        rev/s = velocity / circumference
    */

    float revolutionsPerSecond =
        velocity / wheelCircumference;


    /*
        Motor step rate:

        steps/s =
        rev/s × steps/revolution × microsteps
    */

    float stepsPerSecond =
        revolutionsPerSecond *
        motorStepsPerRevolution *
        microsteps;


    // ------------------------------------------------------------
    // LIMIT STEP RATE
    // ------------------------------------------------------------

    if (stepsPerSecond > maxStepRate)
    {
        stepsPerSecond = maxStepRate;
    }

    else if (stepsPerSecond < -maxStepRate)
    {
        stepsPerSecond = -maxStepRate;
    }


    return (int)stepsPerSecond;
}


// ============================================================
// GET FORWARD VELOCITY
// ============================================================

float MotionController::getForwardVelocity()
{
    return currentVelocity;
}


// ============================================================
// GET TURN RATE
// ============================================================

float MotionController::getTurnRate()
{
    return currentTurnRate;
}


// ============================================================
// GET LEFT WHEEL VELOCITY
// ============================================================

float MotionController::getLeftWheelVelocity()
{
    return leftWheelVelocity;
}


// ============================================================
// GET RIGHT WHEEL VELOCITY
// ============================================================

float MotionController::getRightWheelVelocity()
{
    return rightWheelVelocity;
}


// ============================================================
// GET LEFT STEP RATE
// ============================================================

int MotionController::getLeftStepRate()
{
    return leftStepRate;
}


// ============================================================
// GET RIGHT STEP RATE
// ============================================================

int MotionController::getRightStepRate()
{
    return rightStepRate;
}


// ============================================================
// GET COMMAND
// ============================================================

MotionCommand MotionController::getCommand()
{
    return currentCommand;
}
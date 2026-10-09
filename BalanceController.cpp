
#include "BalanceController.h"
#include <Arduino.h>

BalanceController::BalanceController()
{
    Kp = 0.0f;
    Ki = 0.0f;
    Kd = 0.0f;

    targetAngle = 0.0f;
    angleError = 0.0f;

    integral = 0.0f;
    integralLimit = 100.0f;

    maxOutput = 1000.0f;
    output = 0.0f;

    previousTime = 0;
    firstUpdate = true;
}

void BalanceController::begin()
{
    reset();
}

void BalanceController::setPID(float kp, float ki, float kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}

void BalanceController::setTargetAngle(float angle)
{
    targetAngle = angle;
}

void BalanceController::setOutputLimit(float limit)
{
    if (limit > 0.0f)
    {
        maxOutput = limit;
    }
}

void BalanceController::reset()
{
    integral = 0.0f;
    angleError = 0.0f;
    output = 0.0f;

    previousTime = micros();
    firstUpdate = true;
}

float BalanceController::update(
    float currentAngle,
    float gyroRate)
{
    unsigned long currentTime = micros();

    if (firstUpdate)
    {
        previousTime = currentTime;
        firstUpdate = false;
        return output;
    }

    float dt =
        (currentTime - previousTime) / 1000000.0f;

    previousTime = currentTime;

    // Reject invalid or excessively delayed updates.
    if (dt <= 0.0f || dt > 0.05f)
    {
        integral = 0.0f;
        output = 0.0f;
        return output;
    }

    // Positive error means the measured angle
    // is below the target angle.
    angleError = targetAngle - currentAngle;

    // Calculate the proposed integral.
    float proposedIntegral = integral + angleError * dt;

    if (proposedIntegral > integralLimit)
        proposedIntegral = integralLimit;

    if (proposedIntegral < -integralLimit)
        proposedIntegral = -integralLimit;

    // PID terms.
    float pTerm = Kp * angleError;
    float dTerm = -Kd * gyroRate;

    // Candidate output including integral action.
    float candidateOutput =
        pTerm + (Ki * proposedIntegral) + dTerm;

    // Basic anti-windup: do not accumulate more
    // integral when the output is saturated in
    // the same direction as the error.
    bool pushingFurtherIntoSaturation =
        (candidateOutput > maxOutput && angleError > 0.0f) ||
        (candidateOutput < -maxOutput && angleError < 0.0f);

    if (!pushingFurtherIntoSaturation)
    {
        integral = proposedIntegral;
    }

    // Recalculate using the accepted integral.
    output = pTerm + (Ki * integral) + dTerm;

    // Limit the correction.
    if (output > maxOutput)
        output = maxOutput;

    if (output < -maxOutput)
        output = -maxOutput;

    return output;
}

float BalanceController::getOutput()
{
    return output;
}

float BalanceController::getError()
{
    return angleError;
}
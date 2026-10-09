
#include "SafetyController.h"
#include <math.h>

SafetyController::SafetyController()
{
    maxTiltAngle = 35.0f;
    resetAngle = 5.0f;

    maxSensorAgeMs = 50;
    maxLoopPeriodUs = 20000;

    fault = SAFETY_OK;
    faultLatched = false;
}

void SafetyController::begin()
{
    fault = SAFETY_OK;
    faultLatched = false;
}

void SafetyController::setTiltLimit(float limitDegrees)
{
    if (limitDegrees > 0.0f && limitDegrees < 90.0f)
    {
        maxTiltAngle = limitDegrees;

        // Recovery must be closer to upright than the trip limit.
        resetAngle = limitDegrees * 0.15f;
    }
}

void SafetyController::setSensorAgeLimit(unsigned long limitMs)
{
    if (limitMs > 0)
        maxSensorAgeMs = limitMs;
}

void SafetyController::setLoopPeriodLimit(unsigned long limitUs)
{
    if (limitUs > 0)
        maxLoopPeriodUs = limitUs;
}

bool SafetyController::update(
    float angleDegrees,
    bool sensorValid,
    unsigned long sensorAgeMs,
    unsigned long loopPeriodUs)
{
    // A fault remains latched until explicitly cleared.
    if (faultLatched)
        return false;

    // Check sensor health before trusting the angle.
    if (!sensorValid)
    {
        fault = SAFETY_INVALID_SENSOR;
        faultLatched = true;
        return false;
    }

    if (sensorAgeMs > maxSensorAgeMs)
    {
        fault = SAFETY_STALE_SENSOR;
        faultLatched = true;
        return false;
    }

    // Reject invalid numeric angle values.
    if (isnan(angleDegrees) || isinf(angleDegrees))
    {
        fault = SAFETY_INVALID_SENSOR;
        faultLatched = true;
        return false;
    }

    if (loopPeriodUs == 0 || loopPeriodUs > maxLoopPeriodUs)
    {
        fault = SAFETY_LOOP_OVERRUN;
        faultLatched = true;
        return false;
    }

    if (fabsf(angleDegrees) > maxTiltAngle)
    {
        fault = SAFETY_EXCESSIVE_TILT;
        faultLatched = true;
        return false;
    }

    fault = SAFETY_OK;
    return true;
}

bool SafetyController::isSafe()
{
    return !faultLatched;
}

bool SafetyController::isFaultLatched()
{
    return faultLatched;
}

SafetyFault SafetyController::getFault()
{
    return fault;
}

bool SafetyController::clearFault(
    float angleDegrees,
    bool sensorValid,
    unsigned long sensorAgeMs)
{
    // Explicit recovery conditions.
    if (!faultLatched)
        return true;

    if (!sensorValid || sensorAgeMs > maxSensorAgeMs)
        return false;

    if (isnan(angleDegrees) || isinf(angleDegrees))
        return false;

    if (fabsf(angleDegrees) > resetAngle)
        return false;

    fault = SAFETY_OK;
    faultLatched = false;
    return true;
}
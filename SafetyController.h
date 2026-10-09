
#ifndef SAFETY_CONTROLLER_H
#define SAFETY_CONTROLLER_H

enum SafetyFault
{
    SAFETY_OK,
    SAFETY_EXCESSIVE_TILT,
    SAFETY_INVALID_SENSOR,
    SAFETY_STALE_SENSOR,
    SAFETY_LOOP_OVERRUN
};

class SafetyController
{
private:
    float maxTiltAngle;
    float resetAngle;

    unsigned long maxSensorAgeMs;
    unsigned long maxLoopPeriodUs;

    SafetyFault fault;
    bool faultLatched;

public:
    SafetyController();

    void begin();

    void setTiltLimit(float limitDegrees);
    void setSensorAgeLimit(unsigned long limitMs);
    void setLoopPeriodLimit(unsigned long limitUs);

    bool update(
        float angleDegrees,
        bool sensorValid,
        unsigned long sensorAgeMs,
        unsigned long loopPeriodUs
    );

    bool isSafe();
    bool isFaultLatched();
    SafetyFault getFault();

    // Call only after the cause of the fault is resolved
    // and the robot is in a physically safe condition.
    bool clearFault(
        float angleDegrees,
        bool sensorValid,
        unsigned long sensorAgeMs
    );
};

#endif
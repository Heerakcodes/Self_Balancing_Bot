#include "StateEstimator.h"
#include <Arduino.h>
#include <math.h>

// Complementary filter coefficient
const float ALPHA = 0.98;

// Fall detection threshold
const float FALL_ANGLE = 45.0;

// Constructor
StateEstimator::StateEstimator()
{
    angle = 0.0;
    gyroBias = 0.0;
    previousTime = 0;
}

// Initialize the state estimator
void StateEstimator::begin()
{
    angle = 0.0;
    gyroBias = 0.0;

    previousTime = micros();
}

// Update the estimated state using new IMU readings
void StateEstimator::update(
    float accelX,
    float accelY,
    float accelZ,
    float gyroX,
    float gyroY,
    float gyroZ
)
{
    // --------------------------------------------------
    // 1. Calculate elapsed time
    // --------------------------------------------------

    unsigned long currentTime = micros();

    float dt = (currentTime - previousTime) / 1000000.0;

    previousTime = currentTime;

    // Ignore unrealistic time intervals
    if (dt <= 0.0 || dt > 0.1)
    {
        return;
    }

    // --------------------------------------------------
    // 2. Calculate pitch from accelerometer
    // --------------------------------------------------

    float accelPitch =
        atan2(accelY, accelZ) * 180.0 / PI;

    // --------------------------------------------------
    // 3. Get gyro pitch rate
    // --------------------------------------------------

    // Temporary assumption:
    // Gyro Y measures pitch rotation.
    float gyroRate = gyroY - gyroBias;

    // --------------------------------------------------
    // 4. Integrate gyro
    // --------------------------------------------------

    float gyroAngle = angle + gyroRate * dt;

    // --------------------------------------------------
    // 5. Complementary filter
    // --------------------------------------------------

    angle =
        ALPHA * gyroAngle +
        (1.0 - ALPHA) * accelPitch;
}

// Return the current estimated pitch
float StateEstimator::getPitch()
{
    return angle;
}

// Check whether the robot has fallen
bool StateEstimator::isFallen()
{
    return (fabs(angle) > FALL_ANGLE);
}
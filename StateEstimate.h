#ifndef STATE_ESTIMATOR_H
#define STATE_ESTIMATOR_H

class StateEstimator
{
private:
    float angle;
    float gyroBias;

    unsigned long previousTime;

public:
    StateEstimator();

    void begin();

    void update(
        float accelX,
        float accelY,
        float accelZ,
        float gyroX,
        float gyroY,
        float gyroZ
    );

    float getPitch();

    bool isFallen();
};

#endif
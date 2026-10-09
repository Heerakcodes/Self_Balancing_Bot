
#ifndef BALANCE_CONTROLLER_H
#define BALANCE_CONTROLLER_H

class BalanceController
{
private:
    float Kp;
    float Ki;
    float Kd;

    float targetAngle;
    float angleError;

    float integral;
    float integralLimit;

    float maxOutput;
    float output;

    unsigned long previousTime;
    bool firstUpdate;

public:
    BalanceController();

    void begin();

    void setPID(float kp, float ki, float kd);
    void setTargetAngle(float angle);
    void setOutputLimit(float limit);

    void reset();

    float update(float currentAngle, float gyroRate);

    float getOutput();
    float getError();
};

#endif
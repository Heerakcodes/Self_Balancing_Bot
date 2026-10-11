#ifndef BALANCE_CONTROLLER_H
#define BALANCE_CONTROLLER_H

class BalanceController{
private:
  float Kp;
  float Ki;
  float Kd;
  float targetAngle;
  float integral;
  float outputMin;
  float outputMax;
  float integralMin;
  float integralMax;

public:
  BalanceController(float Kp, float Ki, float  Kd);

  void setTarget(float target);

  void setOutputLimits(float minOutput, float maxOutput);

  void setIntegralLimits(float minIntegral, float maxIntegral);

  float compute(float currentAngle, float gyroRate, float dt);

  void reset();

  float getKp();
  float getKi();
  float getKd();

};

#endif
#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

class PIDController{
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
  PIDController(float Kp, float Ki, float  Kd);

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
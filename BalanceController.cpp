#include "BalanceController.h"

BalanceController::BalanceController(float Kp, float Ki , float Kd){
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;

  targetAngle =0.0;
  integral =0.0;
  outputMin= -1000.0;
  outputMax= 1000.0;

  integralMin= -100.0;
  integralMax= 100.0;
}

void BalanceController::setTarget(float target){
  targetAngle = target;
}
void BalanceController::setOutputLimits(float minOutput, float maxOutput){
  outputMin = minOutput;
  outputMax = maxOutput;
}
void BalanceController::setIntegralLimits(float minIntegral, float maxIntegral){
  integralMin = minIntegral;
  integralMax = maxIntegral;
}
float BalanceController::compute(float currentAngle, float gyroRate, float dt){
  if(dt <= 0.0){
    return 0.0;
  }
  float error = targetAngle - currentAngle;
  float P = Kp * error;
  integral += error*dt;
  if(integral > integralMax){
    integral = integralMax;
  }
  if(integral < integralMin){
    integral = integralMin;
  }
  float I = Ki*integral;
  float D = -Kd*gyroRate;
  float output = P+I+D;
  if(output>outputMax){
    output = outputMax;
  }
  if(output< outputMin){
    output = outputMin;
  }
  return output;
}

float BalanceController::getKp(){
  return Kp;
}

float  BalanceController::getKi(){
  return Ki;
}
float BalanceController::getKd(){
  return Kd;
}
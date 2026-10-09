#include "PIDController.h"

PIDController::PIDController(float Kp, float Ki , float Kd){
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

void PIDController::setTarget(float target){
  targetAngle = target;
}
void PIDController::setOutputLimits(float minOutput, float maxOutput){
  outputMin = minOutput;
  outputMax = maxOutput;
}
void PIDController::setIntegralLimits(float minIntegral, float maxIntegral){
  integralMin = minIntegral;
  integralMax = maxIntegral;
}
float PIDController::compute(float currentAngle, float gyroRate, float dt){
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

float PIDController::getKp(){
  return Kp;
}

float  PIDController::getKi(){
  return Ki;
}
float PIDController::getKd(){
  return Kd;
}
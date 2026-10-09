#include "StepperMotor.h"


StepperMotor::StepperMotor(int stepPin, int dirPin, int enablePin){
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->enablePin = enablePin;
}

void StepperMotor::begin(){
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enablePin, OUTPUT);

    disable();
}

void StepperMotor::enable(){
    digitalWrite(enablePin, LOW);
}

void StepperMotor::disable(){
    digitalWrite(enablePin, HIGH);
}

void StepperMotor::setDirection(bool direction){
    digitalWrite(dirPin, direction);
}

void StepperMotor::step(){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);

    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
}
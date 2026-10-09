#include <Arduino.h>
#include "PIDController.h"
#include "IMUSensor.h"

IMUSensor imu;

PIDController pid(20.0 , 0.0 , 1.0);
unsigned long lastTime =0;

const int STEP_PIN = 26;
const int DIR_PIN = 25;
const int ENA_PIN = 27;

void setup(){

    Serial.begin(115200);

    imu.begin();
    delay(1000);
    imu.calibrateGyro();
    delay(500);
    lastTime = micros();

    pinMode(DIR_PIN , OUTPUT);
    pinMode(ENA_PIN , OUTPUT);
    pinMode(STEP_PIN , OUTPUT);
    
    digitalWrite(DIR_PIN, HIGH);
    digitalWrite(ENA_PIN, LOW);

    pid.setTarget(0.0);
    pid.setOutputLimits(-1000,1000);
    pid.setIntegralLimits(-100,100);
    
    
    for (int delayTime = 3000; delayTime > 300; delayTime -= 5) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(delayTime);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(delayTime);
    }
}

void loop() {
    
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(300); 
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(300);

    imu.update();

    float currentAngle =imu.getAngle();
    float gyroRate =imu.getGyroRate();

    unsigned long currentTime = micros();
    float dt = (currentTime - lastTime)/ 1000000.0;
    lastTime = currentTime;

    float output = pid.compute(currentAngle, gyroRate, dt);

    Serial.print("Angle: ");
    Serial.print(currentAngle);
    Serial.print("| Gyro: ");
    Serial.print(gyroRate);
    Serial.print(" | dt: ");
    Serial.print(dt, 4);
    Serial.print(" | Output: ");
    Serial.println(output);
    



    
    delay(5);


}


#include <Arduino.h>
#include "PIDController.h"
#include "IMUSensor.h"
#include "StepperMotor.h"

IMUSensor imu;

PIDController pid(20.0, 0.0, 1.0);

// Left motor: STEP 25, DIR 26, ENA 33
StepperMotor leftMotor(25, 26, 33);

// Right motor: STEP 19, DIR 18, ENA 17
StepperMotor rightMotor(19, 18, 17);

unsigned long lastTime = 0;

// Change to -1 if the right motor direction needs reversing
const int RIGHT_MOTOR_SIGN = 1;

void setup()
{
    Serial.begin(115200);

    // Initialize and calibrate IMU
    imu.begin();
    delay(1000);

    imu.calibrateGyro();
    delay(500);

    lastTime = micros();

    // Initialize motors
    leftMotor.begin();
    rightMotor.begin();

    leftMotor.enable();
    rightMotor.enable();

    // Initialize PID
    pid.setTarget(0.0);
    pid.setOutputLimits(-1000, 1000);
    pid.setIntegralLimits(-100, 100);

    Serial.println("Self-balancing controller started");
}

void loop()
{
    // Read sensor
    imu.update();

    float currentAngle = imu.getAngle();
    float gyroRate = imu.getGyroRate();

    // Calculate elapsed time
    unsigned long currentTime = micros();

    float dt = (currentTime - lastTime) / 1000000.0f;
    lastTime = currentTime;

    // Calculate PID output
    float output = pid.compute(currentAngle, gyroRate, dt);

    // Use PID output as motor speed in steps per second
    float leftSpeed = output;
    float rightSpeed = output * RIGHT_MOTOR_SIGN;

    // Send speed commands to motors
    leftMotor.setSpeed(leftSpeed);
    rightMotor.setSpeed(rightSpeed);

    // Generate step pulses
    leftMotor.runSpeed();
    rightMotor.runSpeed();

    // Keep your Serial Monitor output
    Serial.print("Angle: ");
    Serial.print(currentAngle);

    Serial.print(" | Gyro: ");
    Serial.print(gyroRate);

    Serial.print(" | dt: ");
    Serial.print(dt, 4);

    Serial.print(" | Output: ");
    Serial.println(output);
}
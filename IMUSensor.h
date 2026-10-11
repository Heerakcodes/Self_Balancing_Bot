#ifndef IMU_SENSOR_H
#define IMU_SENSOR_H

#include <Arduino.h>
#include <Wire.h>

class IMUSensor{
private:
    uint8_t address;

    float angle;
    float gyroRate;

    float accAngle;
    float alpha;

    float gyroOffsetX;
    float gyroOffsetY;
    float gyroOffsetZ;

    unsigned long lastTime;

    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    void readSensorData();

public:
    IMUSensor();

    void begin();
    void calibrateGyro();
    void update();

    float getAngle();
    float getGyroRate();
};

#endif
#include "IMUSensor.h"

IMUSensor::IMUSensor()
{
    address = 0x68;
    angle = 0.0;
    gyroRate = 0.0;
    lastTime = 0;
}

void IMUSensor::writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t IMUSensor::readRegister(uint8_t reg)
{
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(address, (uint8_t)1);

    if (Wire.available())
    {
        return Wire.read();
    }

    return 0;
}

void IMUSensor::begin()
{
    // ESP32 I2C pins
    Wire.begin(21, 22);

    delay(100);

    Serial.println("Starting MPU6050...");

    writeRegister(0x6B, 0x00);

    delay(100);

  
    writeRegister(0x1C, 0x10);

    
    writeRegister(0x1B, 0x08);

    writeRegister(0x1A, 0x03);

    delay(100);

    Serial.println("MPU6050 initialized!");

    Serial.print("WHO_AM_I = 0x");
    Serial.println(readRegister(0x75), HEX);

    angle = 0.0;
    gyroRate = 0.0;

    lastTime = millis();
}

void IMUSensor::readSensorData()
{
    Wire.beginTransmission(address);
    Wire.write(0x3B);
    Wire.endTransmission(false);

    Wire.requestFrom(address, (uint8_t)14);

    if (Wire.available() < 14)
    {
        Serial.println("Sensor data read failed!");
        return;
    }

    int16_t accelX = (Wire.read() << 8) | Wire.read();
    int16_t accelY = (Wire.read() << 8) | Wire.read();
    int16_t accelZ = (Wire.read() << 8) | Wire.read();

   
    Wire.read();
    Wire.read();

    int16_t gyroX = (Wire.read() << 8) | Wire.read();
    int16_t gyroY = (Wire.read() << 8) | Wire.read();
    int16_t gyroZ = (Wire.read() << 8) | Wire.read();

   
    float ax = (float)accelX / 4096.0;
    float ay = (float)accelY / 4096.0;
    float az = (float)accelZ / 4096.0;

   
    gyroRate = (float)gyroX / 65.5;

   
    angle = atan2(ay, az) * 180.0 / PI;

   
    Serial.print("Accel X: ");
    Serial.print(ax, 2);

    Serial.print(" | Y: ");
    Serial.print(ay, 2);

    Serial.print(" | Z: ");
    Serial.print(az, 2);

    Serial.print(" | Angle: ");
    Serial.print(angle, 2);

    Serial.print(" | Gyro X: ");
    Serial.println(gyroRate, 2);
}

void IMUSensor::update()
{
    readSensorData();
}

float IMUSensor::getAngle()
{
    return angle;
}

float IMUSensor::getGyroRate()
{
    return gyroRate;
}